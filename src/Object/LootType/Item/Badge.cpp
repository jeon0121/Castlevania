#include "Object/LootType/Item/Badge.hpp"

#include "State/Menu.hpp"
#include "App.hpp"

namespace LootItem {
Badge::Badge(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR"/items/item/badge.png"}, 0, LootType::Badge) {}

void Badge::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
    (void) app;
    (void) character;
    menu->modifyBadge();
    is_endResult = true;
}
}