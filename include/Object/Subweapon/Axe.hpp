#ifndef SUBWEAPON_AXE_HPP
#define SUBWEAPON_AXE_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"
#include "IUseSubweapon.hpp"

namespace Subweapon {
   class Axe : public Loot, public IUseSubweapon {
   public:
      Axe(glm::vec2 position);

      void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;

      void Use() override;
   
      void SetDirection(std::string direction) override { m_direction = direction; }
      
      std::string GetDirection() override { return m_direction; }

      bool IsDestroyed() override { return false; }
   private:
      std::string m_direction;
   };
}
#endif
