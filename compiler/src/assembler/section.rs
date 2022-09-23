use super::instructions::{InstructionType};
use std::collections::HashMap;

#[derive(Clone)]
pub struct Section {
  instructions: Vec<InstructionType>,
  offset: Option<usize>
}

impl Section {
  pub fn new(offset:Option<usize>) -> Self {
    Self {
      instructions: Vec::new(),
      offset
    }
  }

  pub fn add_instruction(&mut self, instr: &InstructionType) {
    self.instructions.push(instr.clone());
  }
  
  pub fn calculate_section_size(&self) -> usize {
    let mut size = 0;

    for instr in &self.instructions {
      size += instr.calculate_size();
    }

    size
  }

  pub fn set_offset(&mut self, offset: usize) {
    self.offset = Some(offset);
  }

  pub fn get_offset(&self) -> Option<usize> {
    self.offset
  }

  pub fn compile_section(&self, sections: &HashMap<String, Section>) -> Vec<u8> {
    let mut binary = Vec::new();

    for instr in &self.instructions {
      binary.extend(instr.to_bytes(sections));
    }

    binary
  }
}