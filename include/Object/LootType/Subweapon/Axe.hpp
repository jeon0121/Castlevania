#ifndef LOOT_AXE_HPP
#define LOOT_AXE_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
class Axe : public Loot {
public:
   Axe(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/axe.png"}, 0) {}

   void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
      menu->modifyWeapon(WeaponType::Axe);
      (void) character;
      is_endResult = true;
   }
};
}
#endif
