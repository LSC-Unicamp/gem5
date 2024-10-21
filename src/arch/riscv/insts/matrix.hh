#ifndef __ARCH_RISCV_INSTS_MATRIX_HH__
#define __ARCH_RISCV_INSTS_MATRIX_HH__

#include <string>

#include "arch/riscv/insts/static_inst.hh"
#include "arch/riscv/isa.hh"
#include "arch/riscv/matrix.hh"
#include "arch/riscv/regs/misc.hh"
#include "arch/riscv/utility.hh"
#include "cpu/exec_context.hh"
#include "cpu/static_inst.hh"

namespace gem5
{

namespace RiscvISA
{

/**
 * Base class for Matrix operations
 */
class MatrixOp : public RiscvStaticInst
{
  protected:
    MatrixOp(const char *mnem, ExtMachInst _extMachInst, OpClass __opClass)
        : RiscvStaticInst(mnem, _extMachInst, __opClass)
    {
        this->flags[IsMatrix] = true;
    };

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixNonSplitInst : public MatrixOp
{
  protected:
    MatrixNonSplitInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass)
        : MatrixOp(mnem, _machInst, __opClass)
    {}
};

class MatrixMacroInst : public RiscvMacroInst
{
  protected:
    MatrixMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass)
        : RiscvMacroInst(mnem, _machInst, __opClass)
    {
        this->flags[IsMacroop] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMicroInst : public RiscvMicroInst
{
  protected:
    uint32_t microIdx;

    MatrixMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx)
        : RiscvMicroInst(mnem, _machInst, __opClass),
        microIdx(_microIdx)
    {
        this->flags[IsMicroop] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixArithMacroInst : public MatrixMacroInst {
  protected:
    MatrixArithMacroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass)
        : MatrixMacroInst(mnem, _machInst, __opClass)
    {}
};

class MatrixArithMicroInst : public MatrixMicroInst {
  protected:
    MatrixArithMicroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint32_t _microIdx)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx)
    {}
};

// TODO: Unused
class MatrixUnaryArithMacroInst : public MatrixArithMacroInst {
  protected:
    MatrixUnaryArithMacroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass)
        : MatrixArithMacroInst(mnem, _machInst, __opClass)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixUnaryArithMicroInst : public MatrixArithMicroInst {
  protected:
    MatrixUnaryArithMicroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint32_t _microIdx)
        : MatrixArithMicroInst(mnem, _machInst, __opClass, _microIdx)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixArithLineMicroInst : public MatrixMicroInst {
  protected:
    Request::Flags memAccessFlags;
    
    MatrixArithLineMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx
                          ), 
          memAccessFlags(0)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMemMacroInst : public MatrixMacroInst {
  protected:
    MatrixMemMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass)
        : MatrixMacroInst(mnem, _machInst, __opClass)
    {}
};

class MatrixMemMicroInst : public MatrixMicroInst {
  protected:
    MatrixMemMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx)
    {}
};

class MatrixLoadMacroInst : public MatrixMemMacroInst {
  protected:
    MatrixLoadMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass)
        : MatrixMemMacroInst(mnem, _machInst, __opClass)
    {
        this->flags[IsLoad] = true;
    }
};

class MatrixLoadMicroInst : public MatrixMicroInst {
  protected:
    Request::Flags memAccessFlags;
    
    MatrixLoadMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx), 
        memAccessFlags(0)
    {
        this->flags[IsLoad] = true;
    }
};

class MatrixStoreMacroInst : public MatrixMemMacroInst {
  protected:
    MatrixStoreMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass)
        : MatrixMemMacroInst(mnem, _machInst, __opClass)
    {
        this->flags[IsStore] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixStoreMicroInst : public MatrixMicroInst {
  protected:
    Request::Flags memAccessFlags;
    
    MatrixStoreMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx), 
        memAccessFlags(0)
    {
        this->flags[IsStore] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMoveMacroInst : public MatrixMacroInst {
  protected:
    MatrixMoveMacroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass)
        : MatrixMacroInst(mnem, _machInst, __opClass)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMoveMicroInst : public MatrixMicroInst {
  protected:
    MatrixMoveMicroInst(const char* mnem, ExtMachInst _machInst,
                        OpClass __opClass, uint32_t _microIdx)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};


} // namespace RiscvISA
} // namespace gem5

#endif // __ARCH_RISCV_INSTS_MATRIX_HH__
