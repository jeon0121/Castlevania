#ifndef SUBWEAPON_STOPWATCH_HPP
#define SUBWEAPON_STOPWATCH_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"
#include "SubweaponBase.hpp"

namespace Subweapon {
   class Stopwatch : public Loot, public SubweaponBase {
   public:
      Stopwatch(glm::vec2 position, std::string type);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;

      void Use(const std::vector<std::shared_ptr<Block>>& m_Blocks) override;
   
      void SetDirection(std::string direction) override { m_direction = direction; }
      
      std::string GetDirection() override { return m_direction; }

      int GetCost() override { return cost; }
   private:
      std::string m_direction;
      std::string m_type;
      int cost = 5;
      Uint64 stopTime = 0;
   };
}
#endif
