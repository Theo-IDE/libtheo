#include "VM/include/vm.hpp"

using namespace Theo;

VM::Activation::Activation(VM *vm, VM::WordIndex data_start, VM::WordIndex seg_size,
               RegisterIndex ret_target, ProgramIndex ret_addr,
               StackMapIndex debug_info) {
  this->vm = vm;
  this->data_start = data_start;
  this->seg_size = seg_size;
  this->ret_target = ret_target;
  this->ret_addr = ret_addr;
  this->debug_info = debug_info;
}

VM::Activation::Data VM::Activation::getActivationVariables() {
  VM::Activation::Data res;
  Program::StackMap stack_map = this->vm->code.stack_maps[this->debug_info];
  for(int offset = 0; offset < this->seg_size; offset++) {
    for(auto &entry : stack_map){
      res[entry.second] = this->vm->data[this->data_start + entry.first];
    }
  }
  return res;
}

VM::VM(Program code) { this->code = code; }

std::stack<VM::Activation>& VM::getActivations() {
  return this->stack;
}

Program::LineBreak VM::getCurrentBreak() {
  auto itr = this->code.line_info.find(this->instruction_pointer);
  return (itr == this->code.line_info.end()) ? -1 : itr->second;
}

void VM::setSteppingMode(bool mode) {
  this->stepping_mode_enabled = mode;
}

bool VM::setBreakPoint(Program::LineBreak line, bool value) {
  auto itr = this->code.potential_breaks.find(line);
  if (itr == this->code.potential_breaks.end())
    return false;
  if(!value){
    this->enabled_breakpoints.erase(line);
    this->code.code[(*itr).second].op = OpCode::POTENTIAL_BREAK;
  } else {
    this->enabled_breakpoints.insert(line);
    this->code.code[(*itr).second].op = OpCode::BREAK;
  }
  return true;
}

void VM::clearBreakpoints() {
  for(Program::LineBreak const& bp : this->enabled_breakpoints) {
    ProgramIndex i = this->code.potential_breaks[bp];
    this->code.code[i].op = OpCode::POTENTIAL_BREAK;
  }
  this->enabled_breakpoints.clear();
}

std::unordered_set<Program::LineBreak> &VM::getEnabledBreakPoints() {
  return this->enabled_breakpoints;
}

bool VM::isSteppingModeEnabled() {
  return this->stepping_mode_enabled;
}

void VM::reset() {
  this->stepping_mode_enabled = false;
  this->instruction_pointer = 0;
  this->clearBreakpoints();
  this->data.clear();
  this->stack = std::stack<Activation>();
}

// TODO: implement execution :)
