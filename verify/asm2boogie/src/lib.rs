use std::{fs, io::Write, path::Path};

use arm::{ArmFunction, arm_to_boogie_code, get_address_registers, get_used_registers};
use regex::Regex;

pub mod arm;
pub mod riscv;

#[derive(Debug, Clone, PartialEq)]
pub enum Operator {
    Immediate(String), 
    Address(String),
    Label(String),
    Register(String),
}

#[derive(Debug, Clone, PartialEq)]
pub enum UnifiedInstruction {
    Label(String),
    Instr(String, Vec<Operator>),
    Branch(String, String),
}



#[derive(Debug, Clone, PartialEq)]
pub enum FunctionClass {
    Read,
    Write,
    Await,
    AwaitRmw,
    Rmw,
}

fn classify_function(name: &str) -> FunctionClass {
    let rmw_re = Regex::new(r"add|sub|set").unwrap();
    if name.contains("read") {
        FunctionClass::Read
    } else if name.contains("write") {
        FunctionClass::Write
    } else if name.contains("await") {
        if rmw_re.is_match(name) {
            FunctionClass::AwaitRmw
        } else {
            FunctionClass::Await
        }
    } else {
        FunctionClass::Rmw
    }
}

fn get_templates_for_type(func_type: FunctionClass) -> Vec<&'static str> {
    match func_type {
        FunctionClass::Read => vec!["read_only.bpl", "read.bpl"],
        FunctionClass::Write => vec!["write.bpl", "must_store.bpl"],
        FunctionClass::Await => vec!["read_only.bpl", "read.bpl", "await.bpl"],
        FunctionClass::Rmw => vec!["read.bpl", "write.bpl", "rmw.bpl"],
        FunctionClass::AwaitRmw => vec!["read.bpl", "write.bpl", "rmw.bpl", "await.bpl"],
    }
}

pub fn generate_boogie_file(
    function: &ArmFunction,
    output_dir: &str,
    template_dir: &str,
) -> Result<(), std::io::Error> {
    let func_type = classify_function(&function.name);
    let templates = get_templates_for_type(func_type);

    let mut content = String::new();
    for template in templates {
        let template_path = Path::new(template_dir).join(template);
        let template_content = fs::read_to_string(&template_path)?;
        content.push_str(&template_content);
        content.push('\n');
    }

    let content = remove_duplicate_register_declarations(&content);

    let boogie_code = arm_to_boogie_code(function);
    let registers = get_used_registers(function);
    let address_list = get_address_registers(function);

    let arm_state = "local_monitor, monitor_exclusive, flags, event_register";
    let output = "x0";
    let input1 = "x0";
    let input2 = "x1";

    let content = content
        .replace("    #implementation", &boogie_code)
        .replace("#registers", &registers)
        .replace("#address", &address_list)
        .replace("#state", arm_state)
        .replace("#output", output)
        .replace("#input1", input1)
        .replace("#input2", input2);

    let output_file = Path::new(output_dir).join(format!("{}.bpl", function.name));
    fs::write(&output_file, content)?;

    Ok(())
}

pub fn generate_debug_file(code: &[ArmFunction], path: &str) -> Result<(), std::io::Error> {
    let mut file = fs::File::create(path)?;
    for function in code {
        let boogie_code = arm_to_boogie_code(function);
        let registers = get_used_registers(function);

        let content = format!(
            "// ---- {} ----\n{}\n{}\n",
            function.name, registers, boogie_code
        );
        writeln!(file, "{:#?}", content)?;
    }
    Ok(())
}

fn remove_duplicate_register_declarations(content: &str) -> String {
    let mut output = String::new();
    let mut register_declared = false;

    for line in content.lines() {
        if line.trim() == "var #registers: int;" {
            if !register_declared {
                output.push_str(line);
                register_declared = true;
            }
        } else {
            output.push_str(line);
            output.push('\n');
        }
    }
    output
}
