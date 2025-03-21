#ifndef LOOT_POT_HPP
#define LOOT_POT_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
class Pot : public Loot {
public:
   Pot(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/pot.png"}, 0) {}

   void Result(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
      
   }
};
}
#endif
