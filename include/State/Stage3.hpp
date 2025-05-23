#ifndef STAGE3_HPP
#define STAGE3_HPP
#include "Scene.hpp"

#include "Object/AnimatedItems.hpp"

class Stage3 : public Scene{
public:
    void Start(App*) override;
    void Update(App*) override;
    void End(App*) override;

private:
    int mapWidth = 3122;

};
#endif

