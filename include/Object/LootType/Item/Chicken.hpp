#ifndef LOOT_CHICKEN_HPP
#define LOOT_CHICKEN_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
   class Chicken : public Loot {
   public:
      Chicken(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/item/chicken.png"}, 0) {}

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
         
      }
   };
}
#endif
