module Main where
import Tokenizer;
import Parser;
import System.Environment;
import Driver;

main :: IO ()
main = do { args <- getArgs
          ; fileName <- return (args !! 0)
          ; funcSource <- readFile fileName
          ; parsed <- return . parse . alexScanTokens $ funcSource
          ; case parsed of 
                OK lang -> translate lang
                Failed msg -> putStrLn msg
          }
