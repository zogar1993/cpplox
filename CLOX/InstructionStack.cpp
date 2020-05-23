#include "InstructionStack.h"

InstructionStack::InstructionStack()
{
}

InstructionStack::InstructionStack(FunctionType type, InstructionStack* current, Parser* parser) {
	enclosing = current;
	function = NULL;
	InstructionStack::type = type;
	function = newFunction();
	//current = compiler? or is that done in caller?

	if (type != TYPE_SCRIPT) {
		current->function->name = copyString(parser->previous.start, parser->previous.length);
	}

	Local* local = &current->locals[current->localCount++];
	local->depth = 0;
	local->name.start = "";
	local->name.length = 0;
}
