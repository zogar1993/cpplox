#pragma once

#include "common.h" 
#include "value.h"  

typedef struct {
    ObjString* key;
    Value value;
} Entry;

class Table
{
public:
    Table();
    void init();
    void free();
    Entry* findEntry(Entry* entries, int capacity, ObjString* key);
    bool set(ObjString* key, Value value);
    bool get(ObjString* key, Value* value);
    ObjString* findString(const char* chars, int length, uint32_t hash);
    bool remove(ObjString* key);
    Entry* entries;
    int capacity;
    void mark();
    void removeWhite();
private:
    void adjustCapacity(int capacity);
    int count;
};

void tableAddAll(Table* from, Table* to);
