use super::argument::Argument;
use super::program::Program;
use super::section::Section;
use super::instructions::InstructionType;

#[derive(Debug, Clone)]
pub enum Token {
  LABEL{ name: String, offset: Option<usize> },
  OP{ op: String, args: Vec<Argument> }
}

impl Token {
  pub fn tokenize(text: &str) -> Vec<Self> {
    let mut tokens = vec![];
    
    for c in text.split("\n").into_iter() {
      let st = c.trim();
      let comment = st.find(";");

      let str = match comment {
        Some(i) => &st[..i],
        None => st
      }.trim();
      
      if str.len() == 0 { continue; }

      if str.ends_with(":") {
        if str.find("[").is_some() {
          let brack_start = str.find('[').expect("Could not find starting bracket!");
          let offset = usize::from_str_radix(&str[brack_start+1..str.len()-2], 10).unwrap();
          let name = &str[..brack_start];

          tokens.push(Token::LABEL{ name: name.to_string(), offset: Some(offset) });
        } else {
          tokens.push(Token::LABEL{ name: str[..str.len()- 1].to_string(), offset: None });
        }
      } else {
        // Read value up to first space
        match str.find(' ') {
          Some(i) =>{
            let opcode = &str[..i];
            let args_iter = &str[i+1..].replace(" ", "");
            let args = args_iter
                        .split(",")
                        .map(|k| Argument::parse(k))
                        .collect::<Vec<Argument>>();

            tokens.push(Token::OP{ op: opcode.to_string(), args });
          }
          None => {
            tokens.push(Token::OP{ op: str.to_string(), args: vec![] });
          }
        }
      }
    }
    tokens
  }
}

pub fn tokens_to_program(tok: &Vec<Token>) -> Program {
  let mut prog = Program::new();


  let mut current_section: Option<Section> = None;
  let mut current_section_name = String::new();

  for t in tok {
    match t {
      Token::LABEL{ name, offset } => {
        if current_section.is_some() {
          prog.add_section(&current_section_name, current_section.as_ref().unwrap());
        }
        
        current_section = Some(Section::new(*offset));
        current_section_name = name.clone();
      }
      
      Token::OP{ op, args } => {
        if current_section.is_some() {
          current_section.as_mut().unwrap().add_instruction(&InstructionType::parse(op, args.to_vec()));
        }
      }
    }
  }

  if current_section.is_some() {
    prog.add_section(&current_section_name, current_section.as_ref().unwrap());
  }

  prog
}