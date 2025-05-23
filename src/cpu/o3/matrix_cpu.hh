#ifndef __CPU_MATRIX_O3_CPU_HH__
#define __CPU_MATRIX_O3_CPU_HH__

#include <iostream>
#include <list>
#include <queue>
#include <set>
#include <vector>

#include "arch/generic/pcstate.hh"
#include "base/statistics.hh"
#include "cpu/o3/comm.hh"
#include "cpu/o3/cpu.hh"
#include "cpu/o3/commit.hh"
#include "cpu/o3/decode.hh"
#include "cpu/o3/dyn_inst_ptr.hh"
#include "cpu/o3/fetch.hh"
#include "cpu/o3/free_list.hh"
#include "cpu/o3/iew.hh"
#include "cpu/o3/limits.hh"
#include "cpu/o3/rename.hh"
#include "cpu/o3/rob.hh"
#include "cpu/o3/scoreboard.hh"
#include "cpu/o3/thread_state.hh"
#include "cpu/activity.hh"
#include "cpu/base.hh"
#include "cpu/simple_thread.hh"
#include "cpu/timebuf.hh"
#include "params/MatrixO3CPU.hh"
#include "params/BaseO3CPU.hh"
#include "sim/process.hh"



//*
namespace gem5
{

namespace o3
{

class MatrixCPU : public CPU
{
  public:
    MatrixCPU(const BaseO3CPUParams &params);
    
    Fault
    pushMatrixRequest(const DynInstPtr& inst, bool isLoad, uint8_t *data,
                unsigned int size, Addr addr, Request::Flags flags,
                uint64_t *res, AtomicOpFunctorPtr amo_op = nullptr,
                const std::vector<bool>& byte_enable=std::vector<bool>())
    {
        return iew.mldstQueue.pushRequest(inst, isLoad, data, size, addr,
                flags, res, std::move(amo_op), byte_enable);
    }

    Port &
    getMatrixDataPort()
    {
        return iew.mldstQueue.getDataPort();
    }

    Port &getPort(const std::string &if_name,
                  PortID idx=InvalidPortID) override;
};

} // namespace o3
} // namespace gem5

//*/
#endif // __CPU_MATRIX_O3_CPU_HH__