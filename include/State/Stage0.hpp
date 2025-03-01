#ifndef STAGE0_HPP
#define STAGE0_HPP
#include "Scene.hpp"

#include "Object/AnimatedItems.hpp"

class Stage0 : public Scene{
public:
    void Start() override;
    void Update() override;
    void End() override;

private:

};
#endif

