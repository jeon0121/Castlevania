#ifndef IUSESUBWEAPON_HPP
#define IUSESUBWEAPON_HPP

#include <string>

namespace Subweapon {
    class IUseSubweapon { // Interface for using subweapon
    public:
        virtual void Use() = 0;
        virtual ~IUseSubweapon() {}
        void SetDirection(std::string direction) { m_direction = direction; }
        std::string GetDirection() { return m_direction; }
    private:
        std::string m_direction;
    };
}
#endif