#pragma once

#include "common.h"

typedef struct sObj Obj;
typedef struct sObjString ObjString;

typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
    VAL_OBJ
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
        Obj* obj;
    } as;
} Value;

#define IS_BOOL(value)    ((value).type == VAL_BOOL)  
#define IS_NIL(value)     ((value).type == VAL_NIL)   
#define IS_NUMBER(value)  ((value).type == VAL_NUMBER)
#define IS_OBJ(value)     ((value).type == VAL_OBJ)

#define AS_BOOL(value)    ((value).as.boolean)                       
#define AS_NUMBER(value)  ((value).as.number)
#define AS_OBJ(value)     ((value).as.obj)

#define BOOL_VAL(value)   { VAL_BOOL, { .boolean = value != 0.0 } }
#define NIL_VAL           { VAL_NIL, { .number = 0 } }
#define NUMBER_VAL(value) { VAL_NUMBER, { .number = value } }
#define OBJ_VAL(object)   { VAL_OBJ, { .obj = (Obj*)object } }

class ValueArray {
public:
    ValueArray();
    void write(Value byte);
    void free();
    const int getCount() const { return count; }
    const Value getValue(const int offset) const { return values[offset]; }
private:
    int count = 0;
    int capacity = 0;
    Value* values = NULL;
};

bool valuesEqual(Value a, Value b);
void printValue(const Value value);