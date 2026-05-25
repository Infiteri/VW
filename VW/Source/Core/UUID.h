#pragma once

#include "Base.h"
#include <functional>

namespace VW
{
    class VW_API UUID
    {
    private:
        u64 id;

    public:
        UUID();
        UUID(u64 id);
        UUID(const UUID &other);
        ~UUID() = default;

        operator u64() const
        {
            return id;
        };
        inline u64 Get() const
        {
            return id;
        };
        bool operator==(const UUID &other) const
        {
            return id == other.id;
        };
        bool operator!=(const UUID &other) const
        {
            return id != other.id;
        };
    };
} // namespace VW

namespace std
{
    template <> struct hash<VW::UUID>
    {
        std::size_t operator()(const VW::UUID &uuid) const noexcept
        {
            return std::hash<u64>{}(uuid.Get());
        }
    };
} // namespace std
