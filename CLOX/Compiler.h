#pragma once

#include "Chunk.h"
#include "common.h"
#include "object.h"
#include "CompilerTypes.h"
#include "InstructionStack.h"

class Compiler
{
public:
	ObjFunction* compile(const char* source);
	void markRoots();
private:
	Chunk* currentChunk();
	void advance();
	void emitByte(uint8_t byte);
	ObjFunction* endCompiler();
	void emitBytes(uint8_t byte1, uint8_t byte2);
	void emitReturn();
	void expression();
	void emitConstant(Value value);
	uint8_t makeConstant(Value value);
	void number(bool canAssign);
	void literal(bool canAssign);
	void string(bool canAssign);
	void classDeclaration();
	void unary(bool canAssign);
	void binary(bool canAssign);
	void grouping(bool canAssign);
	void declaration();
	void funDeclaration();
	void statement();
	void returnStatement();
	void forStatement();
	void whileStatement();
	void emitLoop(int loopStart);
	void ifStatement();
	int emitJump(uint8_t instruction);
	void patchJump(int offset);
	void block();
	void beginScope();
	void endScope();
	bool match(TokenType type);
	bool check(TokenType type);
	void printStatement();
	void expressionStatement();
	void synchronize();
	void varDeclaration();
	uint8_t parseVariable(const char* errorMessage);
	void declareVariable();
	bool identifiersEqual(Token* a, Token* b);
	void addLocal(Token name);
	uint8_t identifierConstant(Token* name);
	void defineVariable(uint8_t global);
	void markInitialized();
	void variable(bool canAssign);
	void namedVariable(Token name, bool canAssign);
	int resolveUpvalue(InstructionStack* compiler, Token* name);
	int addUpvalue(InstructionStack* compiler, uint8_t index, bool isLocal);
	int resolveLocal(InstructionStack* current, Token* name);
	void and_(bool canAssign);
	void or_(bool canAssign);
	void function(FunctionType type);
	void call(bool canAssign);
	void dot(bool canAssign);
	uint8_t argumentList();
	void consume(TokenType type, const char* message);
	void errorAtCurrent(const char* message);
	void error(const char* message);
	void errorAt(Token* token, const char* message);
	Parser parser;
	Scanner scanner;
	Chunk* compilingChunk;
	typedef void (Compiler::* ParseFn)(bool canAssign);
	struct ParseRule {
		ParseFn prefix;
		ParseFn infix;
		Precedence precedence;
	};
	void parsePrecedence(Precedence precedence);
	ParseRule* getRule(TokenType type);
	InstructionStack* current = NULL;
	ParseRule rules[40] = {
	// Single-character tokens.   
	  { &Compiler::grouping, &Compiler::call,	PREC_CALL},       // TOKEN_LEFT_PAREN      
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_RIGHT_PAREN     
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_LEFT_BRACE
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_RIGHT_BRACE     
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_COMMA           
	  { NULL,     &Compiler::dot,     PREC_CALL },       // TOKEN_DOT         
	  { &Compiler::unary,    &Compiler::binary, PREC_TERM },       // TOKEN_MINUS           
	  { NULL,				 &Compiler::binary, PREC_TERM },       // TOKEN_PLUS            
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_SEMICOLON       
	  { NULL,				 &Compiler::binary, PREC_FACTOR },     // TOKEN_SLASH           
	  { NULL,				 &Compiler::binary, PREC_FACTOR },     // TOKEN_STAR

	// One or two character tokens.
	  { &Compiler::unary,	 NULL,				PREC_NONE },       // TOKEN_BANG            
	  { NULL,				 &Compiler::binary,	PREC_EQUALITY },   // TOKEN_BANG_EQUAL      
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_EQUAL           
	  { NULL,				 &Compiler::binary,	PREC_EQUALITY },   // TOKEN_EQUAL_EQUAL     
	  { NULL,				 &Compiler::binary,	PREC_COMPARISON }, // TOKEN_GREATER         
	  { NULL,				 &Compiler::binary,	PREC_COMPARISON }, // TOKEN_GREATER_EQUAL   
	  { NULL,				 &Compiler::binary,	PREC_COMPARISON }, // TOKEN_LESS
	  { NULL,				 &Compiler::binary,	PREC_COMPARISON }, // TOKEN_LESS_EQUAL

	// Literals.
	  { &Compiler::variable, NULL,				PREC_NONE },       // TOKEN_IDENTIFIER   
	  { &Compiler::string,   NULL,				PREC_NONE },
	  { &Compiler::number,	 NULL,				PREC_NONE },       // TOKEN_NUMBER

	// Keywords.
	  { NULL,				 &Compiler::and_,   PREC_AND },        // TOKEN_AND              
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_CLASS           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_ELSE            
	  { &Compiler::literal,  NULL,				PREC_NONE },       // TOKEN_FALSE           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_FOR             
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_FUN             
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_IF              
	  { &Compiler::literal,  NULL,				PREC_NONE },       // TOKEN_NIL             
	  { NULL,				 &Compiler::or_,    PREC_OR },         // TOKEN_OR           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_PRINT           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_RETURN          
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_SUPER           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_THIS            
	  { &Compiler::literal,  NULL,				PREC_NONE },       // TOKEN_TRUE            
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_VAR             
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_WHILE

	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_ERROR           
	  { NULL,				 NULL,				PREC_NONE },       // TOKEN_EOF
	};
};