{
module Parser where
import Tokenizer
}

%name parse
%tokentype { TokenPos }
%monad { ParsingState }
%error { parseError }

%token
    '(' { TokenPos TokenOP _ }
    ')' { TokenPos TokenCP _ }
    '{' { TokenPos TokenOB _ }
    '}' { TokenPos TokenCB _ }
    '=' { TokenPos TokenAssign _ }
    '==' { TokenPos TokenEq _ }
    '!=' { TokenPos TokenNotEq _ }
    '<' { TokenPos TokenLess _ }
    '<=' { TokenPos TokenLessEq _ }
    '>' { TokenPos TokenGr _ }
    '>=' { TokenPos TokenGrEq _ }
    '+' { TokenPos TokenPlus _ }
    '-' { TokenPos TokenMinus _ }
    '*' { TokenPos TokenStar _ }
    '/' { TokenPos TokenDiv _ }
    '&&' { TokenPos TokenAnd _ }
    '||' { TokenPos TokenOr _ }
    '\\' { TokenPos TokenLambda _ }
    '!' { TokenPos TokenNot _ }
    if { TokenPos TokenIf _ }
    then { TokenPos TokenThen _ }
    else { TokenPos TokenElse _ }
    fi { TokenPos TokenFi _ }
    id { TokenPos (TokenId $$) _ }
    num { TokenPos (TokenInteger $$) _ }
    int { TokenPos TokenTypeInteger _ }
    void { TokenPos TokenTypeVoid _ }
    '->' { TokenPos TokenArrow _ }
    '::' { TokenPos TokenOfType _ }
    any { TokenPos TokenAny _ }
    '.' { TokenPos TokenPeriod _ }
    ';' { TokenPos TokenSemicolon _ }
    ',' { TokenPos TokenComma _ }

%left '*' '+' '-' '/' '||' '&&'
%left '!'
%nonassoc '==' '!=' '<' '>' '<=' '>=' 

%%

Language : FunctionS { $1 }

FunctionS : { [] }
          | FunctionM { $1 }

FunctionM : Function { [$1] }
          | FunctionM Function { $2 : $1 }

Function : id '::' SignatureM FunctionClauseM '.' { Function $1 (ComplexType $3) $4 }

Signature : int  { TypeInteger }
          | void { TypeVoid }
          | '(' SignatureM ')' { ComplexType $2 }

SignatureM : Signature { [$1] }
           | SignatureM '->' Signature { $3 : $1 }


FunctionClauseM : FunctionClause { [$1] }
                | FunctionClauseM ';' FunctionClause { $3 : $1 }

FunctionClause : id ArgumentS '=' Expr { FunctionClause $1 $2 $4 }

ArgumentS : { [] }
          | ArgumentM { $1 }

ArgumentM : Argument { [$1] }
          | ArgumentM Argument { $2 : $1 }
         
Argument : id { NamedArg $1 }
         | num { NumericArg $1 }


Expr : Expr '+' Term { Plus $1 $3 }
     | Term '-' Term { Sub $1 $3 }
     | Term { $1 }

Term : Term '*' Factor { Mult $1 $3 }
     | Term '/' Factor { Div $1 $3 }
     | Factor { $1 }


Factor : Callable { $1 } 
       | num { Num $1 }
       | Callable '(' CallerArgList ')' { FunctionCall $1 $3 }

Callable : id { Var $1 }
         | IfClause { $1 }
         | '(' Expr ')' { $2 }
         | '\\' ArgumentS '::' SignatureM '{' Expr '}' { Lambda $2 (ComplexType $4) $6 }

CallerArgList : Expr  { [$1] }
              | CallerArgList ',' Expr { $3 : $1 }

IfClause : if '(' LExpr ')' then Expr else Expr fi { IfClause $3 $6 $8 }

LExpr : LExpr '||' LTerm { Or $1 $3 }
      | LTerm { $1 }

LTerm : LTerm '&&' LFactor { And $1 $3 }
      | LFactor { $1 }

LFactor : '!' LFactor { Not $2 }
        | Expr '==' Expr { Eq $1 $3 }
        | Expr '!=' Expr { Not . Eq $1 $ $3 }
        | Expr '<' Expr { Less $1 $3 }
        | Expr '>' Expr { Gr $1 $3 }
        | Expr '<=' Expr { Or (Less $1 $3) (Eq $1 $3) }
        | Expr '>=' Expr { Or (Gr $1 $3) (Eq $1 $3) }
        | '(' LExpr')' { $2 } 

{
data Argument = NamedArg String 
              | NumericArg Integer
              deriving Show

data TypeDecl = TypeInteger 
              | TypeVoid  
              | ComplexType [TypeDecl]
              deriving Show

data LExpr = Or LExpr LExpr
           | And LExpr LExpr
           | Not LExpr
           | Eq Expr Expr
           | Gr Expr Expr
           | Less Expr Expr
           | LExpr LExpr
           deriving Show

data Expr = Plus Expr Expr 
          | Sub Expr Expr 
          | Mult Expr Expr 
          | Div Expr Expr 
          | Var String 
          | Num Integer 
          | Expr Expr
          | IfClause LExpr Expr Expr
          | Lambda [Argument] TypeDecl Expr
          | FunctionCall Expr [Expr]
          deriving Show



data ParsingState a = OK a | Failed String
                    deriving Show

instance Monad ParsingState where
    return a = OK a
    ma >>= f = case ma of 
                    Failed s -> Failed s
                    OK a -> f a

data FunctionClause = FunctionClause String [Argument] Expr
                    deriving Show
data Function = Function String TypeDecl [FunctionClause]
              deriving Show
type Language = [Function]
parseError :: [TokenPos] -> ParsingState a
parseError [] = Failed "ParseError: no more tokens\n"
parseError (TokenPos _ (AlexPn _ line col) :_) = Failed $ "Parse error: " ++ "line " ++ (show line) ++ "; col " ++ (show col)
}
