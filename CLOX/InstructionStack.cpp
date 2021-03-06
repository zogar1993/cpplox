#include "InstructionStack.h"

InstructionStack::InstructionStack(FunctionType type, InstructionStack* current, Parser* parser) {
	enclosing = current;
	function = NULL;
	InstructionStack::type = type;
	function = newFunction();

	if (type != TYPE_SCRIPT) {
		function->name = copyString(parser->previous.start, parser->previous.length);
	}

	Local* local = &locals[localCount++];
	local->depth = 0;
	local->name.start = "";
	local->isCaptured = false;
	local->name.length = 0;

	if (type != TYPE_FUNCTION) {
		local->name.start = "this";
		local->name.length = 4;
	} else {
		local->name.start = "";
		local->name.length = 0;
	}
}
