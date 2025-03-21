#include "Object/Torch.hpp"

Torch::Torch(glm::vec2 position, glm::vec2 scale, LootType itemType, int type)
    : AnimatedItems((type == 1) ? std::vector<std::string>{GA_RESOURCE_DIR "/items/fire/fire-1.png", GA_RESOURCE_DIR "/items/fire/fire-2.png"}
                                : std::vector<std::string>{GA_RESOURCE_DIR "/items/fire/fire-3.png", GA_RESOURCE_DIR "/items/fire/fire-4.png"},
    120) {
    SetPosition(position);
    m_Transform.scale = scale;
    SetPlaying();
    SetLooping(true);
    this->itemType = itemType;

    for (int i = 0; i < 4; ++i) {
        torchDeath.emplace_back(GA_RESOURCE_DIR"/items/fire/torch/torchDeath-" + std::to_string(i + 1) + ".png");
    }

    glm::vec2 torchPos = GetPosition();
    glm::vec2 torchSize = glm::abs(GetScaledSize());
    torchTop = torchPos.y + torchSize.y * 0.5f;
    torchBottom = torchPos.y - torchSize.y * 0.5f;
    torchLeft = torchPos.x - torchSize.x * 0.5f;
    torchRight = torchPos.x + torchSize.x * 0.5f;
}

bool Torch::CollideDetection(std::shared_ptr<Character> &character) {
    int frameIndex = character->m_Behavior->GetCurrentFrameIndex();
    if (character->IfWhip() && (frameIndex == 2 || frameIndex == 3) && !is_destroyed) {
        float whipWidth = abs(character->OffsetValues("whipWidth"));
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
        case LootType::Axe:
            return std::make_shared<LootItem::Axe>(position);
        // case LootType::Dagger:
        //     return std::make_shared<Dagger>();
        // case LootType::HolyWater:
        //     return std::make_shared<HolyWater>();
        // case LootType::Heart:
        //     return std::make_shared<Heart>();
        default:
            return nullptr;
    }
}

void Torch::Update(App* app, std::shared_ptr<Character> character, std::shared_ptr<Menu> menu, const std::vector<std::shared_ptr<Block>>& blocks, std::shared_ptr<Torch> torch) {
    if (loot) 
        loot->Fall(blocks);
    if (CollideDetection(character)) 
        Destroy(app);
    if (loot && loot->IsCollected(character, menu)) {
        app->m_Root.RemoveChild(torch);
        app->m_Root.RemoveChild(loot);
    }
}