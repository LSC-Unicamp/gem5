#include "arch/riscv/insts/matrix.hh"

#include <sstream>
#include <string>

#include "arch/riscv/insts/static_inst.hh"
#include "arch/riscv/isa.hh"
#include "arch/riscv/regs/misc.hh"
#include "arch/riscv/regs/matrix.hh"
#include "arch/riscv/utility.hh"
#include "cpu/static_inst.hh"

namespace gem5
{

namespace RiscvISA
{

std::string
MatrixOp::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const {
    std::stringstream ss;
    ss << mnemonic << ' ' 
        << registerName(destRegIdx(0)) << ", " 
        << registerName(srcRegIdx(0)) << ", " 
        << registerName(srcRegIdx(1));
    return ss.str();
}

std::string
MatrixMacroInst::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const {
    std::stringstream ss;
    ss << mnemonic << ' ' 
        << registerName(destRegIdx(0)) << ", " 
        << registerName(srcRegIdx(0)) << ", " 
        << registerName(srcRegIdx(1));
    return ss.str();
}

std::string
MatrixMicroInst::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const {
    std::stringstream ss;
    ss << mnemonic << ' ' 
        << registerName(destRegIdx(0)) << ", " 
        << registerName(srcRegIdx(0)) << ", " 
        << registerName(srcRegIdx(1));
    return ss.str();
}

std::string
MatrixStoreMacroInst::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const {
    std::stringstream ss;
    ss << mnemonic << ' '
        << registerName(srcRegIdx(0)) << ", " 
        << registerName(srcRegIdx(1)) << ", " 
        << registerName(srcRegIdx(2));
    return ss.str();
}

std::string
MatrixStoreMicroInst::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const {
    std::stringstream ss;
    ss << mnemonic << ' '
        << registerName(srcRegIdx(0)) << ", " 
        << registerName(srcRegIdx(1)) << ", " 
        << registerName(srcRegIdx(2));
    return ss.str();
}

std::string
MatrixArithLineMicroInst::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const {
    std::stringstream ss;
    ss << mnemonic << ' '
        << registerName(destRegIdx(0)) << ", " 
        << registerName(srcRegIdx(0)) << ", " 
        << registerName(srcRegIdx(1));
    return ss.str();
}

std::string
MatrixUnaryArithMacroInst::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const {
    std::stringstream ss;
    ss << mnemonic << ' '
        << registerName(destRegIdx(0));
    return ss.str();
}

std::string
MatrixUnaryArithMicroInst::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const {
    std::stringstream ss;
    ss << mnemonic << ' '
        << registerName(destRegIdx(0));
    return ss.str();
}

std::string
MatrixMoveMacroInst::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const {
    std::stringstream ss;
    ss << mnemonic << ' '
        << registerName(destRegIdx(0)) << ", " 
        << registerName(srcRegIdx(0));
    return ss.str();
}

std::string
MatrixMoveMicroInst::generateDisassembly(Addr pc, const loader::SymbolTable *symtab) const {
    std::stringstream ss;
    ss << mnemonic << ' '
        << registerName(destRegIdx(0)) << ", " 
        << registerName(srcRegIdx(0));
    return ss.str();
}

} // namespace RiscvISA
} // namespace gem5
