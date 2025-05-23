
#include "cpu/o3/matrix_cpu.hh"

#include "cpu/activity.hh"
#include "cpu/checker/cpu.hh"
#include "cpu/checker/thread_context.hh"
#include "cpu/o3/dyn_inst.hh"
#include "cpu/o3/limits.hh"
#include "cpu/o3/thread_context.hh"
#include "cpu/simple_thread.hh"
#include "cpu/thread_context.hh"
#include "debug/Activity.hh"
#include "debug/Drain.hh"
#include "debug/O3CPU.hh"
#include "debug/Quiesce.hh"
#include "enums/MemoryMode.hh"
#include "sim/cur_tick.hh"
#include "sim/full_system.hh"
#include "sim/process.hh"
#include "sim/stat_control.hh"
#include "sim/system.hh"

namespace gem5
{

struct BaseO3CPUParams;

namespace o3
{

MatrixCPU::MatrixCPU(const BaseO3CPUParams &params)
    : CPU(params)
{
    this->isMatrix = true;
}

Port &
MatrixCPU::getPort(const std::string &if_name, PortID idx)
{
    // Get the right port based on name. This applies to all the
    // subclasses of the base CPU and relies on their implementation
    // of getDataPort and getInstPort.
    if (if_name == "dcache_port")
        return getDataPort();
    else if (if_name == "icache_port")
        return getInstPort();
    else if (if_name == "model_reset")
        return modelResetPort;
    else if (if_name == "mdcache_port")
        return getMatrixDataPort();
    else
        return ClockedObject::getPort(if_name, idx);
}

} // namespace o3
} // namespace gem5
