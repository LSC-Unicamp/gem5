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
class MatrixNonSplitInst : public RiscvStaticInst
{
  protected:
    MatrixNonSplitInst(const char* mnem, ExtMachInst _machInst,
OpClass __opClass)
        : RiscvStaticInst(mnem, _machInst, __opClass)
    {
        this->flags[IsMatrix] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMacroInst : public RiscvMacroInst
{
  protected:
    uint64_t rlen;
    uint64_t microMl;

    MatrixMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint64_t _rlen = 128,
                    uint64_t _microMl = 256)
        : RiscvMacroInst(mnem, _machInst, __opClass),
        rlen(_rlen),
        microMl(_microMl)
    {
        this->flags[IsMatrix] = true;
        this->flags[IsMacroop] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMicroInst : public RiscvMicroInst
{
  protected:
    uint32_t microIdx;
    uint64_t microMl;

    MatrixMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx, uint64_t _microMl)
        : RiscvMicroInst(mnem, _machInst, __opClass),
        microIdx(_microIdx),
        microMl(_microMl)
    {
        this->flags[IsMatrix] = true;
        this->flags[IsMicroop] = true;
        // this->flags[IsNonSpeculative] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixArithMacroInst : public MatrixMacroInst {
  protected:
    MatrixArithMacroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint64_t _rlen = 128,
                         uint64_t _microMl = 256)
        : MatrixMacroInst(mnem, _machInst, __opClass, _rlen,
                          _microMl)
    {}
};

class MatrixArithMicroInst : public MatrixMicroInst {
  protected:
    MatrixArithMicroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint32_t _microIdx,
                         uint64_t _microMl = 256)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx,
                          _microMl)
    {}
};

// TODO: Unused
class MatrixUnaryArithMacroInst : public MatrixArithMacroInst {
  protected:
    MatrixUnaryArithMacroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint64_t _rlen = 128)
        : MatrixArithMacroInst(mnem, _machInst, __opClass, _rlen)
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
    MatrixArithLineMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _microMl = 256)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx,
                          _microMl)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMemMacroInst : public MatrixMacroInst {
  protected:
    MatrixMemMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint64_t _rlen,
                    uint64_t _microMl)
        : MatrixMacroInst(mnem, _machInst, __opClass, _rlen,
                          _microMl)
    {}
};

class MatrixMemMicroInst : public MatrixMicroInst {
  protected:
    Request::Flags memAccessFlags;

    MatrixMemMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _microMl = 256)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx,
                          _microMl),
        memAccessFlags(0)
    {}
};

class MatrixLoadMacroInst : public MatrixMemMacroInst {
  protected:
    MatrixLoadMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint64_t _rlen = 128,
                    uint64_t _microMl = 256)
        : MatrixMemMacroInst(mnem, _machInst, __opClass, _rlen,
                             _microMl)
    {
        this->flags[IsLoad] = true;
    }
};

class MatrixLoadMicroInst : public MatrixMemMicroInst {
  protected:
    MatrixLoadMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _microMl = 256)
        : MatrixMemMicroInst(mnem, _machInst, __opClass, _microIdx,
                          _microMl)
    {
        this->flags[IsLoad] = true;
    }
};

class MatrixStridedLoadMicroInst : public MatrixMemMicroInst {
  protected:
    Request::Flags memAccessFlags;

    MatrixStridedLoadMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _microMl = 256)
        : MatrixMemMicroInst(mnem, _machInst, __opClass, _microIdx,
                          _microMl),
        memAccessFlags(0)
    {
        this->flags[IsLoad] = true;
    }
};

class MatrixStoreMacroInst : public MatrixMemMacroInst {
  protected:
    MatrixStoreMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint64_t _rlen = 128,
                    uint64_t _microMl = 256)
        : MatrixMemMacroInst(mnem, _machInst, __opClass, _rlen,
                            _microMl)
    {
        this->flags[IsStore] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixStoreMicroInst : public MatrixMemMicroInst {
  protected:
    MatrixStoreMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _microMl = 256)
        : MatrixMemMicroInst(mnem, _machInst, __opClass, _microIdx,
                          _microMl)
    {
        this->flags[IsStore] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixStridedStoreMicroInst : public MatrixStoreMicroInst {
  protected:
    MatrixStridedStoreMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _microMl = 256)
        : MatrixStoreMicroInst(mnem, _machInst, __opClass, _microIdx,
                          _microMl)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMoveMacroInst : public MatrixMacroInst {
  protected:
    MatrixMoveMacroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint64_t _rlen = 128,
                         uint64_t _microMl = 256)
        : MatrixMacroInst(mnem, _machInst, __opClass, _rlen,
                          _microMl)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMoveMicroInst : public MatrixMicroInst {
  protected:
    MatrixMoveMicroInst(const char* mnem, ExtMachInst _machInst,
                        OpClass __opClass, uint32_t _microIdx,
                        uint64_t _microMl = 256)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx,
                          _microMl)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};


} // namespace RiscvISA
} // namespace gem5

#endif // __ARCH_RISCV_INSTS_MATRIX_HH__
