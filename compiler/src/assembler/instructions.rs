use super::argument::Argument;
use super::section::Section;
use std::collections::HashMap;
use strum_macros::EnumString;
use std::str::FromStr;

fn vec_opt_fix<T: Clone>(vec: &Vec<T>, index: usize) -> Option<T> {
  let val = vec.iter().nth(index);

  if val.is_some() {
    Some(val.unwrap().clone())
  } else {
    None
  }
}

#[derive(Clone, Copy, EnumString, Debug)]
pub enum InstructionSet {
  ADD,
  ADDW,
  ADDI,
  ADDIW,
  JMP,
  JMPI,
  JEQ,
  JEQI,
  JNE,
  JNEI,
  JLT,
  JLTI,
  JGT,
  JGTI,
  PUSH,
  PUSHW,
  PUSHIW,
  POP,
  POPW,
  POPN,
  MOV,
  MOVW,
  LDI,
  LDIW,
  LDFA,
  LDFAW,
  LDFAI,
  LDFAIW,
  STA,
  STAW,
  STAI,
  STAIW,
  CMP,
  CMPW,
  CMPI,
  CMPIW,
  HLT,
  AND,
  ANDW,
  ANDI,
  ANDIW,
  OR,
  ORW,
  ORI,
  ORIW,
  INV,
  INVW,
  XOR,
  XORW,
  XORI,
  XORIW,
  REGADD,
  REGADDW,
  INX,
  INY
}

#[derive(Clone)]
pub struct Instruction {
  instr: InstructionSet,
  arg0: Option<Argument>,
  arg1: Option<Argument>
}

impl Instruction {
  pub fn new(instr: InstructionSet, arg0: Option<Argument>, arg1: Option<Argument>) -> Self {
    Self { instr, arg0, arg1 }
  }
  pub fn to_bytes(&self, sections: &HashMap<String, Section>) -> Vec<u8> {
    let mut b = vec![];
    b.push(self.instr as u8);

    if self.arg0.is_some() {
      b.extend(self.arg0.as_ref().unwrap().to_bytes(sections));
    } else {
      b.extend(vec![0; 2]);
    }

    if self.arg1.is_some() {
      b.extend(self.arg1.as_ref().unwrap().to_bytes(sections));
    } else {
      b.extend(vec![0; 2]);
    }
    
    b
  }

  pub fn calculate_size(&self) -> usize {
    5
  }
}

#[derive(Clone)]
pub enum AssemblerInstructions {
  FILL(Vec<u8>),
  ZERO(u16)
}

impl AssemblerInstructions {
  pub fn to_bytes(&self) -> Vec<u8> {
    match &*self {
      AssemblerInstructions::FILL(v) => {
        v.clone()
      }
      AssemblerInstructions::ZERO(n_bytes) => {
        vec![0; *n_bytes as usize]
      }
    }
  }

  pub fn calculate_size(&self) -> usize {
    match &*self {
      AssemblerInstructions::FILL(v) => {
        v.len()
      }
      AssemblerInstructions::ZERO(n_bytes) => {
        *n_bytes as usize
      }
    }
  }
}

#[derive(Clone)]
pub enum InstructionType {
  AssemblerInstr(AssemblerInstructions),
  Instr(Instruction)
}

impl InstructionType {
  pub fn to_bytes(&self, sections: &HashMap<String, Section>) -> Vec<u8> {
    match &*self {
      InstructionType::AssemblerInstr(instr) => {
        instr.to_bytes()
      }
      InstructionType::Instr(instr) => {
        instr.to_bytes(sections)
      }
    }
  }

  pub fn calculate_size(&self) -> usize {
    match &*self {
      InstructionType::AssemblerInstr(instr) => {
        instr.calculate_size()
      }
      InstructionType::Instr(instr) => {
        instr.calculate_size()
      }
    }
  }

  pub fn parse(opcode: &str, args: Vec<Argument>) -> Self {
    if opcode == "DB" {
      let values = args.iter().map(|a| a.extract_u8()).collect::<Vec<u8>>();
      InstructionType::AssemblerInstr(AssemblerInstructions::FILL(values))
    } else if opcode == "ZERO" {
      InstructionType::AssemblerInstr(AssemblerInstructions::ZERO(args.first().unwrap().extract_u16()))
    } else {
      let instr = InstructionSet::from_str(opcode).unwrap();
      
      let arg0 = vec_opt_fix(&args, 0);
      let arg1 = vec_opt_fix(&args, 1);
      
      InstructionType::Instr(Instruction{ instr, arg0, arg1 })
    }
  }
}