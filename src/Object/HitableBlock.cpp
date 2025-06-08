#include "Object/HitableBlock.hpp"
#include "Utility/Time.hpp"

HitableBlock::HitableBlock(LootType itemType) : ImageItems(GA_RESOURCE_DIR"/fonts/Menu/weapon-frame.png"), itemType(itemType) {}

void HitableBlock::AddBlock(std::shared_ptr<Block> &block, LootType loot, std::vector<std::shared_ptr<Util::GameObject>> &m_All) {
    if (block == nullptr) return;
    block->SetZIndex(6);
    blockLootPair.push_back(std::make_pair(block, loot));
    m_All.push_back(block);
    UpdatePosition();
    breakSound = std::make_shared<Util::SFX>(GA_RESOURCE_DIR "/Sound Effects/21.wav");
    breakSound->SetVolume(50);
}

void HitableBlock::UpdatePosition() {
    if (blockLootPair.empty()) return;
    glm::vec2 blockPos = glm::vec2(0.0f);
    glm::vec2 blockSize = glm::vec2(0.0f);
    for (const auto &pair : blockLootPair) {
        auto block = pair.first;
        blockPos += block->GetPosition();
        blockSize += glm::abs(block->GetScaledSize());
    }
    pos = blockPos / static_cast<float>(blockLootPair.size());
    width = blockSize.x / static_cast<float>(blockLootPair.size());
    height = blockSize.y / static_cast<float>(blockLootPair.size());
}

const glm::vec2& HitableBlock::GetPosition() const {
    return pos;
}

void HitableBlock::SetPosition(const glm::vec2 &position) {
    pos = position;
}

bool HitableBlock::CollideDetection(std::shared_ptr<Character> &character) {
    int frameIndex = character->m_Behavior->GetCurrentFrameIndex();
    int whipLevel = character->GetWhipLevel();
    blockLeft = pos.x - width * 0.5f;
    blockRight = pos.x + width * 0.5f;
    blockTop = pos.y + height * 0.5f;
    blockBottom = pos.y - height * 0.5f;
    if (character->IfWhip() && (Time::GetRunTimeMs(startDestroyedTime) > 500.0f || startDestroyedTime == 0)) {
        startDestroyedTime = 0;
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

            bool overlapX = (blockLeft < whipLeft && whipLeft < blockRight) ||
                            (blockLeft < whipRight && whipRight < blockRight) ||
                            (whipLeft - 3.0f < blockLeft && blockRight < whipRight);
            bool overlapY = blockTop + 1.5f > charPos.y && blockBottom - 1.5f < charPos.y;
            if (overlapX && overlapY) {
                startDestroyedTime = SDL_GetPerformanceCounter();
                breakSound->Play();
                return true;
            }
        }
    }
    return false;
}

void HitableBlock::RemoveBlock(App* app, std::vector<std::shared_ptr<Block>> &blocks, std::vector<std::shared_ptr<Util::GameObject>> &m_All) {
    if (blockLootPair.empty()) return;
    auto block = blockLootPair.front().first;
    LootType lootType = blockLootPair.front().second;
    if (lootType == itemType) {
        loot = Loot::CreateLoot(itemType, block->GetPosition());
        loot->SetPosition(block->GetPosition());
        app->m_Root.AddChild(loot);
    }
    blockLootPair.erase(blockLootPair.begin());
    app->m_Root.RemoveChild(block);
    m_All.erase(std::remove(m_All.begin(), m_All.end(), block), m_All.end());
    blocks.erase(std::remove(blocks.begin(), blocks.end(), block), blocks.end());
    SetParticles(app, block);
}

void HitableBlock::SetParticles(App* app, std::shared_ptr<Block> &block) {
    particles.clear();
    y_vel = 6.0f;
    auto blockPos = block->GetPosition();
    auto blockSize = glm::abs(block->GetScaledSize());
    std::vector<glm::vec2> particlePositions = {
        {blockPos.x - blockSize.x * 0.5f - blockSize.x, blockPos.y + blockSize.y * 0.5f + blockSize.y}, // top-left
        {blockPos.x - blockSize.x * 0.5f - blockSize.x, blockPos.y - blockSize.y * 0.5f + blockSize.y}, // bottom-left
        {blockPos.x + blockSize.x * 0.5f - blockSize.x, blockPos.y + blockSize.y * 0.5f + blockSize.y}, // top-right
        {blockPos.x + blockSize.x * 0.5f - blockSize.x, blockPos.y - blockSize.y * 0.5f + blockSize.y}  // bottom-right
    };
    x_vel = { -1.5f, 2.5f, 1.5f, 3.0f };
    for (int i = 0; i < 4; i++) {
        auto particle = std::make_shared<ImageItems>(GA_RESOURCE_DIR"/items/scatter/scatter.png");
        particle->m_Transform.scale = glm::vec2(0.9f, 0.9f);
        particle->SetPosition(particlePositions[i]);
        particle->SetZIndex(7);
        particles.push_back(particle);
        app->m_Root.AddChild(particle);
    }
}

void HitableBlock::ScatterParticles(App *app) {
    if (particles.empty()) return;
    for (size_t i = 0; i < particles.size(); i++) {
        glm::vec2 pos = particles[i]->GetPosition();
        pos.x += x_vel[i];
        pos.y += y_vel;
        particles[i]->SetPosition(pos);
    }
    y_vel -= 0.6f;
    if (y_vel < -12.0f) {
        for (auto &particle : particles) {
            particle->SetVisible(false);
            app->m_Root.RemoveChild(particle);
        }
        particles.clear();
    }
}