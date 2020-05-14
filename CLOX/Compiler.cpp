#include "Compiler.h"
#include "common.h"
#include "debug.h"

bool Compiler::compile(const char* source, Chunk* chunk)
{
    Compiler::scanner = Scanner(source);
    compilingChunk = chunk;
    advance();
    expression();
    consume(TOKEN_EOF, "Expect end of expression.");
    endCompiler();
    return !parser.hadError;
}

void Compiler::advance()
{
    parser.previous = parser.current;

    for (;;) {
        parser.current = scanner.scanToken();
        if (parser.current.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.current.start);
    }
}

void Compiler::emitByte(uint8_t byte) {
    currentChunk()->write(byte, parser.previous.line);
}

void Compiler::endCompiler() {
    emitReturn();

#ifdef DEBUG_PRINT_CODE                      
    if (!parser.hadError) {
        disassembleChunk(currentChunk(), "code");
    }
#endif 
}

void Compiler::emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}

void Compiler::emitReturn() {
    emitByte(OP_RETURN);
}

Chunk* Compiler::currentChunk() {
    return compilingChunk;
}

void Compiler::expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}

void Compiler::number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}

void Compiler::emitConstant(Value value) {
    emitBytes(OP_CONSTANT, makeConstant(value));
}

uint8_t Compiler::makeConstant(Value value) {
    int constant = currentChunk()->addConstant(value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }

    return (uint8_t)constant;
}

void Compiler::unary() {
    TokenType operatorType = parser.previous.type;

    // Compile the operand.                        
    expression();

    // Emit the operator instruction.              
    switch (operatorType) {
    case TOKEN_BANG: emitByte(OP_NOT); break;
    case TOKEN_MINUS: emitByte(OP_NEGATE); break;
    default:
        return; // Unreachable.                    
    }
}

void Compiler::binary() {
    // Remember the operator.                                
    TokenType operatorType = parser.previous.type;

    // Compile the right operand.                            
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    // Emit the operator instruction.                        
    switch (operatorType) {
    case TOKEN_BANG_EQUAL:    emitBytes(OP_EQUAL, OP_NOT); break;
    case TOKEN_EQUAL_EQUAL:   emitByte(OP_EQUAL); break;
    case TOKEN_GREATER:       emitByte(OP_GREATER); break;
    case TOKEN_GREATER_EQUAL: emitBytes(OP_LESS, OP_NOT); break;
    case TOKEN_LESS:          emitByte(OP_LESS); break;
    case TOKEN_LESS_EQUAL:    emitBytes(OP_GREATER, OP_NOT); break;
    case TOKEN_PLUS:          emitByte(OP_ADD); break;
    case TOKEN_MINUS:         emitByte(OP_SUBTRACT); break;
    case TOKEN_STAR:          emitByte(OP_MULTIPLY); break;
    case TOKEN_SLASH:         emitByte(OP_DIVIDE); break;
    default:
        return; // Unreachable.                              
    }
}

void Compiler::grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}

void Compiler::parsePrecedence(Precedence precedence) {
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expect expression.");
        return;
    }
    
    (*this.*prefixRule)();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        (*this.*infixRule)();
    }
}
Compiler::ParseRule* Compiler::getRule(TokenType type) {
    return &rules[type];
}

//static void expression();
//static ParseRule* getRule(TokenType type);
//static void parsePrecedence(Precedence precedence);


void Compiler::consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance();
        return;
    }

    errorAtCurrent(message);
}

void Compiler::errorAtCurrent(const char* message) {
    errorAt(&parser.current, message);
}

void Compiler::error(const char* message) {
    errorAt(&parser.previous, message);
}

void Compiler::errorAt(Token* token, const char* message) {
    if (parser.panicMode) return;
    parser.panicMode = true;

    fprintf(stderr, "[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        fprintf(stderr, " at end");
    }
    else if (token->type == TOKEN_ERROR) {
        // Nothing.                                                
    }
    else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = true;
}

void Compiler::literal() {
    switch (parser.previous.type) {
    case TOKEN_FALSE: emitByte(OP_FALSE); break;
    case TOKEN_NIL: emitByte(OP_NIL); break;
    case TOKEN_TRUE: emitByte(OP_TRUE); break;
    default:
        return; // Unreachable.                   
    }
}