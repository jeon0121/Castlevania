#ifndef LOOT_POT_HPP
#define LOOT_POT_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
   class Pot : public Loot {
   public:
      Pot(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/weapon/pot.png"}, 0, LootType::Pot) {}

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
         
      }
   };
}
#endif
