#ifndef SUBWEAPONBASE_HPP
#define SUBWEAPONBASE_HPP

#include <string>
#include "Object/Block.hpp"

namespace Subweapon {
    class SubweaponBase {
    public:
        virtual void Use(const std::vector<std::shared_ptr<Block>>& m_Blocks) = 0;
        virtual ~SubweaponBase() {}
        virtual void SetDirection(std::string direction) = 0;
        virtual std::string GetDirection() = 0;
        void SetDestroyed(bool ifDestroy) { if_destroyed = ifDestroy; }
        bool IsDestroyed() { return if_destroyed; }
        void SetIsCollide(bool ifCollide) { if_collide = ifCollide; }
        virtual int GetCost() = 0;
    protected:
        bool if_destroyed = false;
        bool if_collide = false;
        std::shared_ptr<Util::SFX> usingSound;// sound when using subWeapon
    };
}
#endif