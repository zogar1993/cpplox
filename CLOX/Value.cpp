#include "Value.h"
#include "Memory.h"
#include <string.h>
#include "object.h" 

ValueArray::ValueArray()
{
}

void ValueArray::write(Value value)
{
    if (capacity < count + 1) {
        int oldCapacity = capacity;
        capacity = GROW_CAPACITY(oldCapacity);
        values = GROW_ARRAY(Value, values, oldCapacity, capacity);
    }

    values[count] = value;
    count++;
}

void ValueArray::free()
{
    FREE_ARRAY(Value, values, capacity);
    count = 0;
    capacity = 0;
    values = NULL;
}

bool valuesEqual(Value a, Value b) {
    if (a.type != b.type) return false;

    switch (a.type) {
    case VAL_BOOL:   return AS_BOOL(a) == AS_BOOL(b);
    case VAL_NIL:    return true;
    case VAL_NUMBER: return AS_NUMBER(a) == AS_NUMBER(b);
    case VAL_OBJ:    return AS_OBJ(a) == AS_OBJ(b);
    }
}

void printValue(const Value value)
{
    switch (value.type) {
    case VAL_BOOL:   printf(AS_BOOL(value) ? "true" : "false"); break;
    case VAL_NIL:    printf("nil"); break;
    case VAL_NUMBER: printf("%g", AS_NUMBER(value)); break;
    case VAL_OBJ:    printObject(value); break;
    }
}