#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Object/AnimatedItems.hpp"
#include "Object/Character.hpp"
#include "Object/Block.hpp"
#include "State/Menu.hpp"
#include "Object/Loot.hpp"
#include "Object/ObjectData.hpp"
#include <Util/SFX.hpp>

class Enemy : public AnimatedItems {
    public:
        Enemy(glm::vec2 position, std::string direction, std::vector<std::string> animationPath, int interval, int score);

        virtual void MoveBehav() {}

        virtual void MoveBehav(std::vector<std::shared_ptr<Block>> &blocks) { (void) blocks; }

        virtual void MoveBehav(std::shared_ptr<Character> &character) { (void) character; }

        virtual void MoveBehav(std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks) { (void) character; (void) blocks; }
        
        virtual void MoveBehav(std::shared_ptr<Character> &character, int screenHeight, int screenWidth) { (void) character; (void) screenHeight; (void) screenWidth; }

        virtual void SetReset() = 0;

        void SetDirection(std::string direction);

        std::string GetDirection();

        void Death(App* app, std::vector<std::shared_ptr<Loot>> &m_Loots, std::vector<PossibleLootData> &m_PossibleLoots, std::vector<std::shared_ptr<GameObject>> &rendererVec);

        void Flip();

        void UpdatePosition();

        virtual bool CollideDetection(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu, int modeState);

        virtual void InWindowDetection(int screenWidth, int offsetX);

        void SetEnemyRange(glm::vec2 range);

        void SetHidden(bool hidden);

        void SetDead(bool dead);

        virtual bool CheckReset();

        bool IsDead();
        
    protected:
        std::string direction;
        std::vector<std::string> deathImages;
        bool is_dead = false;
        bool is_hidden = false;
        bool hasEnteredWindow = false;
        int countHurt = 0;
        int score = 0;
        float enemyTop;
        float enemyBottom;
        float enemyLeft;
        float enemyRight;
        glm::vec2 enemyRange = {-10000.0f, 10000.0f};
        LootType lootType = LootType::None;
        std::shared_ptr<Util::SFX> deadSound;
};


#endif