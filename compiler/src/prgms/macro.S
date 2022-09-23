@macro f_call(addr)
  PUSHW %A

  MOVW %A, %IP
  ADDIW %A, #20W
  JMP addr

  POPW %A
@endmacro

@macro f_return(v)
  POPW %A
  PUSH v
  JMP %A
@endmacro

@macro f_returnW(v)
  POPW %A
  PUSHW v
  JMP %A
@endmacro

@macro push_regs
PUSHW %A
PUSHW %B
PUSHW %C
PUSHW %D
PUSH %X
PUSH %Y
@endmacro

@macro pop_regs
POP %Y
POP %X
POPW %D
POPW %C
POPW %B
POPW %A
@endmacros

main:
  LDIW %A, #20W
  f_call($myfunc)

myfunc:
  ADDIW %A, #10W
  f_returnW(%A)