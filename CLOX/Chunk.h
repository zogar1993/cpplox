#pragma once

#include "common.h"
#include "Value.h"

typedef enum {
    OP_CONSTANT,
    OP_POP,
    OP_GET_GLOBAL,
    OP_DEFINE_GLOBAL,
    OP_SET_GLOBAL,
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
    OP_RETURN,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
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
    Value getConstant(int index) const { return constants.getValue(index); }
    int getLine(int index) const { return lines[index]; }
private:
    int count = 0;
    int capacity = 0;
    uint8_t* code = NULL;
    int* lines = NULL;
    ValueArray constants = ValueArray();
};