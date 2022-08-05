## List of instructions
Instructions will be followed by a example and a explanation

Assume letters are registers

---

### ADD
Argument 0 - reg8

Argument 1 - reg8
#### Desc.
Adds the value stored in the registers and store the value in the A register (8-Bit add).
#### Flags
Modifys zero flag

---

### ADDW
Argument 0 - reg16

Argument 1 - reg16
#### Desc.
Adds the value stored in the registers and store the value in the A register (16-Bit add).
#### Flags
Modifys zero flag

---

### ADDI
Argument 0 - reg8

Argument 1 - imm8
#### Desc.
Adds the value of a 8-Bit register and a 8-Bit immediate and store the value in the A Register.
#### Flags
Modifys zero flag

---

### ADDIW
Argument 0 - reg16

Argument 1 - imm16
#### Desc.
Adds the value of a 16-Bit register and a 16-Bit immediate and store the value in the A Register.
#### Flags
Modifys zero flag

---

### JMP
Argument 0 - reg16

Argument 1 - N/a
#### Desc.
Jump to the location stored in the register supplied.

---

### JMPI
Argument 0 - imm16

Argument 1 - N/a
#### Desc.
Jump to the location in the 16-Bit immediate.

---

### JEQ
Argument 0 - reg16

Argument 1 - N/a
#### Desc.
Jump to the location stored in the register supplied if the zero flag is set.
### Flags 
Clears zero flag

---

### JEQI
Argument 0 - imm16

Argument 1 - N/a
#### Desc.
Jump to the location stored in the immediate if the zero flag is set.
### Flags 
Clears zero flag

---
### JNE
Argument 0 - reg16

Argument 1 - N/a
#### Desc.
Jump to the location stored in the register supplied if the zero flag is clear.

---

### JNEI
Argument 0 - imm16

Argument 1 - N/a
#### Desc.
Jump to the location stored in the immediate if the zero flag is clear.

---

### JEQI
Argument 0 - imm16

Argument 1 - N/a
#### Desc.
Jump to the location stored in the immediate supplied if the zero flag is true.

---

### JNEI
Argument 0 - imm16

Argument 1 - N/a
#### Desc.
Jump to the location stored in the immediate supplied if the zero flag is false.

---

### PUSH
Argument 0 - reg8

Argument 1 - N/a
#### Desc.
Push the value of the register onto the stack, while subtracting 1 from the SP.

---

### PUSHW
Argument 0 - reg16

Argument 1 - N/a
#### Desc.
Push the value of the register onto the stack, while subtracting 2 from the SP.

---

### POP
Argument 0 - reg8

Argument 1 - N/a
#### Desc.
Pop a value off the stack and store the value into the register, while adding 1 to the SP.

---

### POPW
Argument 0 - reg16

Argument 1 - N/a
#### Desc.
Pop a value off the stack and store the value into the register, while adding 2 to the SP.

---

### MOV
Argument 0 - reg8

Argument 1 - reg8
#### Desc.
Move the value stored in register denoted by argument 1 into the register denoated by argument 0.

---

### MOVW
Argument 0 - reg16

Argument 1 - reg16
#### Desc.
Move the value stored in register denoted by argument 1 into the register denoated by argument 0.

---

### LDI
Argument 0 - reg8

Argument 1 - imm8
#### Desc.
Store a immediate in a register.

---

### LDIW
Argument 0 - reg16

Argument 1 - imm16
#### Desc.
Store a immediate in a register.

---

### LDFA
Argument 0 - reg16

Argument 1 - reg8
#### Desc.
Derefrence the value stored in the first argument and store that result in the second register.
```c
// Y is register 0
// X is register 1
X = *Y
```

---

### LDFAW
Argument 0 - reg16

Argument 1 - reg16
#### Desc.
Derefrence the value stored in the first argument and store that result in the second register.
```c
// Y is register 0
// X is register 1
X = *Y
```

---

### LDFAI
Argument 0 - imm16

Argument 1 - reg8
#### Desc.
Derefrence the value stored in the first argument and store that result in the second register.
```c
// Y is the immediate
// X is register 1
X = *Y
```

---

### LDFAIW
Argument 0 - imm16
Argument 1 - reg16
#### Desc.
Derefrence the value stored in the first argument and store that result in the second register.
```c
// Y is the immediate
// X is register
X = *Y
```

---

### HLT
Argument 0 - N/a

Argument 1 - N/a
#### Desc.
Halt the emulator.

---

### AND
Argument 0 - reg8

Argument 1 - reg8
#### Desc.
Performs a AND operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### ANDW
Argument 0 - reg16

Argument 1 - reg16
#### Desc.
Performs a AND operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### ANDI
Argument 0 - reg8

Argument 1 - imm8
#### Desc.
Performs a AND operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### ANDIW
Argument 0 - reg8

Argument 1 - imm16
#### Desc.
Performs a AND operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### OR
Argument 0 - reg8

Argument 1 - reg8
#### Desc.
Performs a OR operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### ORW
Argument 0 - reg16

Argument 1 - reg16
#### Desc.
Performs a OR operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### ORI
Argument 0 - reg8

Argument 1 - imm8
#### Desc.
Performs a OR operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### ORIW
Argument 0 - reg16

Argument 1 - imm16
#### Desc.
Performs a OR operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### INV
Argument 0 - reg8

Argument 1 - N/a
#### Desc.
Performs a NOT operation on the value and stores the result in the A register
#### Flags
Modifys zero flag

---

### INVW
Argument 0 - reg16

Argument 1 - N/a
#### Desc.
Performs a NOT operation on the value and stores the result in the A register
#### Flags
Modifys zero flag

---

### XOR
Argument 0 - reg8

Argument 1 - reg8
#### Desc.
Performs a XOR operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### XORW
Argument 0 - reg16

Argument 1 - reg16
#### Desc.
Performs a XOR operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### XORI
Argument 0 - reg8

Argument 1 - imm8
#### Desc.
Performs a XOR operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---

### XORIW
Argument 0 - reg16

Argument 1 - imm16
#### Desc.
Performs a XOR operation on the values and stores the result in the A register
#### Flags
Modifys zero flag

---