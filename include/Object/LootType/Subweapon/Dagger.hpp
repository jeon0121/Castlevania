#ifndef LOOT_DAGGER_HPP
#define LOOT_DAGGER_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
class Dagger : public Loot {
public:
   Dagger(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/dagger.png"}, 0) {}

   void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
      menu->modifyWeapon(WeaponType::Dagger);
      (void) character;
      is_endResult = true;
   }
};
}
#endif
