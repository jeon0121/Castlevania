#ifndef LOOT_BADGE_HPP
#define LOOT_BADGE_HPP

#include "Object/Loot.hpp"
#include "State/Menu.hpp"

namespace LootItem {
    class Badge : public Loot {
    public:
        Badge(glm::vec2 position);

        void Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) override;
    private:
        Uint64 startTime = 0;
    };
};

#endif