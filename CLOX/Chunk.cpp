#include "chunk.h"
#include "memory.h"

Chunk::Chunk()
{

}

void Chunk::write(uint8_t byte, int line)
{
    if (capacity < count + 1) {
        int oldCapacity = capacity;
        capacity = GROW_CAPACITY(oldCapacity);
        code = GROW_ARRAY(uint8_t, code, oldCapacity, capacity);
        lines = GROW_ARRAY(int, lines, oldCapacity, capacity);
    }

    code[count] = byte;
    lines[count] = line;
    count++;
}

int Chunk::addConstant(Value value)
{
    constants.write(value);
    return constants.getCount() - 1;
}

void Chunk::free()
{
    FREE_ARRAY(uint8_t, code, capacity);
    FREE_ARRAY(int, lines, capacity);
    count = 0;
    capacity = 0;
    lines = NULL;
    code = NULL;
    constants = ValueArray();
}
