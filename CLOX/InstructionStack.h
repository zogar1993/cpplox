#pragma once

#include "common.h"
#include "object.h"
#include "CompilerTypes.h"

class InstructionStack
{
public:
	InstructionStack();
	InstructionStack(FunctionType type, InstructionStack* current, Parser* parser);
	ObjFunction* function = NULL;
	FunctionType type;

	InstructionStack* enclosing;
	Local locals[UINT8_COUNT];
	int localCount = 0;
	int scopeDepth = 0;
};

