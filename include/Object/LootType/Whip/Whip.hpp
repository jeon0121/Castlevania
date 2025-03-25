#ifndef LOOT_WHIP_HPP
#define LOOT_WHIP_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
class Whip : public Loot {
public:
   Whip(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/whip.png"}, 0) {}

   void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
      (void) menu;
      character->LevelUpWhip();
      is_endResult = true;
   }
};
}
#endif
