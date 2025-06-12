#ifndef ENEMYMANAGER_H
#define ENEMYMANAGER_H

#include "App.hpp"
#include "Object/Enemy.hpp"
#include "Object/Character.hpp"
#include "Object/Block.hpp"
#include "Object/Boss/PhantomBat.hpp"
#include "Object/Enemies/Zombie.hpp"
#include "Object/Enemies/Leopard.hpp"
#include "Object/Enemies/Bat.hpp"
#include "Object/Enemies/Fishman.hpp"

class EnemiesManager {
    public:
        EnemiesManager(const std::vector<PossibleLootData>& possibleLoots);

        void AddZombie(glm::vec2 range, int numZombie, glm::vec2 pos, std::string direction, App *app);

        void AddLeopard(glm::vec2 positions, std::string direction, App *app);

        void AddBat(glm::vec2 positions, std::string direction, App *app);

        void AddFishman(glm::vec2 positions, std::string direction, App *app);

        void Update(float offsetX, int screenWidth, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, App* app);

        void PauseAllEnemy();

        void PlayAllEnemy();

        void SetEnemyRange(glm::vec2 range);

        void ManageZombies(float offsetX, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, int screenWidth);

        void ManageLeopard(float offsetX, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, int screenWidth);

        void ManageBat(float offsetX, std::shared_ptr<Character> &character, int screenWidth);

        void ManageFishman(App *app, std::shared_ptr<Character> &character, std::vector<std::shared_ptr<Block>> &blocks, int screenWidth);

        void FireAttack(std::shared_ptr<Character> &character, int screenWidth, std::shared_ptr<Menu> &menu);

        void RemoveAllChild(App *app);

        void AddAllChild(App *app);

        std::vector<std::shared_ptr<Enemy>> m_Enemies;

        std::vector<std::shared_ptr<Loot>> m_Loots;

        std::vector<std::shared_ptr<Util::GameObject>> rendererVec;
    private:
        Uint64 resetStartTime = 0;
        std::vector<std::shared_ptr<ZombieHorde>> m_Zombies;
        std::vector<std::shared_ptr<Leopard>> m_Leopards;
        std::vector<std::shared_ptr<Bat>> m_Bats;
        std::vector<std::shared_ptr<Fishman>> m_Fishmans;
        std::vector<PossibleLootData> m_PossibleLoots;
        std::vector<std::shared_ptr<ImageItems>> fires;
        std::vector<bool> isFire = {false, false};
        std::vector<std::string> fireDirections = {"right", "right"};
        std::shared_ptr<Util::SFX> stopWatchSound;
        bool isEnemyPause = false;
};

#endif