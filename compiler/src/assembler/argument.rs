use std::collections::HashMap;
use super::section::Section;
use std::str::FromStr;
use super::registers::{ U8Registers, U16Registers };

#[derive(Clone, Debug)]
pub enum Argument {
  Raw8Bit(u8),
  Raw16Bit(u16),
  SectionAddressByName(String),
}

impl Argument {
  pub fn extract_u8(&self) -> u8 {
    match &*self {
      Argument::Raw8Bit(v) => *v,
      Argument::Raw16Bit(v) => panic!(),
      Argument::SectionAddressByName(n) => panic!()
    }  
  }
  pub fn extract_u16(&self) -> u16 {
    match &*self {
      Argument::Raw8Bit(v) => panic!(),
      Argument::Raw16Bit(v) => *v,
      Argument::SectionAddressByName(n) => panic!()
    }  
  }
  
  pub fn to_bytes(&self, sections: &HashMap<String, Section>) -> [u8; 2] {
    match self {
      Argument::Raw8Bit(v) => {
        (*v as u16).to_be_bytes()
      }
      Argument::Raw16Bit(v) => {
        v.to_be_bytes()
      }
      Argument::SectionAddressByName(v) => {
        let res = sections.get(v).unwrap().get_offset().unwrap();
        (res as u16).to_be_bytes()
      }
    }
  }

  pub fn parse(arg: &str) -> Self {
    if arg.chars().nth(0).unwrap() == '#' {
      if arg.ends_with('W') {
        Argument::Raw16Bit(u16::from_str(&arg[1..arg.len()-1]).unwrap())
      } else {
        Argument::Raw8Bit(u8::from_str(&arg[1..]).unwrap())
      }
    } else if arg.chars().nth(0).unwrap() == '%' {
      let reg = &arg[1..];

      //first check u8 registers
      let u8reg = U8Registers::from_str(reg);
      let u16reg = U16Registers::from_str(reg);  

      if u8reg.is_ok() {
        Argument::Raw8Bit(u8reg.unwrap() as u8)
      } else {
        Argument::Raw8Bit(u16reg.unwrap() as u8)
      }
    } else {
      Argument::SectionAddressByName(arg[1..].to_string())
    }
  }
}