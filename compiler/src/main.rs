#![allow(dead_code, unused_variables, unused_must_use, unused_imports)]

mod assembler;
use assembler::{ instructions::*, program::*, argument::*, textparser::{ Token, tokens_to_program } };

use std::io::Write; // bring trait into scope
use std::io::Read; // bring trait into scope
use std::fs;
use std::path::Path;

fn main() {
  let f_bytes = fs::read("src/test.S").unwrap();
  let file = String::from_utf8_lossy(&f_bytes);
  let tok = Token::tokenize(&file);
  let prgm = tokens_to_program(&tok).compile_sections();
  
  fs::write(Path::new("prgm.bin"), prgm);
}