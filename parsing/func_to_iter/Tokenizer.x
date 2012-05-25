{
module Tokenizer where
}


%wrapper "posn"

$digit = 0-9
$alpha = [a-zA-Z]

toknes :-
    $white+ ;
    \(      { \p s -> TokenPos TokenOP p }
    \)      { \p s -> TokenPos TokenCP p }
    \{      { \p s -> TokenPos TokenOB p }
    \}      { \p s -> TokenPos TokenCB p }
    \+      { \p s -> TokenPos TokenPlus p }
    \-      { \p s -> TokenPos TokenMinus p }
    \*      { \p s -> TokenPos TokenStar p }
    \/      { \p s -> TokenPos TokenDiv p }
    >       { \p s -> TokenPos TokenGr p }
    >=      { \p s -> TokenPos TokenGrEq p }
    \<      { \p s -> TokenPos TokenLess p }
    \<=     { \p s -> TokenPos TokenLessEq p }
    !=      { \p s -> TokenPos TokenNotEq p }
    ==      { \p s -> TokenPos TokenEq p }
    \|\|    { \p s -> TokenPos TokenOr p }
    &&      { \p s -> TokenPos TokenAnd p }
    !       { \p s -> TokenPos TokenNot p }
    if      { \p s -> TokenPos TokenIf p }
    then    { \p s -> TokenPos TokenThen p }
    else    { \p s -> TokenPos TokenElse p }
    fi      { \p s -> TokenPos TokenFi p }
    true    { \p s -> TokenPos TokenTrue p }
    false   { \p s -> TokenPos TokenFalse p }
    [a-z] [$alpha $digit \_]*  { \p s -> TokenPos (TokenId s) p }
    Integer { \p s -> TokenPos TokenTypeInteger p }
    Void    { \p s -> TokenPos TokenTypeVoid p }
    $digit+ { \p s -> TokenPos (TokenInteger (read s)) p }
    \-\>    { \p s -> TokenPos TokenArrow p }
    ::      { \p s -> TokenPos TokenOfType p }
    \\      { \p s -> TokenPos TokenLambda p }
    =       { \p s -> TokenPos TokenAssign p }
    _       { \p s -> TokenPos TokenAny p }
    \,      { \p s -> TokenPos TokenComma p } 
    \.      { \p s -> TokenPos TokenPeriod p }
    \;      { \p s -> TokenPos TokenSemicolon p }
    \|      { \p s -> TokenPos TokenVert p }
{

data TokenPos = TokenPos Token AlexPosn
              deriving Show
data Token = TokenOP
           | TokenComma
           | TokenCP
           | TokenOB
           | TokenCB
           | TokenSemicolon
           | TokenPlus
           | TokenMinus
           | TokenStar
           | TokenVert
           | TokenDiv
           | TokenGr
           | TokenGrEq
           | TokenLess
           | TokenLessEq
           | TokenNotEq
           | TokenEq
           | TokenOr
           | TokenAnd
           | TokenNot
           | TokenId String
           | TokenInteger Integer
           | TokenArrow
           | TokenOfType
           | TokenIf
           | TokenThen
           | TokenElse
           | TokenFi
           | TokenTrue
           | TokenFalse
           | TokenLambda
           | TokenAssign
           | TokenAny
           | TokenTypeInteger
           | TokenTypeVoid
           | TokenPeriod
           deriving Show
}
