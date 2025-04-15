#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "app.hpp"
#include "Object/Enemy.hpp"
#include "Object/Enemies/Zombie.hpp"

class EnemiesManager {
    public:
        EnemiesManager(App *app);

        void Update(float offsetX, int screenWidth, std::shared_ptr<Character> &character);

        void ManageZombies(float offsetX, std::shared_ptr<Character> &character);

        void ManageLeopard(float offsetX);

        std::vector<std::shared_ptr<Enemy>> m_Enemies;
    private:
        Uint64 resetStartTime = 0;
        std::vector<std::shared_ptr<Zombie>> m_Zombies;
};

#endif