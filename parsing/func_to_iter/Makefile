PACKAGES=-package mtl
GHC_FLAGS=-fno-warn-missing-fields -fno-warn-overlapping-patterns

all : trans

trans: Tokenizer.hs Main.hs Driver.hs Parser.hs
	ghc ${GHC_FLAGS} ${PACKAGES} -o trans Tokenizer.hs Parser.hs Driver.hs Main.hs

Tokenizer.hs : Tokenizer.x
	alex Tokenizer.x

Parser.hs : Parser.y
	happy Parser.y

ololo.o : test.func trans
	./trans test.func ololo && g++ -c ololo.cpp

ololo.h : ololo.o


main : ololo.o ololo.h main.cpp
	g++ -o main main.cpp ololo.o

clean: 
	rm *.hi *.o ololo.cpp Tokenizer.hs Parser.hs trans main
