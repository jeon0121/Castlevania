#ifndef SUBWEAPON_HPP
#define SUBWEAPON_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

class Axe : public Loot {
public:
   Axe(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/axe.png"}, 0) {}

   void Result(std::shared_ptr<Menu>& menu) override {
      menu->modifyWeapon(WeaponType::Axe);
   }
};

#endif
