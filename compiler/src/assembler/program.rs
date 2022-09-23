use std::collections::HashMap;
use super::section::Section;

fn insert_in_array_at_index(vec: &mut Vec<u8>, n_vec: &Vec<u8>, index: usize) {
  for i in 0..n_vec.len() {
    vec[index + i] = n_vec[i];
  }
}

pub struct Program {
  sections: HashMap<String, Section>
}

impl Program {
  pub fn new() -> Program {
    Program {
      sections: HashMap::new()
    }
  }

  pub fn add_section(&mut self, name: &str, sect: &Section) {
    self.sections.insert(name.to_string(), sect.clone());
  }

  fn set_offsets(&mut self) {
    let mut current_offset = 0;
    for (name, section) in &mut self.sections {
      
      if section.get_offset().is_some() {
        current_offset = section.get_offset().unwrap();
      } else if name == "main" {
        current_offset = 0x100;
      }

      section.set_offset(current_offset);
      current_offset += section.calculate_section_size();
    }
  }

  pub fn compile_sections(&mut self) -> Vec<u8> {
    self.set_offsets();
    let mut binary = Vec::new();
    binary.resize(65536, 0);

    for (_name, section) in &self.sections {
      insert_in_array_at_index(&mut binary, &section.compile_section(&self.sections), section.get_offset().unwrap());
    }

    binary
  }
}