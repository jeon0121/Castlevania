#include "Object/Torch.hpp"

Torch::Torch(glm::vec2 position, glm::vec2 scale, LootType itemType, int type)
    : AnimatedItems((type == 1) ? std::vector<std::string>{GA_RESOURCE_DIR "/items/fire/fire-1.png", GA_RESOURCE_DIR "/items/fire/fire-2.png"}
                                : std::vector<std::string>{GA_RESOURCE_DIR "/items/fire/fire-3.png", GA_RESOURCE_DIR "/items/fire/fire-4.png"},
    120) {
    SetPosition(position);
    m_Transform.scale = scale;
    SetPlaying();
    SetLooping(true);
    SetZIndex(6);
    this->itemType = itemType;

    for (int i = 0; i < 4; ++i) {
        torchDeath.emplace_back(GA_RESOURCE_DIR"/items/fire/torch/torchDeath-" + std::to_string(i + 1) + ".png");
    }
    UpdatePosition();
}

void Torch::UpdatePosition() {
    glm::vec2 torchPos = GetPosition();
    glm::vec2 torchSize = glm::abs(GetScaledSize());
    torchTop = torchPos.y + torchSize.y * 0.5f;
    torchBottom = torchPos.y - torchSize.y * 0.5f;
    torchLeft = torchPos.x - torchSize.x * 0.5f;
    torchRight = torchPos.x + torchSize.x * 0.5f;
}

bool Torch::CollideDetection(std::shared_ptr<Character> &character) {
    int frameIndex = character->m_Behavior->GetCurrentFrameIndex();
    int whipLevel = character->GetWhipLevel();
    UpdatePosition();
    if (character->IfWhip() && !is_destroyed) {
        bool isNormalWhip = (whipLevel != 3 && (frameIndex == 2 || frameIndex == 3));
        bool isLv3Whip = (whipLevel == 3 && frameIndex >= 8 && frameIndex <= 15);
        if (isNormalWhip || isLv3Whip) {
            float whipWidth = (whipLevel == 3) ? abs(character->OffsetValues("whipWidth_lv3")) : abs(character->OffsetValues("whipWidth"));
            glm::vec2 charPos = character->GetPosition();
            glm::vec2 charSize = character->GetSize();
            float charLeft = charPos.x - charSize.x * 0.5f;
            float charRight = charPos.x + charSize.x * 0.5f;
            float whipLeft = (character->GetDirection() == "left") ? charLeft - whipWidth : charRight;
            float whipRight = (character->GetDirection() == "left") ? charLeft : charRight + whipWidth;

            bool overlapX = (whipLeft > torchLeft && whipLeft < torchRight) ||
                            (whipRight > torchLeft && whipRight < torchRight) ||
                            (whipLeft < torchLeft && whipRight > torchRight);
            bool overlapY = torchTop > charPos.y && torchBottom < charPos.y;

            if (overlapX && overlapY) {
                SetPaused();
                is_destroyed = true;
            }
        }
    }
    return is_destroyed;
}

void Torch::Destroy(App* app) {
    if (IfAnimationStart())
        SetAnimationFrames(torchDeath, 120);
    SetPlaying();
    if (IfPlayingTime(0.5)) {
        SetPaused();
        SetVisible(false);
        if (!loot) {
            loot = CreateLoot(itemType, GetPosition());
            app->m_Root.AddChild(loot);
        }
    }
}

std::shared_ptr<Loot> Torch::CreateLoot(LootType itemType, glm::vec2 position) {
    switch (itemType) {
        // Sub weapon
        case LootType::Axe:
            return std::make_shared<Subweapon::Axe>(position);
        case LootType::Dagger:
            return std::make_shared<Subweapon::Dagger>(position);
        case LootType::HolyWater:
            return std::make_shared<Subweapon::HolyWater>(position);
        case LootType::Stopwatch:
            return std::make_shared<Subweapon::Stopwatch>(position);

        // Whip
        case LootType::Whip:
            return std::make_shared<LootItem::Whip>(position);

        // Bags
        case LootType::RedBag:
            return std::make_shared<LootItem::Bag>(position, LootType::RedBag);
        case LootType::WhiteBag:
            return std::make_shared<LootItem::Bag>(position, LootType::WhiteBag);
        case LootType::PurpleBag:
            return std::make_shared<LootItem::Bag>(position, LootType::PurpleBag);
        case LootType::SpecialBag:
            return std::make_shared<LootItem::Bag>(position, LootType::SpecialBag);

        // Items
        case LootType::HeartSmall:
            return std::make_shared<LootItem::Heart>(position, LootType::HeartSmall);
        case LootType::HeartBig:
            return std::make_shared<LootItem::Heart>(position, LootType::HeartBig);
        default:
            return nullptr;
    }
}

void Torch::Update(App* app, std::shared_ptr<Character> character, std::shared_ptr<Menu> menu, const std::vector<std::shared_ptr<Block>>& blocks, std::shared_ptr<Torch> torch) {
    if (loot && !loot->IfCollected()) {
        loot->Fall(blocks);
        if (loot->IsCollected(character))
            app->m_Root.RemoveChild(torch);
    }else if (loot && loot->IfCollected() && !loot->IfEnded()) {
        loot->Result(app, character, menu);
        if (loot->IfEnded())
            app->m_Root.RemoveChild(loot);
    }
    if (CollideDetection(character) && !loot)
        Destroy(app);
}