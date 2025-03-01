#ifndef STAGE1_HPP
#define STAGE1_HPP
#include "Scene.hpp"

#include "Object/AnimatedItems.hpp"

class Stage1 : public Scene{
public:
    void Start(App* app) override;
    void Update() override;
    void End(App* app) override;

private:

};
#endif

