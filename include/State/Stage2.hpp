#ifndef STAGE2_HPP
#define STAGE2_HPP
#include "Scene.hpp"
#include "App.hpp"

#include "Object/AnimatedItems.hpp"

class Stage2a : public Scene{
public:
    void Start(App* app) override;
    void Update(App* app) override;
    void End(App* app) override;

private:
    int mapWidth = 2070;
    std::shared_ptr<AnimatedItems> door_1;
    std::shared_ptr<AnimatedItems> door_2;
};

class Stage2b : public Scene{
public:
    void Start(App* app) override;
    void Update(App* app) override;
    void End(App* app) override;
    bool Bubble();
    void HiddenItem(App* app);
private:
    int mapWidth = 2070;
    Uint64 hiddenLootTime = 0;
    std::vector<float> bbl_vel = {0, 12.0f, 0}; // bubble velocity
    std::vector<std::shared_ptr<ImageItems>> bubblesVector;
};
#endif

