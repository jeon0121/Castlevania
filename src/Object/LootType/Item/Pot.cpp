#include "Object/LootType/Item/Pot.hpp"
#include "State/Scene.hpp"

namespace LootItem {
    Pot::Pot(glm::vec2 position) : Loot(position, {GA_RESOURCE_DIR "/Sound Effects/27.wav"}, {GA_RESOURCE_DIR "/items/weapon/pot.png"}, 0, LootType::Pot) {}

    void Pot::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
        (void) app;
        (void) menu;
        character->SetInvisibleFlag(true);
        is_endResult = true;
    }
}