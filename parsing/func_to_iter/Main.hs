module Main where
import Tokenizer;
import Parser;
import System.Environment;
import Driver;

main :: IO ()
main = do { args <- getArgs
          ; fileName <- return (args !! 0)
          ; modName <- return (args !! 1)
          ; funcSource <- readFile fileName
          ; parsed <- return . parse . alexScanTokens $ funcSource
          ; case parsed of 
                OK lang -> translate lang modName
                Failed msg -> error msg
          }
