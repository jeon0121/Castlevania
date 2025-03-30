#ifndef LOOT_DAGGER_HPP
#define LOOT_DAGGER_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"
#include "IUseSubweapon.hpp"

namespace LootItem {
class Dagger : public Loot, public IUseSubweapon {
public:
   Dagger(glm::vec2 position);

   void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;

   void Use(std::string direction) override;
};
}
#endif
