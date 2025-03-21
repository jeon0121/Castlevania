#ifndef STAGE0_HPP
#define STAGE0_HPP
#include "Scene.hpp"
#include "App.hpp"

#include "Object/AnimatedItems.hpp"

class Stage0 : public Scene{
public:
    void Start(App* app) override;
    void Update(App* app) override;
    void End(App* app) override;

private:

};
#endif

