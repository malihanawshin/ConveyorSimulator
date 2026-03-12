#pragma once
#include <functional>

enum class ItemType {
    TypeA,
    TypeB
};

namespace std {
template <>
struct hash<ItemType> {
    std::size_t operator()(const ItemType& itemType) const {
        return std::hash<int>()(static_cast<int>(itemType));
    }
};
}
