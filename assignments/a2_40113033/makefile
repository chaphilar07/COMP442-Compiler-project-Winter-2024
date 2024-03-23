
CC=gcc

CFLAGS=-Wall -Werror
LEXTEST=testdriver
TARGET=Lexdriver
PARSETEST=parsedriver

all: $(TARGET)

$(TARGET): Scanner.c
	$(CC) $(CFLAGS) Scanner.c -o $(TARGET)


run:
	./$(TARGET)

lexdriver:
	$(CC) $(CFLAGS)  src/lexer/Lexer.c src/utils/Utils.c src/lexer/Lexdriver.c -o $(TARGET)


testlexer:
	$(CC) $(CFLAGS)  src/lexer/Lexer.c src/utils/Utils.c src/lexer/testdriver.c -o $(LEXTEST)
	./$(LEXTEST)
	rm $(LEXTEST)


parser:
	$(CC) $(CLAGS) -g src/lexer/lexer.c src/utils/utils.c src/parser/parser.c src/parser/parsedriver.c -o $(PARSETEST)

testparser:
	$(CC) $(CFLAGS) src/lexer/lexer.c src/utils/utils.c src/parser/parser.c src/parser/parsedriver.c -o $(PARSETEST)
	./$(PARSETEST)
	rm $(PARSETEST)




