#ifndef LOOT_HOLYWATER_HPP
#define LOOT_HOLYWATER_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
class HolyWater : public Loot {
public:
   HolyWater(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/holywater.png"}, 0) {}

   void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
      menu->modifyWeapon(WeaponType::HolyWater);
      character->SetSubWeapon(WeaponType::HolyWater);
      (void) app;
      is_endResult = true;
   }
};
}
#endif
