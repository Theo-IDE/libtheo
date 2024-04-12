#ifndef _LIBTHEO_VM_INSTR_HPP_
#define _LIBTHEO_VM_INSTR_HPP_

namespace Theo {
  enum class OpCode {
    POTENTIAL_BREAK,
    BREAK,
    HALT,
    ADD_CONST,
    JMP,
    JMPC,
    PREPARE_EXEC,
    ARG,
    EXEC,
    RET
  };
  
  typedef int RegisterIndex, JumpOffset, Constant, ProgramIndex, RegisterCount, StackMapIndex;
  
  struct Instruction {
    OpCode op;
    
    union {
      struct {
	RegisterIndex target;
	RegisterIndex source;
	Constant constant;
      } add;
      struct {
	JumpOffset offset;
      } jmp;
      struct {
	JumpOffset offset;
	RegisterIndex source;
      } jmpc;
      struct {
	RegisterCount count;
	StackMapIndex index;
      } prepare;
      struct {
	RegisterIndex target;
	RegisterIndex source;
      } arg;
      struct {
	RegisterIndex target;
	ProgramIndex entry;
      } exec;
      struct {
	RegisterIndex source;
      } ret;
    } parameters;

    static Instruction PotentialBreak();
    
    static Instruction Halt();

    static Instruction Break();
    
    /**
     * <target> := <source> + <constant>
     */
    static Instruction Add(RegisterIndex target, RegisterIndex source, Constant constant);

    /**
     * InstructionPointer := InstructionPointer + <offset>
     */
    static Instruction Jmp(JumpOffset offset);

    /**
     * IF <source> != 0 THEN InstructionPointer := InstructionPointer + <offset>
     */
    static Instruction JmpC(JumpOffset offset, RegisterIndex source);

    /**
     * Creates new stack frame after the current one with <count> registers
     * and initializes those to zero;
     * StackMapIndex is the index to a stack map assigning names of variables
     * to virtual registers
     */
    static Instruction PrepareExec (RegisterCount count, StackMapIndex index);

    /**
     * copies from register <source> of second to last stack frame
     * to register <target> of last stack frame
     */
    static Instruction Arg(RegisterIndex target, RegisterIndex source);

    /**
     * InstructionPointer := <entry>;
     * sets the return target of the current stack frame to register <target>
     * of the second to last stack frame
     * used for calling functions:
     * PREPARE 3
     * ARG 1 13
     * ARG 2 14
     * EXEC <entry>
     */
    static Instruction Exec(RegisterIndex target, ProgramIndex entry);

    /**
     * copies from register <source> of the current stack frame
     * to the return register specified for the second to last stack frame
     * and resets InstructionPointer to the return address
     */
    static Instruction Ret(RegisterIndex source);
  };
}

#endif
