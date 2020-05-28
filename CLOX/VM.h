#pragma once

#include "common.h"
#include "chunk.h"
#include "value.h"
#include "compiler.h"
#include "table.h"
#include "object.h"

#define FRAMES_MAX 64                       
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

typedef struct {
	ObjClosure* closure;
	uint8_t* ip;
	Value* slots;
} CallFrame;

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
	void defineMethod(ObjString* name);
	bool callValue(Value callee, int argCount);
	void closeUpvalues(Value* last);
	bool call(ObjClosure* closure, int argCount);
	void defineNative(const char* name, NativeFn function);
	ObjUpvalue* captureUpvalue(Value* local);
	Obj* objects;
	Table strings = Table();

	CallFrame frames[FRAMES_MAX];
	int frameCount;

	int grayCount;
	int grayCapacity;
	Obj** grayStack;

	size_t bytesAllocated;
	size_t nextGC;

	Value stack[STACK_MAX];
	Value* stackTop;
	Table globals = Table();
	ObjUpvalue* openUpvalues;
	Compiler compiler = Compiler();
private:
	const Chunk* chunk;
	uint8_t* ip;
	InterpretResult run();
	void resetStack();
	bool bindMethod(ObjClass* klass, ObjString* name);
	void runtimeError(const char* format, ...);
};

VM* vm();
