#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "App.hpp"
#include "Object/Enemy.hpp"
#include "Object/Character.hpp"
#include "Object/Block.hpp"
#include "Object/Enemies/Zombie.hpp"
#include "Object/Enemies/Leopard.hpp"

class EnemiesManager {
    public:
        EnemiesManager(App *app);

        void Update(float offsetX, int screenWidth, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, std::shared_ptr<Menu> &menu);

        void ManageZombies(float offsetX, std::shared_ptr<Character> &character);

        void ManageLeopard(float offsetX, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, int screenWidth);

        std::vector<std::shared_ptr<Enemy>> m_Enemies;
    private:
        Uint64 resetStartTime = 0;
        std::vector<std::shared_ptr<Zombie>> m_Zombies;
        std::vector<std::shared_ptr<Leopard>> m_Leopards;
};

#endif