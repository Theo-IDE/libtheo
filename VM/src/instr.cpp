#include "VM/include/instr.hpp"

using namespace Theo;

Instruction Instruction::Nop() {
  return {
    .op = OpCode::NOP
  };
}

Instruction Instruction::Add(RegisterIndex target, RegisterIndex source, Constant constant){
  return {
    .op = OpCode::ADD_CONST,
    .parameters = {
      .add = {
	.target = target,
	.source = source,
	.constant = constant
      }
    }
  };
}

Instruction Instruction::Halt() { return {.op = OpCode::HALT}; }

Instruction Instruction::Jmp(JumpOffset offset) {
  return {
    .op = OpCode::JMP,
    .parameters = {
      .jmp = {
	.offset = offset,
      }
    }
  };
}

Instruction Instruction::JmpC(JumpOffset offset, RegisterIndex source) {
  return {
    .op = OpCode::JMPC,
    .parameters = {
      .jmpc = {
	.offset = offset,
	.source = source
      }
    }
  };
}

Instruction Instruction::PrepareExec(RegisterCount count, StackMapIndex index) {
  return {
    .op = OpCode::PREPARE_EXEC,
    .parameters = {
      .prepare = {
	.count = count,
	.index = index
      }
    }
  };
}

Instruction Instruction::Arg(RegisterIndex target, RegisterIndex source) {
  return {
    .op = OpCode::ARG,
    .parameters = {
      .arg = {
	.target = target,
	.source = source
      }
    }
  };
}

Instruction Instruction::Exec(RegisterIndex target, ProgramIndex entry) {
  return {
    .op = OpCode::EXEC,
    .parameters = {
      .exec = {
	.target = target,
	.entry = entry
      }
    }
  };
}

Instruction Instruction::Ret(RegisterIndex source) {
  return {
    .op = OpCode::RET,
    .parameters = {
      .ret = {
	.source = source
      }
    }
  };
}
