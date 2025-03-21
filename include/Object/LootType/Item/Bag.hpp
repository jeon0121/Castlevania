#ifndef LOOT_BAG_HPP
#define LOOT_BAG_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
class Bag : public Loot {
public:
   Bag(glm::vec2 position, int type) : Loot(position, {}, 0) {
      
   }

   void Result(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override {
      
   }
};
}
#endif
