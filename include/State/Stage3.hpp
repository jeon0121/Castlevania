#ifndef STAGE3_HPP
#define STAGE3_HPP
#include "Scene.hpp"

#include "Object/AnimatedItems.hpp"

class Stage3 : public Scene{
public:
    void Start(App* app) override;
    void Update() override;
    void End(App* app) override;

private:

};
#endif

