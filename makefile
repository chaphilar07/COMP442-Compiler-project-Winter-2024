
CC=gcc

CFLAGS=-Wall -Werror
LEXTEST=testdriver
TARGET=Lexdriver
PARSETEST=parsedriver

ASTTEST=ASTDRIVER
SYMBOLDRIVER=SYMBOLDRIVER

CODEGEN = CODEGENDRIVER


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


testastgen:
	$(CC) $(CFLAGS) src/lexer/lexer.c src/utils/utils.c src/parser/parser.c src/parser/parsedriver.c src/parser/AST/AST_gen.c -o $(ASTTEST)
	./$(ASTTEST)
	rm $(ASTTEST)


symboltabletest:
	$(CC) $(CFLAGS)  src/SemanticAnalyzer/SemanticAnalyzer.c src/SemanticAnalyzer/SemanticError.c src/lexer/lexer.c src/utils/utils.c src/parser/parser.c src/parser/AST/AST_gen.c src/SemanticAnalyzer/SymbolTable.c src/SemanticAnalyzer/SemanticAnalyzerDriver.c -o $(SYMBOLDRIVER)
	./$(SYMBOLDRIVER)
	rm $(SYMBOLDRIVER)

symboldriver:
	$(CC) $(CFLAGS)  src/SemanticAnalyzer/SemanticAnalyzer.c src/SemanticAnalyzer/SemanticError.c src/lexer/lexer.c src/utils/utils.c src/parser/parser.c src/parser/AST/AST_gen.c src/SemanticAnalyzer/SymbolTable.c src/SemanticAnalyzer/SemanticAnalyzerDriver.c -o $(SYMBOLDRIVER)


codegentest:
	$(CC) $(CFLAGS) src/SemanticAnalyzer/SemanticAnalyzer.c src/SemanticAnalyzer/SemanticError.c src/lexer/lexer.c src/utils/utils.c src/parser/parser.c src/parser/AST/AST_gen.c src/SemanticAnalyzer/SymbolTable.c src/SemanticAnalyzer/SemanticAnalyzerDriver.c -o $(CODEGEN)
	./$(CODEGEN)
	rm $(CODEGEN)

codegendriver:
	$(CC) $(CFLAGS) src/SemanticAnalyzer/SemanticAnalyzer.c src/SemanticAnalyzer/SemanticError.c src/lexer/lexer.c src/utils/utils.c src/parser/parser.c src/parser/AST/AST_gen.c src/SemanticAnalyzer/SymbolTable.c src/SemanticAnalyzer/SemanticAnalyzerDriver.c -o $(CODEGEN)

