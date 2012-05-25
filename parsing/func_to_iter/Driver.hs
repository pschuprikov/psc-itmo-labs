module Driver where
import Tokenizer;
import Parser;
import System.Environment;
import Control.Monad.State;

-- named type declaration
data DeclUnit = DeclUnit TypeDecl (Maybe String)

-- strores information about current builded function, used lambda functionsi,
-- last used in this function lambda's index and signatures of all functions
data GenerationState = GenerationState { funcName    :: String
                                       , lambdas     :: [Function]
                                       , lastLambda  :: Int
                                       , funcDecls   :: [String]
                                       , lambdaDecls :: [String]}
type GSM = State GenerationState

--
-- Interactions with GenerationState. Pretty straightforward.
--

startFunction :: String -> GSM ()
startFunction fname = modify (\s -> 
    GenerationState { funcName    = fname
                    , lambdas     = []
                    , lastLambda  = 0
                    , funcDecls   = funcDecls s
                    , lambdaDecls = lambdaDecls s}) 

addFunctionDeclaration :: String -> GSM ()
addFunctionDeclaration decl = modify (\s -> 
    if (head . funcName $ s) == '_' then 
        GenerationState { funcName    = funcName s
                        , lambdas     = lambdas s
                        , lastLambda  = lastLambda s
                        , funcDecls   = funcDecls s
                        , lambdaDecls = decl : (lambdaDecls s)
                        }
    else
        GenerationState { funcName    = funcName s
                        , lambdas     = lambdas s
                        , lastLambda  = lastLambda s
                        , funcDecls   = decl : (funcDecls s)
                        , lambdaDecls = lambdaDecls s
                        })
                                                

addLambda :: Function -> GSM ()
addLambda f = modify (\s -> 
    GenerationState { funcName    = funcName s
                    , lambdas     = f : (lambdas s)
                    , lastLambda  = lastLambda s + 1
                    , funcDecls   = funcDecls s
                    , lambdaDecls = lambdaDecls s
                    })

getLambdaAndFinish :: GSM [Function] 
getLambdaAndFinish = do { lmbds <- gets (\ (GenerationState { lambdas = ls }) -> ls)
                        ; modify (\(GenerationState { funcDecls   = fds
                                                    , lambdaDecls = lmbdds
                                                    }) -> 
                                      GenerationState {funcDecls    = fds
                                                      , lambdaDecls = lmbdds
                                                      })
                        ; return lmbds
                        }

-- | Generates name of next lambda is prefix "_lambda" plus its index in owner function 
-- and finally owner function name.
getNextLambdaName :: GSM String
getNextLambdaName = gets (\(GenerationState { funcName   = fname
                                            , lastLambda = ll 
                                            }) -> 
                              "_lambda" ++ (show ll) ++ fname)

getFunctionName :: GSM String
getFunctionName = gets (\ (GenerationState { funcName = fname }) -> fname)

--
-- Argument list utilities. 
--

genArgDecl :: [DeclUnit] -> String
genArgDecl args = "(" ++ (genArgList (map (\t -> genType t Nothing) args) "") ++ ")"

genArgList :: [String] -> String -> String
genArgList [] acc = acc
genArgList (a:as) acc = case as of 
    [] -> genArgList as $ a ++ acc
    _  -> genArgList as $ ", " ++ a ++ acc 

--
-- Type signature generation stuff.
--

-- | Changes ComplexType [t] -> t. 
simplifyType :: TypeDecl -> TypeDecl
simplifyType TypeInteger = TypeInteger
simplifyType TypeVoid = TypeVoid
simplifyType (ComplexType ts@(a:as)) = case as of 
    [] -> simplifyType a
    _  -> ComplexType . map simplifyType $ ts

-- | Generates type's string representation, using named type,
-- caller string representation (for functional parents).
genType :: DeclUnit -> Maybe String -> String
genType (DeclUnit _ (Just name)) (Just caller) = error "named return"
genType (DeclUnit TypeInteger mname) mcaller = "int" ++ case (mname, mcaller) of 
    (Nothing, Just caller) -> ' ' : caller
    (Just name, Nothing)   -> ' ' : name
    (Nothing, Nothing)     -> ""
genType (DeclUnit TypeVoid mname) mcaller = "void" ++ case (mname, mcaller) of 
    (Nothing, Just caller) -> ' ' : caller
    (Just name, Nothing)   -> ""
    (Nothing, Nothing)     -> "" 
genType (DeclUnit (ComplexType (a:as)) mname) mcaller = 
    let iCallerArgs = genArgDecl . map (\x -> DeclUnit x Nothing) $ as in
       let iCaller = case (mname, mcaller) of
               (Nothing, Just caller) -> "(*" ++ caller ++ ")" ++ iCallerArgs
               (Just name, Nothing)   -> "(*" ++ name ++ ")" ++ iCallerArgs
               (Nothing, Nothing)     -> "(*)" ++ iCallerArgs  
           in genType (DeclUnit a Nothing) (Just iCaller)

-- 
-- Argument names generators
--

genArgName :: Int -> String
genArgName k = '_' : [toEnum (fromEnum 'a' + k)]

genArgsNames :: Int -> Int -> [String]
genArgsNames k n | k == n = genArgName k : []
                 | otherwise = genArgName k : (genArgsNames (k + 1) n)

--
-- Trivial pattern matching stuff.
--

-- | Generates assignments from actual arguments to variables, declared in current clause.
genArgAssignment :: TypeDecl -- ^ Type of argument
                   -> Argument -- ^ Argument variable of clause (numeric and empty ignored)
                   -> String -- ^ Name of actual argument
                   -> Maybe String -- ^ Returns Nothing if argument decl isn't a var name
genArgAssignment t AnyArg _ = Nothing
genArgAssignment t (NumericArg _) _ = Nothing
genArgAssignment t (NamedArg var) str = Just (genType (DeclUnit t (Just var)) Nothing  
    ++ " = " ++ str ++ ";\n")

genArgConditions :: [Argument] -> Int -> LExpr
genArgConditions [] 0 = LTrue
genArgConditions ((NamedArg _):as) k = genArgConditions as (k - 1) 
genArgConditions (AnyArg:as) k = genArgConditions as (k - 1)
genArgConditions ((NumericArg n):as) k = And (Eq (Num n) (Var $ genArgName (k - 1))) 
    $ genArgConditions as (k - 1)

--
-- Expression generators stuff. Logical is clear.
--

genBinaryOp :: a -> a -> (a -> GSM String ) -> String  -> GSM String
genBinaryOp a b gen op = do { as <- gen a
                            ; bs <- gen b
                            ; return $ "(" ++ as ++ ") " ++ op ++ " (" ++ bs ++ ")"
                            }

genLExpr :: LExpr -> GSM String
genLExpr (Or l1 l2) = genBinaryOp l1 l2 genLExpr "||" 
genLExpr (And l1 l2) = genBinaryOp l1 l2 genLExpr "&&"
genLExpr (Not l1) = do { ls <- genLExpr l1
                       ; return $ '!' : "(" ++ ls ++ ")"
                       }
genLExpr (Eq e1 e2) = genBinaryOp e1 e2 genExpr "=="
genLExpr (Gr e1 e2) = genBinaryOp e1 e2 genExpr ">"
genLExpr (Less e1 e2) = genBinaryOp e1 e2 genExpr "<"
genLExpr LTrue = return "true"
genLExpr LFalse = return "false"

-- | Generates string representation of an expression. Note a lambda addition to Generation State.
genExpr :: Expr -> GSM String
genExpr (Plus e1 e2) = genBinaryOp e1 e2 genExpr "+"
genExpr (Sub e1 e2) = genBinaryOp e1 e2 genExpr "-"
genExpr (Mult e1 e2) = genBinaryOp e1 e2 genExpr "*"
genExpr (Div e1 e2) = genBinaryOp e1 e2 genExpr "/"
genExpr (Var name) = return name
genExpr (Num int) = return (show int)
genExpr (IfClause le e1 e2) = 
    do { les <- genLExpr le
       ; e1s <- genExpr e1
       ; e2s <- genExpr e2
       ; return $ "(" ++ les ++ ") ? (" ++ e1s ++ ")\n\t: (" ++ e2s ++ ")"
       }
genExpr (FunctionCall expr exprs) = 
    do { caller <- genExpr expr
       ; args <- mapM genExpr exprs
       ; return $ "(" ++ caller ++ ")(" ++ (genArgList args "") ++ ")"
       }
genExpr (Lambda args t expr) = 
    do { lmbdName <- getNextLambdaName
       ; addLambda (Function lmbdName t [FunctionClause lmbdName args LTrue expr])
       ; return $ lmbdName
       }

-- | Generates function clause in separate code block. 
-- Firstly checks pattern-matching and guard then executes expression.
genFunctionClause :: [String] -> [TypeDecl] -> FunctionClause -> GSM String
genFunctionClause argnames types (FunctionClause id args grd expr) 
    = let assignments = concat . map (\(Just s) -> s) . filter (\s -> case s of 
                                                                        Nothing -> False
                                                                        _ -> True) 
             . zipWith3 genArgAssignment types args $ argnames in
          let conditions = genArgConditions args (length args) in
            do { conditions_str <- genLExpr (And conditions grd)
               ; exprs <- genExpr expr
               ; return $ "{\n" ++ assignments ++ "\nif (" ++ conditions_str 
                     ++ ")\n\treturn (" ++ exprs ++ ");\n}\n"
               }

-- | Generates function string representation, clause by clause. 
-- At the end goes into recursion for found lambdas.
genFunction :: Function -> GSM String
genFunction (Function id types clauses) = 
    let ComplexType (r:as) = simplifyType types in 
        let argCnt = length as in
            let argList = genArgsNames 0 (length as) in
                do { startFunction id
                ; funcdecl <- return $ genType (DeclUnit r Nothing) $ Just 
                      $ id ++ genArgDecl (zipWith DeclUnit as . map Just $ argList)
                ; addFunctionDeclaration funcdecl
                ; clauses <- sequence . map (genFunctionClause argList as) . reverse $ clauses
                ; lambdas <- getLambdaAndFinish
                ; lambdasStrs <- mapM genFunction lambdas
                ; return $ funcdecl ++ "\n{\n" ++ (concat clauses) ++ "\n}\n\n" ++ (concat lambdasStrs)
                }
-- | Simple driver. Firstly gens header, then forwards of lambdas and finally the code.
translate :: Language -> String -> IO ()
translate functions name = let (res, (GenerationState {funcDecls = fds, lambdaDecls = lmbds})) 
                                = runState (mapM genFunction functions) 
                                      (GenerationState { lambdas = []
                                                       , funcDecls = []
                                                       , lambdaDecls = []
                                                       }) in 
    do { writeFile (name ++ ".h") (concat . map (\s -> "extern " ++ s ++ ";\n") $ fds)
       ; writeFile (name ++ ".cpp") 
             ("#include \"" ++ name ++ ".h\"\n\n" 
                  ++ (concat . map (\s -> s ++ ";\n") $ lmbds) ++ "\n" ++ (concat res))
       }
