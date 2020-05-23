#include "InstructionStack.h"

InstructionStack::InstructionStack()
{
}

InstructionStack::InstructionStack(FunctionType type, InstructionStack* current) {
	function = NULL;
	InstructionStack::type = type;
	function = newFunction();

	Local* local = &current->locals[current->localCount++];
	local->depth = 0;
	local->name.start = "";
	local->name.length = 0;
}
