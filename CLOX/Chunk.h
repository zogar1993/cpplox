#pragma once

#include "common.h"
#include "Value.h"

typedef enum {
    OP_CONSTANT,
    OP_POP,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_GET_GLOBAL,
    OP_DEFINE_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_UPVALUE,
    OP_SET_UPVALUE,
    OP_GET_PROPERTY,
    OP_SET_PROPERTY,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_NOT,
    OP_NEGATE,
    OP_PRINT,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_LOOP,
    OP_CALL,
    OP_CLOSURE,
    OP_CLOSE_UPVALUE,
    OP_RETURN,
    OP_CLASS,
    OP_METHOD
} OpCode;

class Chunk {
public:
    Chunk();
    void write(uint8_t byte, int line);
    int addConstant(Value value);
    void free();
    int getCount() const { return count; }
    uint8_t* getCode() const { return code; }
    uint8_t getCode(int offset) const { return code[offset]; }
    void setCode(int offset, uint8_t op) { code[offset] = op; }
    Value getConstant(int index) const { return constants.getValue(index); }
    int getLine(int index) const { return lines[index]; }
    int count = 0;

    ValueArray constants = ValueArray();
private:
    int capacity = 0;
    uint8_t* code = NULL;
    int* lines = NULL;
};