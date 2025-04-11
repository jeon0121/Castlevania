#ifndef STAGE1_HPP
#define STAGE1_HPP
#include "Scene.hpp"

#include "Object/AnimatedItems.hpp"

class Stage1 : public Scene{
public:
    void Start(App*) override;
    void Update(App*) override;
    void End(App*) override;

private:
    int mapWidth = 6144;
};
#endif

