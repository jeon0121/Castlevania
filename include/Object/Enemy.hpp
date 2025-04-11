#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Object/AnimatedItems.hpp"
#include "Object/Character.hpp"

class Enemy : public AnimatedItems {
    public:
        Enemy(glm::vec2 position, std::string direction, std::vector<std::string> animationPath, int interval);

        virtual void MoveBehav() = 0;

        virtual void SetReset() = 0;

        void Death();

        void Flip();

        void UpdatePosition();

        bool CollideDetection(std::shared_ptr<Character> &character);

        void InWindowDetection(int screenWidth);

        bool CheckReset();

    protected:
        std::string direction;
        std::vector<std::string> deathImages;
        bool is_dead = false;
        bool is_hidden = false;
        bool hasEnteredWindow = false;
    private:
        int countHurt = 0;
        float enemyTop;
        float enemyBottom;
        float enemyLeft;
        float enemyRight;
};


#endif