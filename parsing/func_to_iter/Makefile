PACKAGES=-package mtl
GHC_FLAGS=-fno-warn-missing-fields -fno-warn-overlapping-patterns

all : trans

trans: Tokenizer.hs Main.hs Driver.hs Parser.hs
	ghc ${GHC_FLAGS} ${PACKAGES} -o trans Tokenizer.hs Parser.hs Driver.hs Main.hs

Tokenizer.hs : Tokenizer.x
	alex Tokenizer.x

Parser.hs : Parser.y
	happy Parser.y

main.cpp : test.func trans
	./trans test.func > main.cpp && gcc -c main.cpp


clear: 
	rm *.hi *.o *.cpp Tokenizer.hs Parser.hs trans
