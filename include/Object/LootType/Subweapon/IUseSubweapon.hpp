#ifndef IUSESUBWEAPON_HPP
#define IUSESUBWEAPON_HPP

namespace LootItem {
    class IUseSubweapon { // Interface for using subweapon
    public:
        virtual void Use(std::string direction) = 0;
        virtual ~IUseSubweapon() {}
    };
}
#endif