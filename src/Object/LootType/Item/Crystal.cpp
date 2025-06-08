#include "Object/LootType/Item/Crystal.hpp"

namespace LootItem {
Crystal::Crystal(glm::vec2 position) : Loot(position, {}, {}, 0, LootType::Crystal) {
    SetAnimationFrames({GA_RESOURCE_DIR "/items/item/crystal-1.png", GA_RESOURCE_DIR "/items/item/crystal-2.png"}, 50);
    SetPlaying();
    SetLooping(true);
}

void Crystal::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
    (void) app;
    character->SetHeart(16);
    menu->modifyHealth(16, "player");
    is_endResult = true;
}
}