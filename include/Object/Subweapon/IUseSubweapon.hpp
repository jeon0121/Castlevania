#ifndef IUSESUBWEAPON_HPP
#define IUSESUBWEAPON_HPP

#include <string>

namespace Subweapon {
    class IUseSubweapon { // Interface for using subweapon
    public:
        virtual void Use() = 0;
        virtual ~IUseSubweapon() {}
        virtual void SetDirection(std::string direction) = 0;
        virtual std::string GetDirection() = 0;
        virtual bool IsDestroyed() = 0;
    };
}
#endif