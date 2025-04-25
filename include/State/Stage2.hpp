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
    int mapWidth = 2048;
};

class Stage2b : public Scene{
public:
    void Start(App* app) override;
    void Update(App* app) override;
    void End(App* app) override;

private:
    int mapWidth = 2048;
};
#endif

