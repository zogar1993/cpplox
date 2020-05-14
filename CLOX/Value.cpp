#include "Value.h"
#include "Memory.h"

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
    }
}
