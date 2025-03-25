#include "Object/LootType/Item/Bag.hpp"

namespace LootItem {
Bag::Bag(glm::vec2 position, LootType type) : Loot(position, {}, 50) {
    this->type = type;
    SetAnimationFrames(GetImage(), 50);
    SetPlaying();
    SetLooping(true);
}

std::vector<std::string> Bag::GetImage() {
    std::vector<std::string> bagImages;
    for (int i = 0; i < 3; ++i) {
        bagImages.emplace_back(GA_RESOURCE_DIR"/items/bag/bag-" + std::to_string(i + 1) + ".png");
    }
    switch(type) {
        case LootType::RedBag:
            addScore = 100;
        return {bagImages[0]};
        case LootType::PurpleBag:
            addScore = 400;
        return {bagImages[1]};
        case LootType::WhiteBag:
            addScore = 700;
        return {bagImages[2]};
        case LootType::SpecialBag:
            addScore = 1000;
        return bagImages;
        default:
            return {};
    }
}

void Bag::Result(App* app, std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu) {
    if (!score) {
        (void) character;
        glm::vec2 pos = GetPosition();
        menu->modifyNumber(menu->formatScore(menu->m_value.score + addScore), 0);
        std::vector<std::string> scoreImage = {GA_RESOURCE_DIR"/items/number/" + std::to_string(addScore) + ".png"};
        score = std::make_shared<AnimatedItems>(scoreImage, 0);
        score->SetPosition({pos.x + 25, pos.y + 20});
        score->SetPlaying();
        app->m_Root.AddChild(score);
    }
    if (score->IfPlayingTime(1)) {
        score->SetVisible(false);
        is_endResult = true;
    }
}
}