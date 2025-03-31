#ifndef LOOT_HEART_HPP
#define LOOT_HEART_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"
#include <iostream>

namespace LootItem {
   class Heart : public Loot {
   public:
      Heart(glm::vec2 position, LootType type);

      void Fall(const std::vector<std::shared_ptr<Block>>& m_Blocks) override;

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;

   private:
      int addScore;
      std::shared_ptr<AnimatedItems> score = nullptr;
   };
};
#endif
