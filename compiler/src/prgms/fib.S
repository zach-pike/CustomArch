n_loops:
  DB #27

main:
  LDFAI %X, $n_loops ; Load the number of loops onto the stack

  MOVW %A, %IP       ; Copy the Instruction Pointer into A register
  ADDIW %A, #25W     ; Add 25 bytes to the return address
  PUSHW %A           ; Push the return address
  PUSH %X            ; Push the loop number argument
  JMPI $fib          ; Jump to the function

  POPW %A            ; Pop the return value off the stack
  HLT                ; Stop the VM

fib:
  POP %X             ; Pop the number of loops off the stack
  LDIW %C, #1W       ; Put a one in the C register
  JMPI $fib_loop     ; Jump to the loop
fib_loop:
  ADDW %B, %C        ; Add B and C regster (B + C = A)
  MOVW %C, %B        ; Move B register into C
  MOVW %B, %A        ; Move A into B

  INY                ; Increment the loop counter

  CMP %Y, %X         ; Compare the loop count and the number of loops
  JEQI $fib_ret      ; If the count and totalare the same return
  JMPI $fib_loop     ; Else jump back to the loop
fib_ret:
  POPW %B            ; Pop the return address off the stack
  PUSHW %A           ; Push the return value onto the stack
  JMP %B             ; Jump back