#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Object/AnimatedItems.hpp"
#include "Object/Character.hpp"
#include "Object/Block.hpp"
#include "State/Menu.hpp"
#include "Object/Loot.hpp"
#include "Object/ObjectData.hpp"

class Enemy : public AnimatedItems {
    public:
        Enemy(glm::vec2 position, std::string direction, std::vector<std::string> animationPath, int interval, int score);

        virtual void MoveBehav() {}

        virtual void MoveBehav(std::shared_ptr<Character> &character) {
            (void) character;
        }

        virtual void MoveBehav(std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks) {
            (void) character;
            (void) blocks;
        }

        virtual void SetReset() = 0;

        void Death(App* app, std::vector<std::shared_ptr<Loot>> &m_Loots, std::vector<PossibleLootData> &m_PossibleLoots);

        void Flip();

        void UpdatePosition();

        bool CollideDetection(std::shared_ptr<Character> &character, std::shared_ptr<Menu> &menu);

        virtual void InWindowDetection(int screenWidth);

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
        LootType lootType = LootType::None;
    private:
        float enemyTop;
        float enemyBottom;
        float enemyLeft;
        float enemyRight;
};


#endif