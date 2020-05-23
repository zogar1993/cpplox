#pragma once

#include "Scanner.h"

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

typedef struct {
	Token name;
	int depth;
} Local;

typedef enum {
	TYPE_FUNCTION,
	TYPE_SCRIPT
} FunctionType;
