#ifndef LOOT_STOPWATCH_HPP
#define LOOT_STOPWATCH_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
class Stopwatch : public Loot {
public:
   Stopwatch(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/stopwatch.png"}, 0) {}

   void Result(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
      menu->modifyWeapon(WeaponType::Stopwatch);
      (void) character;
   }
};
}
#endif
