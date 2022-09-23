myarr:
  ZERO #20
myarr_size:
  DB #0

arr_push:
  LDFAI $myarr_size, %BL
  
  LDIW $myarr, %A
  ADDW %A, 
  

arr_push_ret:
  POPW %A
  JMP %A

main:
  