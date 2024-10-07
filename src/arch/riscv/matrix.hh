#ifndef __ARCH_RISCV_MATRIX_HH__
#define __ARCH_RISCV_MATRIX_HH__

#include <array>
#include <cassert>
#include <cstring>
#include <type_traits>

#include "base/cprintf.hh"
#include "base/logging.hh"
#include "base/types.hh"
#include "sim/serialize_handlers.hh"

namespace gem5
{

class MatStore
{
    static constexpr size_t RLEN = 256; // in bits
    static constexpr size_t LINES = RLEN / 32;

    static constexpr size_t LINEAR_SIZE = (RLEN * RLEN / 32) / 8;
  public:
    static constexpr inline size_t linearSize() { return LINEAR_SIZE; };

    using Container = std::array<uint8_t, LINEAR_SIZE>;
    using MyClass = MatStore;
  private:
    alignas(16) Container container;

  public:
    MatStore() {};
    MatStore(const MatStore&) = default;

    void
    zero()
    {
        memset(container.data(), 0, LINEAR_SIZE);
    }

    MyClass&
    operator=(const MyClass& that)
    {
        if (&that == this)
            return *this;
        memcpy(container.data(), that.container.data(), LINEAR_SIZE);
        return *this;
    }

    inline bool
    operator==(const MatStore& that) const
    {
        return !memcmp(container.data(), that.container.data(), LINEAR_SIZE);
    }

    bool
    operator!=(const MatStore& that) const
    {
        return !operator==(that);
    }

  private:
    /** Get pointer to the raw data. */
    template <typename ElemType>
    const ElemType* rawPtr() const
    {
        return reinterpret_cast<const ElemType*>(container.data());
    }

    template <typename ElemType>
    ElemType* rawPtr() {
        return reinterpret_cast<ElemType*>(container.data());
    }

  public:
    template <typename VecElem>
    VecElem *
    as()
    {
        static_assert(LINEAR_SIZE % sizeof(VecElem) == 0,
                "VecElem does not evenly divide the register size");
        return (VecElem *)container.data();
    }

    template <typename VecElem>
    const VecElem *
    as() const
    {
        static_assert(LINEAR_SIZE % sizeof(VecElem) == 0,
                "VecElem does not evenly divide the register size");
        return (VecElem *)container.data();
    }

    friend std::ostream&
    operator<<(std::ostream& os, const MatStore& v)
    {
        // When printing for human consumption, break into 4 byte chunks.
        ccprintf(os, "[");
        size_t count = 0;
        for (auto& b: v.container) {
            if (count && (count % 4) == 0)
                os << "_";
            ccprintf(os, "%02x", b);
            count++;
        }
        ccprintf(os, "]");
        return os;
    }

    friend ParseParam<MatStore>;
    friend ShowParam<MatStore>;
};

template<>
struct ParseParam<MatStore>
{
    static bool
    parse(const std::string &str, MatStore &value)
    {
        for (int i = 0; i < value.linearSize(); i++) {
            uint8_t b = 0;
            if (2 * i < str.size())
                b = stoul(str.substr(i * 2, 2), nullptr, 16);
            value.template rawPtr<uint8_t>()[i] = b;
        }
        return true;
    }
};

template<>
struct ShowParam<MatStore>
{
    static void
    show(std::ostream &os, const MatStore &value)
    {
        for (auto& b: value.container)
            ccprintf(os, "%02x", b);
    }
};

} // namespace gem5

#endif // __ARCH_RISCV_MATRIX_HH__
