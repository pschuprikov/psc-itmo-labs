module Driver where
import Tokenizer;
import Parser;
import System.Environment;
import Control.Monad.State;

data DeclUnit = DeclUnit TypeDecl (Maybe String)

data GenerationState = GenerationState {func_name :: String, lambdas :: [Function], last_lambda :: Int, func_decls :: [String]}
type GSM = State GenerationState

start_function :: String -> GSM ()
start_function fname = modify (\s -> GenerationState {func_name = fname, lambdas = [], last_lambda = 0, func_decls = func_decls s}) 

add_func_decl :: String -> GSM ()
add_func_decl decl = modify (\s -> GenerationState {func_name = func_name s, 
                                                    lambdas = lambdas s, 
                                                    last_lambda = last_lambda s, 
                                                    func_decls = decl : (func_decls s)})

add_lambda :: Function -> GSM ()
add_lambda f = modify (\s -> GenerationState { func_name = func_name s,
                                               lambdas = f : (lambdas s),
                                               last_lambda = last_lambda s + 1,
                                               func_decls = func_decls s})

get_lambdas_and_end :: GSM [Function] 
get_lambdas_and_end = do { lmbds <- gets (\ (GenerationState { lambdas = ls }) -> ls)
                         ; modify (\(GenerationState { func_decls = fds }) -> GenerationState {func_decls = fds})
                         ; return lmbds
                         }

get_next_lambda_name :: GSM String
get_next_lambda_name = gets (\(GenerationState { func_name = fname, last_lambda = ll }) -> "_lambda" ++ (show ll) ++ fname)

get_function_name :: GSM String
get_function_name = gets (\ (GenerationState { func_name = fname }) -> fname)

gen_arg_decl :: [DeclUnit] -> String
gen_arg_decl args = "(" ++ (gen_arg_list (map (\t -> gen_type t Nothing) args) "") ++ ")"

gen_arg_list :: [String] -> String -> String
gen_arg_list [] acc = acc
gen_arg_list (a:as) acc = case as of 
                              [] -> gen_arg_list as $ a ++ acc
                              _ -> gen_arg_list as $ ", " ++ a ++ acc 

simplify_type :: TypeDecl -> TypeDecl
simplify_type TypeInteger = TypeInteger
simplify_type TypeVoid = TypeVoid
simplify_type (ComplexType ts@(a:as)) = case as of 
                                             [] -> simplify_type a
                                             _  -> ComplexType . map simplify_type $ ts

gen_type :: DeclUnit -> Maybe String -> String
gen_type (DeclUnit _ (Just name)) (Just caller) = error "named return"
gen_type (DeclUnit TypeInteger mname) mcaller = "int" ++ case (mname, mcaller) of 
                                                              (Nothing, Just caller) -> ' ' : caller
                                                              (Just name, Nothing) -> ' ' : name
                                                              (Nothing, Nothing) -> ""
gen_type (DeclUnit TypeVoid mname) mcaller = "void" ++ case (mname, mcaller) of 
                                                            (Nothing, Just caller) -> ' ' : caller
                                                            (Just name, Nothing) -> ""
                                                            (Nothing, Nothing) -> "" 
gen_type (DeclUnit (ComplexType (a:as)) mname) mcaller = 
    let icaller_args = gen_arg_decl . map (\x -> DeclUnit x Nothing) $ as in
       let icaller = case (mname, mcaller) of
                         (Nothing, Just caller) -> "(*" ++ caller ++ ")" ++ icaller_args
                         (Just name, Nothing) -> "(*" ++ name ++ ")" ++ icaller_args
                         (Nothing, Nothing) -> "(*)" ++ icaller_args  
                     in
           gen_type (DeclUnit a Nothing) (Just icaller)

gen_arg_name :: Int -> String
gen_arg_name k = '_' : [toEnum (fromEnum 'a' + k)]

gen_args_names :: Int -> Int -> [String]
gen_args_names k n | k == n = gen_arg_name k : []
                   | otherwise = gen_arg_name k : (gen_args_names (k + 1) n)

gen_kth_arg_copy :: Int -> String -> String 
gen_kth_arg_copy n var = take n . repeat $ '_'

gen_arg_assignment :: TypeDecl -> Argument -> String -> Maybe String 
gen_arg_assignment t AnyArg _ = Nothing
gen_arg_assignment t (NumericArg _) _ = Nothing
gen_arg_assignment t (NamedArg var) str = Just (gen_type (DeclUnit t (Just var)) Nothing  ++ " = " ++ str ++ ";\n")

gen_arg_conditions :: [Argument] -> Int -> LExpr
gen_arg_conditions [] 0 = LTrue
gen_arg_conditions ((NamedArg _):as) k = gen_arg_conditions as (k - 1) 
gen_arg_conditions (AnyArg:as) k = gen_arg_conditions as (k - 1)
gen_arg_conditions ((NumericArg n):as) k = And (Eq (Num n) (Var $ gen_arg_name k)) $ gen_arg_conditions as (k - 1)

gen_lexpr :: LExpr -> GSM String
gen_lexpr (Or l1 l2) = do { l1s <- gen_lexpr l1
                           ; l2s <- gen_lexpr l2
                           ; return $ "(" ++ l1s ++ ") || (" ++ l2s ++ ")"
                           }
gen_lexpr (And l1 l2) = do { l1s <- gen_lexpr l1
                            ; l2s <- gen_lexpr l2
                            ; return $ "(" ++ l1s ++ ") && (" ++ l2s ++ ")"
                            }
gen_lexpr (Not l1) = do { ls <- gen_lexpr l1
                        ; return $ '!' : "(" ++ ls ++ ")"
                        }
gen_lexpr (Eq e1 e2) = do { e1s <- gen_expr e1
                          ; e2s <- gen_expr e2
                          ; return $ "(" ++ e1s ++ ") == (" ++ e2s ++ ")"
                          }
gen_lexpr (Gr e1 e2) = do { e1s <- gen_expr e1
                          ; e2s <- gen_expr e2
                          ; return $ "(" ++ e1s ++ ") > (" ++ e2s ++ ")"
                          }
gen_lexpr (Less e1 e2) = do { e1s <- gen_expr e1
                            ; e2s <- gen_expr e2
                            ; return $ "(" ++ e1s ++ ") < (" ++ e2s ++ ")"
                            }
gen_lexpr LTrue = return "true"
gen_lexpr LFalse = return "false"

gen_expr :: Expr -> GSM String
gen_expr (Plus e1 e2) = do { e1s <- gen_expr e1
                           ; e2s <- gen_expr e2
                           ; return $ "(" ++ e1s ++ ") + (" ++ e2s ++ ")"
                           }
gen_expr (Sub e1 e2) = do { e1s <- gen_expr e1
                          ; e2s <- gen_expr e2
                          ; return $ "(" ++ e1s ++ ") - (" ++ e2s ++ ")"
                          }
gen_expr (Mult e1 e2) = do { e1s <- gen_expr e1
                           ; e2s <- gen_expr e2
                           ; return $ "(" ++ e1s ++ ") * (" ++ e2s ++ ")"
                           }
gen_expr (Div e1 e2) = do { e1s <- gen_expr e1
                          ; e2s <- gen_expr e2
                          ; return $ "(" ++ e1s ++ ") / (" ++ e2s ++ ")"
                          }
gen_expr (Var name) = return name
gen_expr (Num int) = return (show int)
gen_expr (IfClause le e1 e2) = do { les <- gen_lexpr le
                                  ; e1s <- gen_expr e1
                                  ; e2s <- gen_expr e2
                                  ; return $ "(" ++ les ++ ") ? (" ++ e1s ++ ")\n\t: (" ++ e2s ++ ")"
                                  }
gen_expr (FunctionCall expr exprs) = do { caller <- gen_expr expr
                                        ; args <- mapM gen_expr exprs
                                        ; return $ "(" ++ caller ++ ")(" ++ (gen_arg_list args "") ++ ")"
                                        }
gen_expr (Lambda args t expr) = do { lmbd_name <- get_next_lambda_name
                                   ; add_lambda (Function lmbd_name t [FunctionClause lmbd_name args LTrue expr])
                                   ; return $ lmbd_name
                                   }

gen_function_clause :: [String] -> [TypeDecl] -> FunctionClause -> GSM String
gen_function_clause argnames types (FunctionClause id args grd expr) 
    = let assignments = concat . map (\(Just s) -> s) . filter (\s -> case s of 
                                                                        Nothing -> False
                                                                        _ -> True) 
             . zipWith3 gen_arg_assignment types args $ argnames in
          let conditions = gen_arg_conditions args (length args) in
            do { conditions_str <- gen_lexpr (And conditions grd)
               ; exprs <- gen_expr expr
               ; return $ "{\n" ++ assignments ++ "\nif (" ++ conditions_str ++ ")\n\treturn (" ++ exprs ++ ");\n}\n"
               }

gen_function :: Function -> GSM String
gen_function (Function id types clauses) = let ComplexType (r:as) = simplify_type types in 
                                               let arg_cnt = length as in
                                                   let arg_list = gen_args_names 0 (length as) in
                                                       do { start_function id
                                                          ; funcdecl <- return $ gen_type (DeclUnit r Nothing) $ Just 
                                                                        $ id ++ gen_arg_decl (zipWith DeclUnit as . map Just $ arg_list)
                                                          ; add_func_decl funcdecl
                                                          ; clauses <- sequence . map (gen_function_clause arg_list as) . reverse $ clauses
                                                          ; lambdas <- get_lambdas_and_end
                                                          ; lambdas_strs <- mapM gen_function lambdas
                                                          ; return $ funcdecl ++ "\n{\n" ++ (concat clauses) ++ "\n}\n\n" ++ (concat lambdas_strs)
                                                          }

translate :: Language -> IO ()
translate functions = let (res, (GenerationState {func_decls = fds})) = runState (mapM gen_function functions) (GenerationState {lambdas = [], func_decls = [] }) in 
                        do { mapM_ putStrLn . map (\s -> s ++ ";\n") $ fds
                           ; mapM_ putStrLn res
                           }
