#ifndef HITABLE_BLOCK_HPP
#define HITABLE_BLOCK_HPP

#include "Object/Block.hpp"
#include "Object/Character.hpp"
#include "Object/Loot.hpp"
#include "App.hpp"

class HitableBlock : public ImageItems {
    public:
        HitableBlock(LootType itemType);
        void AddBlock(std::shared_ptr<Block> &block, LootType loot, std::vector<std::shared_ptr<Util::GameObject>> &m_All);
        void UpdatePosition();
        const glm::vec2& GetPosition() const override;
        void SetPosition(const glm::vec2 &position) override;
        bool CollideDetection(std::shared_ptr<Character> &character);
        void RemoveBlock(App* app, std::vector<std::shared_ptr<Block>> &blocks, std::vector<std::shared_ptr<Util::GameObject>> &m_All);
        void SetParticles(App* app, std::shared_ptr<Block> &block);
        void ScatterParticles(App* app);
        bool IsDestroyed() const;
        std::vector<std::pair<std::shared_ptr<Block>, LootType>> blockLootPair;
        std::vector<std::shared_ptr<ImageItems>> particles;
        std::shared_ptr<Loot> loot = nullptr;
        
    private:
        glm::vec2 pos;
        float width;
        float height;
        float blockLeft;
        float blockRight;
        float blockTop;
        float blockBottom;
        LootType itemType;
        float y_vel = 0;
        std::vector<float> x_vel;
        Uint64 startDestroyedTime = 0;
        std::shared_ptr<Util::SFX> breakSound;
};

#endif