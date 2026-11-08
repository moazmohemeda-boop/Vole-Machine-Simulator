#ifndef OPCODE_H
#define OPCODE_H

enum class Opcode {
    LOAD_MEM,
    LOAD_VAL,
    STORE,
    MOVE,
    ADD_INT,
    ADD_FLOAT,
    OR,
    AND,
    XOR,
    ROTATE,
    JUMP_EQ,
    HALT,
    INVALID
};

#endif // OPCODE_H
