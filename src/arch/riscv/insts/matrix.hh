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
#include "debug/MatrixCmd.hh"

namespace gem5
{

namespace RiscvISA
{

struct MatrixCmd {
  enum Opcode {
    ML = 0x20,
    MS = 0x21,
    MLS = 0x24,
    MSS = 0x25,
    MZERO = 0x41,
    MMACU = 0x60,
    MADDU = 0x61,
    MSUBU = 0x62,
    MMAC = 0x68,
    MADD = 0x69,
    MSUB = 0x6A,
    MMACF = 0x70,
    MADDF = 0x71,
    MSUBF = 0x72,
  };

  enum Register {
    M0 = 0,
    M1,
    M2,
    M3,
    M4,
    M5,
    M6,
    M7,
    M8,
    M9,
    M10,
    M11,
    M12,
    M13,
    M14,
    M15,
    M16,
    M17,
    M18,
    M19,
    M20,
    M21,
    M22,
    M23,
    M24,
    M25,
    M26,
    M27,
    M28,
    M29,
    M30,
    M31,
  };

  enum SEW {
    _1 = 0,
    _2,
    _4,
    _8,
    _16,
    _32,
    _64,
    _128,
  };

  Opcode opcode : 8;
  Register md : 5;
  Register ms1 : 5;
  Register ms2 : 5;
  unsigned mrm : 1;
  SEW mdsew : 4;
  SEW ms1sew : 4;
  SEW ms2sew : 4;
  unsigned unused : 4;
  unsigned m : 8;
  unsigned n : 8;
  unsigned k : 8;
  uint64_t addr : 64;
  uint64_t stride : 64;
  uint64_t unused2 : 64;

  MatrixCmd() {
    opcode = Opcode::ML;
    md = Register::M0;
    ms1 = Register::M0;
    ms2 = Register::M0;
    mrm = 1;
    mdsew = SEW::_32;
    ms1sew = SEW::_32;
    ms2sew = SEW::_32;
    unused = 0;
    m = 4;
    n = 4;
    k = 4;
    addr = 0;
    stride = 0;
    unused2 = 0;
  }

  void print() const {
    print(addr, stride);
  }

  void print(unsigned long _addr) const {
    print(_addr, stride);
  }

  void print(unsigned long _addr, unsigned long _stride) const {
    DPRINTF(MatrixCmd, "MatrixCmd: %#016x%016x%016x%016x \n",
      ((uint64_t) opcode << 56) | ((uint64_t) md << 51) | ((uint64_t) ms1 << 46) | ((uint64_t) ms2 << 41) |
      ((uint64_t) mrm << 40) | ((uint64_t) mdsew << 36) | ((uint64_t) ms1sew << 32) | ((uint64_t) ms2sew << 28) |
      ((uint64_t) unused << 24) | ((uint64_t) m << 16) | ((uint64_t) n << 8) | k,
      _addr, _stride, unused2);
  }
};

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
    uint64_t mlen;

    MatrixMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint64_t _rlen,
                    uint64_t _mlen)
        : RiscvMacroInst(mnem, _machInst, __opClass),
        rlen(_rlen), mlen(_mlen)
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
    uint64_t rlen;
    uint64_t mlen;
    uint32_t microIdx;
    MatrixCmd cmd;

    MatrixMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _rlen, uint64_t _mlen)
        : RiscvMicroInst(mnem, _machInst, __opClass),
        rlen(_rlen), mlen(_mlen),
        microIdx(_microIdx)
    {
        this->flags[IsMatrix] = true;
        this->flags[IsMicroop] = true;
        // this->flags[IsNonSpeculative] = true;

        cmd.md = MatrixCmd::Register((unsigned) _machInst.mrd);
        cmd.ms1 = MatrixCmd::Register((unsigned) _machInst.mrs1);
        cmd.ms2 = MatrixCmd::Register((unsigned) _machInst.mrs2);
        cmd.mrm = MatrixCmd::Register((unsigned) _machInst.mrm);
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixArithMacroInst : public MatrixMacroInst {
  protected:
    MatrixArithMacroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint64_t _rlen, uint64_t _mlen)
        : MatrixMacroInst(mnem, _machInst, __opClass, _rlen, _mlen)
    {}
};

class MatrixArithMicroInst : public MatrixMicroInst {
  protected:
    MatrixArithMicroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint32_t _microIdx,
                         uint64_t _rlen, uint64_t _mlen)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx, _rlen, _mlen)
    {}
};

// TODO: Unused
class MatrixUnaryArithMacroInst : public MatrixArithMacroInst {
  protected:
    MatrixUnaryArithMacroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint64_t _rlen, uint64_t _mlen)
        : MatrixArithMacroInst(mnem, _machInst, __opClass, _rlen, _mlen)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixUnaryArithMicroInst : public MatrixArithMicroInst {
  protected:
    MatrixUnaryArithMicroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint32_t _microIdx,
                         uint64_t _rlen, uint64_t _mlen)
        : MatrixArithMicroInst(mnem, _machInst, __opClass, _microIdx, _rlen, _mlen)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixArithLineMicroInst : public MatrixMicroInst {
  protected:
    MatrixArithLineMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _rlen, uint64_t _mlen)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx, _rlen, _mlen)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMemMacroInst : public MatrixMacroInst {
  protected:
    MatrixMemMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint64_t _rlen, uint64_t _mlen)
        : MatrixMacroInst(mnem, _machInst, __opClass, _rlen, _mlen)
    {}
};

class MatrixMemMicroInst : public MatrixMicroInst {
  protected:
    Request::Flags memAccessFlags;

    MatrixMemMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _rlen, uint64_t _mlen)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx, _rlen, _mlen),
        memAccessFlags(0)
    {}
};

class MatrixLoadMacroInst : public MatrixMemMacroInst {
  protected:
    MatrixLoadMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint64_t _rlen, uint64_t _mlen)
        : MatrixMemMacroInst(mnem, _machInst, __opClass, _rlen, _mlen)
    {
        this->flags[IsLoad] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixLoadMicroInst : public MatrixMemMicroInst {
  protected:
    MatrixLoadMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _rlen, uint64_t _mlen)
        : MatrixMemMicroInst(mnem, _machInst, __opClass, _microIdx, _rlen, _mlen)
    {
        this->flags[IsLoad] = true;
    }

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixStridedLoadMicroInst : public MatrixMemMicroInst {
  protected:
    Request::Flags memAccessFlags;

    MatrixStridedLoadMicroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint32_t _microIdx,
                    uint64_t _rlen, uint64_t _mlen)
        : MatrixMemMicroInst(mnem, _machInst, __opClass, _microIdx, _rlen, _mlen),
        memAccessFlags(0)
    {
        this->flags[IsLoad] = true;
    }
};

class MatrixStoreMacroInst : public MatrixMemMacroInst {
  protected:
    MatrixStoreMacroInst(const char* mnem, ExtMachInst _machInst,
                    OpClass __opClass, uint64_t _rlen, uint64_t _mlen)
        : MatrixMemMacroInst(mnem, _machInst, __opClass, _rlen, _mlen)
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
                    uint64_t _rlen, uint64_t _mlen)
        : MatrixMemMicroInst(mnem, _machInst, __opClass, _microIdx, _rlen, _mlen)
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
                    uint64_t _rlen, uint64_t _mlen)
        : MatrixStoreMicroInst(mnem, _machInst, __opClass, _microIdx,
                          _rlen, _mlen)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMoveMacroInst : public MatrixMacroInst {
  protected:
    MatrixMoveMacroInst(const char* mnem, ExtMachInst _machInst,
                         OpClass __opClass, uint64_t _rlen,
                         uint64_t _mlen)
        : MatrixMacroInst(mnem, _machInst, __opClass, _rlen, _mlen)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};

class MatrixMoveMicroInst : public MatrixMicroInst {
  protected:
    MatrixMoveMicroInst(const char* mnem, ExtMachInst _machInst,
                        OpClass __opClass, uint32_t _microIdx,
                        uint64_t _rlen, uint64_t _mlen)
        : MatrixMicroInst(mnem, _machInst, __opClass, _microIdx, _rlen, _mlen)
    {}

    std::string generateDisassembly(
        Addr pc, const loader::SymbolTable *symtab) const override;
};


} // namespace RiscvISA
} // namespace gem5

#endif // __ARCH_RISCV_INSTS_MATRIX_HH__
