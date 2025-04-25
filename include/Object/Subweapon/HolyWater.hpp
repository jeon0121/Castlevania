#ifndef SUBWEAPON_HOLYWATER_HPP
#define SUBWEAPON_HOLYWATER_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"
#include "IUseSubweapon.hpp"

namespace Subweapon {
   class HolyWater : public Loot, public IUseSubweapon {
   public:
      HolyWater(glm::vec2 position, std::string type);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;

      void Use(const std::vector<std::shared_ptr<Block>>& m_Blocks) override;
   
      void SetDirection(std::string direction) override { m_direction = direction; }
      
      std::string GetDirection() override { return m_direction; }

      int GetCost() override { return cost; }
   private:
      std::string m_direction;
      std::string m_type;
      std::vector<std::string> m_throw, m_flame;
      int cost = 1;
   };
}
#endif
