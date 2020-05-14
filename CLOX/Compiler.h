#pragma once

#include "Chunk.h"
#include "Scanner.h"
#include "common.h"

typedef struct {
	Token current;
	Token previous;
	bool hadError = false;
	bool panicMode = false;
} Parser;

typedef enum {
	PREC_NONE,
	PREC_ASSIGNMENT,  // =        
	PREC_OR,          // or       
	PREC_AND,         // and      
	PREC_EQUALITY,    // == !=    
	PREC_COMPARISON,  // < > <= >=
	PREC_TERM,        // + -      
	PREC_FACTOR,      // * /      
	PREC_UNARY,       // ! -      
	PREC_CALL,        // . ()     
	PREC_PRIMARY
} Precedence;

class Compiler
{
public:
	bool compile(const char* source, Chunk* chunk);
private:
	void advance();
	void emitByte(uint8_t byte);
	void endCompiler();
	void emitBytes(uint8_t byte1, uint8_t byte2);
	void emitReturn();
	Chunk* currentChunk();
	void expression();
	void number();
	void emitConstant(Value value);
	uint8_t makeConstant(Value value);
	void literal();
	void unary();
	void binary();
	void grouping();
	void consume(TokenType type, const char* message);
	void errorAtCurrent(const char* message);
	void error(const char* message);
	void errorAt(Token* token, const char* message);
	Parser parser;
	Scanner scanner;
	Chunk* compilingChunk;
	typedef void (Compiler::* ParseFn)();
	struct ParseRule {
		ParseFn prefix;
		ParseFn infix;
		Precedence precedence;
	};
	void parsePrecedence(Precedence precedence);
	ParseRule* getRule(TokenType type);
	ParseRule rules[50] = {
	  { &Compiler::grouping, NULL,				PREC_NONE },       // TOKEN_LEFT_PAREN      
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_RIGHT_PAREN     
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_LEFT_BRACE
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_RIGHT_BRACE     
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_COMMA           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_DOT             
	  { &Compiler::unary,    &Compiler::binary, PREC_TERM },       // TOKEN_MINUS           
	  { NULL,				 &Compiler::binary, PREC_TERM },       // TOKEN_PLUS            
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_SEMICOLON       
	  { NULL,				 &Compiler::binary, PREC_FACTOR },     // TOKEN_SLASH           
	  { NULL,				 &Compiler::binary, PREC_FACTOR },     // TOKEN_STAR            
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_BANG            
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_BANG_EQUAL      
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_EQUAL           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_EQUAL_EQUAL     
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_GREATER         
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_GREATER_EQUAL   
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_LESS            
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_LESS_EQUAL      
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_IDENTIFIER      
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_STRING          
	  { &Compiler::number,	 NULL,				PREC_NONE },       // TOKEN_NUMBER          
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_AND             
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_CLASS           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_ELSE            
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_FALSE           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_FOR             
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_FUN             
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_IF              
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_NIL             
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_OR              
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_PRINT           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_RETURN          
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_SUPER           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_THIS            
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_TRUE            
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_VAR             
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_WHILE           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_ERROR           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_EOF

	  { &Compiler::unary,    NULL,    PREC_NONE },       // TOKEN_BANG  

	  { NULL,     &Compiler::binary,  PREC_EQUALITY },   // TOKEN_BANG_EQUAL
	  { NULL,     &Compiler::binary,  PREC_EQUALITY },   // TOKEN_EQUAL_EQUAL  
	  { NULL,     &Compiler::binary,  PREC_COMPARISON }, // TOKEN_GREATER      
	  { NULL,     &Compiler::binary,  PREC_COMPARISON }, // TOKEN_GREATER_EQUAL
	  { NULL,     &Compiler::binary,  PREC_COMPARISON }, // TOKEN_LESS         
	  { NULL,     &Compiler::binary,  PREC_COMPARISON }, // TOKEN_LESS_EQUAL 

	  { &Compiler::literal,  NULL,    PREC_NONE },       // TOKEN_FALSE
	  { &Compiler::literal,  NULL,    PREC_NONE },       // TOKEN_TRUE
	  { &Compiler::literal,  NULL,    PREC_NONE },       // TOKEN_NIL
	};
};