#ifndef ROSARY_HPP
#define ROSARY_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
   class Rosary : public Loot {
   public:
      Rosary(glm::vec2 position);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;
   };
}
#endif