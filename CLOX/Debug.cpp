#include "Debug.h"
#include "common.h"
#include "value.h"
#include <iostream>

static int simpleInstruction(const char* name, int offset) {
    printf("%s\n", name);
    return offset + 1;
}

static int constantInstruction(const char* name, const Chunk* chunk, int offset) {
    uint8_t constant_index = chunk->getCode(offset + 1);
    printf("%-16s %4d '", name, constant_index);
    printValue(chunk->getConstant(constant_index));
    printf("'\n");
    return offset + 2;
}

void disassembleChunk(const Chunk* chunk, const char* name){
    std::cout << "== " << name << "==\n";

    for (int offset = 0; offset < chunk->getCount();) {
        offset = disassembleInstruction(chunk, offset);
    }  
}

int disassembleInstruction(const Chunk* chunk, int offset)
{
    printf("%04d ", offset);

    if (offset > 0 && chunk->getLine(offset) == chunk->getLine(offset - 1))
        printf("   | ");
    else 
        printf("%4d ", chunk->getLine(offset));

    uint8_t instruction = chunk->getCode(offset);
    switch (instruction) {
    case OP_CONSTANT:
        return constantInstruction("OP_CONSTANT", chunk, offset);
    case OP_NIL:
        return simpleInstruction("OP_NIL", offset);
    case OP_TRUE:
        return simpleInstruction("OP_TRUE", offset);
    case OP_FALSE:
        return simpleInstruction("OP_FALSE", offset);
    case OP_POP:
        return simpleInstruction("OP_POP", offset);
    case OP_EQUAL:
        return simpleInstruction("OP_EQUAL", offset);
    case OP_GREATER:
        return simpleInstruction("OP_GREATER", offset);
    case OP_LESS:
        return simpleInstruction("OP_LESS", offset);
    case OP_ADD:
        return simpleInstruction("OP_ADD", offset);
    case OP_SUBTRACT:
        return simpleInstruction("OP_SUBTRACT", offset);
    case OP_MULTIPLY:
        return simpleInstruction("OP_MULTIPLY", offset);
    case OP_DIVIDE:
        return simpleInstruction("OP_DIVIDE", offset);
    case OP_NOT:
        return simpleInstruction("OP_NOT", offset);
    case OP_NEGATE:
        return simpleInstruction("OP_NEGATE", offset);
    case OP_PRINT:
        return simpleInstruction("OP_PRINT", offset);
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}
