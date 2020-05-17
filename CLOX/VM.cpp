#include "VM.h"
#include <stdio.h> 
#include "debug.h"
#include "compiler.h"
#include <stdarg.h>
#include <string.h>
#include "object.h"
#include "memory.h"

InterpretResult VM::run() {

#define READ_BYTE() (*ip++)
#define READ_CONSTANT() (chunk->getConstant(READ_BYTE()))
#define BINARY_OP(valueType, op) \
    do { \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) { \
            runtimeError("Operands must be numbers."); \
            return INTERPRET_RUNTIME_ERROR; \
        } \
        double b = AS_NUMBER(pop()); \
        double a = AS_NUMBER(pop()); \
        push(valueType(b op a)); \
    } while (false)


    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value* slot = stack; slot < stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(chunk, (int)(ip - chunk->getCode()));
        //TODO entender bien como funciona. Since disassembleInstruction()
        //takes an integer byte offset and we store the current instruction reference as a direct pointer,
        //we first do a little pointer math to convert ip back to a relative offset from the beginning of the bytecode.
        //Then we disassemble the instruction that begins at that byte.
#endif 
        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_NIL:      push(NIL_VAL); break;
            case OP_TRUE:     push(BOOL_VAL(true)); break;
            case OP_FALSE:    push(BOOL_VAL(false)); break;
            case OP_EQUAL: {
                Value b = pop();
                Value a = pop();
                push(BOOL_VAL(valuesEqual(a, b)));
                break;
            }
            case OP_GREATER:  BINARY_OP(BOOL_VAL, > ); break;
            case OP_LESS:     BINARY_OP(BOOL_VAL, < ); break;
            case OP_ADD: {
                if (IS_STRING(peek(0)) && IS_STRING(peek(1))) {
                    concatenate();
                }
                else if (IS_NUMBER(peek(0)) && IS_NUMBER(peek(1))) {
                    double b = AS_NUMBER(pop());
                    double a = AS_NUMBER(pop());
                    push(NUMBER_VAL(a + b));
                }
                else {
                    runtimeError("Operands must be two numbers or two strings.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                break;
            }
            case OP_SUBTRACT: BINARY_OP(NUMBER_VAL, -); break;
            case OP_MULTIPLY: BINARY_OP(NUMBER_VAL, *); break;
            case OP_DIVIDE:   BINARY_OP(NUMBER_VAL, / ); break;
            case OP_NOT:
                push(BOOL_VAL(isFalsey(pop())));
                break;
            case OP_NEGATE:
                if (!IS_NUMBER(peek(0))) {
                    runtimeError("Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }

                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            case OP_RETURN: {
                printValue(pop());
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}

void VM::resetStack() {
    stackTop = stack;
}

void VM::runtimeError(const char* format, ...) {
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
    fputs("\n", stderr);

    size_t instruction = ip - chunk->getCode() - 1;
    int line = chunk->getLine(instruction);
    fprintf(stderr, "[line %d] in script\n", line);

    resetStack();
}

VM::VM()
{
    init();
}

void VM::init()
{
    resetStack();
    objects = NULL;
}

void VM::free()
{
    freeObjects();
}

InterpretResult VM::interpret(const char* source)
{
    Chunk chunk;

    if (!compiler.compile(source, &chunk)) {
        chunk.free();
        return INTERPRET_COMPILE_ERROR;
    }

    VM::chunk = &chunk;
    ip = chunk.getCode();

    InterpretResult result = run();

    chunk.free();
    return result;
}

void VM::push(Value value)
{
    *stackTop = value;
    stackTop++;
}

Value VM::pop()
{
    stackTop--;
    return *stackTop;
}

Value VM::peek(int distance) {
    return stackTop[-1 - distance];
}

bool VM::isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

void VM::concatenate() {
    ObjString* b = AS_STRING(pop());
    ObjString* a = AS_STRING(pop());

    int length = a->length + b->length;
    char* chars = ALLOCATE(char, length + 1);
    memcpy(chars, a->chars, a->length);
    memcpy(chars + a->length, b->chars, b->length);
    chars[length] = '\0';

    ObjString* result = takeString(chars, length);
    push(OBJ_VAL(result));
}

static VM _vm = VM();

VM* vm()
{
    return &_vm;
}
