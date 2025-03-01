#ifndef STAGE2_HPP
#define STAGE2_HPP
#include "Scene.hpp"

#include "Object/AnimatedItems.hpp"

class Stage2 : public Scene{
public:
    void Start(App* app) override;
    void Update() override;
    void End(App* app) override;

private:

};
#endif

