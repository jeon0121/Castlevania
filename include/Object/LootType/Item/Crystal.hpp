#ifndef LOOT_CRYSTAL_HPP
#define LOOT_CRYSTAL_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
   class Crystal : public Loot {
   public:
      Crystal(glm::vec2 position);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;
   };
}
#endif
