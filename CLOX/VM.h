#pragma once

#include "common.h"
#include "chunk.h"
#include "value.h"
#include "compiler.h"
#include "table.h"

#define STACK_MAX 256

typedef enum {
	INTERPRET_OK,
	INTERPRET_COMPILE_ERROR,
	INTERPRET_RUNTIME_ERROR
} InterpretResult;

class VM
{
public:
	VM();
	void free();
	InterpretResult interpret(const char* chunk);
	void push(Value value);
	Value pop();
	Value peek(int distance);
	bool isFalsey(Value value);
	void concatenate();
	Obj* objects;
	Table strings = Table();
private:
	const Chunk* chunk;
	uint8_t* ip;
	InterpretResult run();
	void resetStack();
	void runtimeError(const char* format, ...);
	Value stack[STACK_MAX];
	Value* stackTop;
	Compiler compiler = Compiler();
	Table globals = Table();
};

VM* vm();
