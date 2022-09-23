use strum_macros::EnumString;

#[derive(EnumString)]
pub enum  U16Registers {
  A,
  B,
  C,
  D,
  SP,
  IP
}

#[derive(EnumString)]
pub enum U8Registers {
  AL,
  AH,
  BL,
  BH,
  CL,
  CH,
  DL,
  DH,
  X,
  Y
}