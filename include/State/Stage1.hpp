#ifndef STAGE1_HPP
#define STAGE1_HPP
#include "Scene.hpp"
#include "App.hpp"

#include "Object/AnimatedItems.hpp"

class Stage1 : public Scene{
public:
    void Start(App* app) override;
    void Update(App* app) override;
    void End(App* app) override;

private:
    int mapWidth = 6144;
};
#endif

