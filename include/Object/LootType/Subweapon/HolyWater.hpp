#ifndef LOOT_HOLYWATER_HPP
#define LOOT_HOLYWATER_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
class HolyWater : public Loot {
public:
   HolyWater(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/holywater.png"}, 0) {}

   void Result(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
      menu->modifyWeapon(WeaponType::HolyWater);
      (void) character;
   }
};
}
#endif
