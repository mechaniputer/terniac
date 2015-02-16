terniac
=======

Terniac is a simulation of an extremely basic ternary computer. It is Turing-complete, but not necessarily efficient.

Terniac's machine language currently uses only the first three trits at each memory location.
In theory, it should be easy to extend the data width to 9 trits without changing any existing instructions.

Terniac has a somewhat limited instruction set consisting of ADD, LD, STO, INV, INC, JP, SKP, HALT and NOOP.
Most of these instructions act on one of the three general-purpose registers, and JP and SKP also act on the program counter.

ADD, LD and STO use a memory-deferred (absolute) addressing mode. 

For example, if you want to load the value 3 into reg A, don't do this:
LD A
DATA 3

Instead, you must store 3 somewhere, and provide that address to the LD instruction. 
Like this:
(...)
LD A
ADDR THRE
(...)
LABEL THRE
DATA 3
(...)

This might seem inconvenient at first, but it allows a sort of makeshift naming of variables without supporting multiple addressing modes.
As mentioned above, this applies to: {ADD LD STO}{A B C}

However, JP will set the program counter to whatever value is contained in the immediate next memory location after the JP.

INV and INC act only on one of the three registers.

NOOP simply goes to the next instruction with no effect on anything other than the program counter. It can be used to reserve space for data.

HALT causes the machine to immediately stop execution.
