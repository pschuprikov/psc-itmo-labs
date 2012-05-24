module Driver where
import Tokenizer;
import Parser;
import System.Environment;

data DeclUnit = DeclUnit TypeDecl (Maybe String)

gen_arg_decl :: [DeclUnit] -> String
gen_arg_decl args = "(" ++ (gen_arg_list args "") ++ ")"

gen_arg_list :: [DeclUnit] -> String -> String
gen_arg_list [] acc = acc
gen_arg_list (a:as) acc = case as of 
                              [] -> gen_arg_list as $ gen_type a Nothing ++ acc
                              _ -> gen_arg_list as $ ',' : gen_type a Nothing ++ acc 

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
                                                            (Just name, Nothing) -> ' ' : name
                                                            (Nothing, Nothing) -> "" 
gen_type (DeclUnit (ComplexType (a:as)) mname) mcaller = 
    let icaller_args = gen_arg_decl . map (\x -> DeclUnit x Nothing) $ as in
       let icaller = case (mname, mcaller) of
                         (Nothing, Just caller) -> "(*" ++ caller ++ ")" ++ icaller_args
                         (Just name, Nothing) -> "(*" ++ name ++ ")" ++ icaller_args
                         (Nothing, Nothing) -> "(*)" ++ icaller_args  
                     in
           gen_type (DeclUnit a Nothing) (Just icaller)

gen_args_names :: Int -> Int -> [String]
gen_args_names k n | k == n = ('_' : [toEnum n]) : []
                  | otherwise = ('_' : [toEnum (fromEnum 'a' + k)]) : (gen_args_names (k + 1) n)

gen_function :: Function -> IO ()
gen_function (Function id types clauses) = let ComplexType (r:as) = simplify_type types in 
                                               let arg_list = gen_args_names 0 (length as) in
                                                   putStrLn $ gen_type (DeclUnit r Nothing) $ Just $ id ++ gen_arg_decl (zipWith DeclUnit as . map Just $ arg_list)

translate :: Language -> IO ()
translate functions = mapM_ gen_function functions
