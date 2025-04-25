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
        EnemiesManager(const std::vector<PossibleLootData>& possibleLoots);

        void AddZombie(glm::vec2 range, int numZombie, glm::vec2 pos, std::string direction, App *app);

        void AddLeopard(glm::vec2 positions, std::string direction, App *app);

        void Update(float offsetX, int screenWidth, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, App* app);

        void PauseAllEnemy();

        void PlayAllEnemy();

        void ManageZombies(float offsetX, std::shared_ptr<Character> &character, int screenWidth);

        void ManageLeopard(float offsetX, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, int screenWidth);

        void RemoveAllEnemies(App *app);

        std::vector<std::shared_ptr<Enemy>> m_Enemies;

        std::vector<std::shared_ptr<Loot>> m_Loots;
    private:
        Uint64 resetStartTime = 0;
        std::vector<std::shared_ptr<ZombieHorde>> m_Zombies;
        std::vector<std::shared_ptr<Leopard>> m_Leopards;
        std::vector<PossibleLootData> m_PossibleLoots;

        bool isEnemyPause = false;
};

#endif