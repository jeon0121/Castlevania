#include "Object/LootType/Item/Chicken.hpp"

namespace LootItem {
Chicken::Chicken(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/items/item/chicken.png"}, 0, LootType::Chicken) {}

void Chicken::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
    (void) app;
    int health = character->GetHeart() + 6;
    if (health > 16)
        health = 16;
    character->SetHeart(health);
    menu->modifyHealth(health, "player");
    is_endResult = true;
}
}