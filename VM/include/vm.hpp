#ifndef _LIBTHEO_VM_VM_HPP_
#define _LIBTHEO_VM_VM_HPP_

#include <unordered_set>
#include <vector>
#include <string>
#include <utility>
#include <optional>

#include "VM/include/instr.hpp"
#include "program.hpp"

namespace Theo {

  class VM {
  public:
    typedef int Word, WordIndex;

    class Activation {
      VM *vm;
      
      WordIndex data_start;
      WordIndex seg_size;
      RegisterIndex ret_target;
      ProgramIndex ret_addr;
      StackMapIndex debug_info;

      Activation(VM *vm, WordIndex data_start, WordIndex seg_size, RegisterIndex ret_target, ProgramIndex ret_addr, StackMapIndex debug_info);
      
    public:
      typedef std::unordered_map<std::string, Word> Data;
      
      /**
       * get a list of variable names and corresponding current values
       */
      VM::Activation::Data getActivationVariables();

      friend class VM;
    };
    
  private:
    bool stepping_mode_enabled;
    ProgramIndex instruction_pointer;
    Program code;
    std::vector<Word> data;
    std::vector<Activation> stack;
    std::unordered_set<Program::LineBreak> enabled_breakpoints;

  public:
    VM(Program code);

    /**
     * get reference to activation stack (for debug purposes)
     */
    std::vector<VM::Activation>& getActivations();

    /**
     * returns the line number of the breakpoint the VM
     * is currently on
     * @return line number only if the VM is currently halted on a breakpoint, -1 otherwise
     */
    Program::LineBreak getCurrentBreak();

    /**
     * in stepping mode, potential breakpoints get treated as breakpoints
     * @param mode true enables stepping mode
     */
    void setSteppingMode (bool mode);

    bool isSteppingModeEnabled();

    /**
     * enable a breakpoint
     * @param line is the actual source code line to break on
     * @param value true to enable the breakpoint, false to disable
     * @return true if operation was successfull (false if e.g. line does not exist)
     */
    bool setBreakPoint (Program::LineBreak line, bool value);

    /**
     * disable all breakpoints
     */
    void clearBreakpoints();

    /**
     * reference to the set of currently enabled breakpoints
     */
    std::unordered_set<Program::LineBreak>& getEnabledBreakPoints();

    /**
     * resets the VM to its initial state;
     * any references held to activations will become invalid after this call;
     */
    void reset ();

    /**
     * executes code until it runs into BREAK or HALT;
     * upon reaching HALT, all subsequent calls to this method
     * will remain without effect
     */
    void execute ();
  };
  
}

#endif
