#pragma once
#include "Chunk.h"

void disassembleChunk(const Chunk* chunk, const char* name);
int disassembleInstruction(const Chunk* chunk, int offset);
void printValue(const Value value);
