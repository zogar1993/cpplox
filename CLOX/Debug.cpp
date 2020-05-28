#include "Debug.h"
#include "common.h"
#include "object.h"
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

int byteInstruction(const char* name, const Chunk* chunk, int offset) {
    uint8_t slot = chunk->getCode(offset + 1);
    printf("%-16s %4d\n", name, slot);
    return offset + 2;
}

static int jumpInstruction(const char* name, int sign, const Chunk* chunk,
    int offset) {
    uint16_t jump = (uint16_t)(chunk->getCode(offset + 1) << 8);
    jump |= chunk->getCode(offset + 2);
    printf("%-16s %4d -> %d\n", name, offset, offset + 3 + sign * jump);
    return offset + 3;
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
    case OP_GET_LOCAL:
        return byteInstruction("OP_GET_LOCAL", chunk, offset);
    case OP_SET_LOCAL:
        return byteInstruction("OP_SET_LOCAL", chunk, offset);
    case OP_GET_GLOBAL:
        return constantInstruction("OP_GET_GLOBAL", chunk, offset);
    case OP_DEFINE_GLOBAL:
        return constantInstruction("OP_DEFINE_GLOBAL", chunk, offset);
    case OP_SET_GLOBAL:
        return constantInstruction("OP_SET_GLOBAL", chunk, offset);
    case OP_GET_UPVALUE:
        return byteInstruction("OP_GET_UPVALUE", chunk, offset);
    case OP_SET_UPVALUE:
        return byteInstruction("OP_SET_UPVALUE", chunk, offset);
    case OP_GET_PROPERTY:
        return constantInstruction("OP_GET_PROPERTY", chunk, offset);
    case OP_SET_PROPERTY:
        return constantInstruction("OP_SET_PROPERTY", chunk, offset);
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
    case OP_JUMP:
        return jumpInstruction("OP_JUMP", 1, chunk, offset);
    case OP_JUMP_IF_FALSE:
        return jumpInstruction("OP_JUMP_IF_FALSE", 1, chunk, offset);
    case OP_LOOP:
        return jumpInstruction("OP_LOOP", -1, chunk, offset);
    case OP_CALL:
        return byteInstruction("OP_CALL", chunk, offset);
    case OP_CLOSURE: {
        offset++;
        uint8_t constant = chunk->getCode(offset++);
        printf("%-16s %4d ", "OP_CLOSURE", constant);
        printValue(chunk->getConstant(constant));
        printf("\n");

        ObjFunction* function = AS_FUNCTION(chunk->getConstant(constant));
        for (int j = 0; j < function->upvalueCount; j++) {
            int isLocal = chunk->getCode(offset++);
            int index = chunk->getCode(offset++);
            printf("%04d      |                     %s %d\n",
                offset - 2, isLocal ? "local" : "upvalue", index);
        }

        return offset;
    }
    case OP_CLOSE_UPVALUE:
        return simpleInstruction("OP_CLOSE_UPVALUE", offset);
    case OP_RETURN:
        return simpleInstruction("OP_RETURN", offset);
    case OP_CLASS:
        return constantInstruction("OP_CLASS", chunk, offset);
    default:
        printf("Unknown opcode %d\n", instruction);
        return offset + 1;
    }
}