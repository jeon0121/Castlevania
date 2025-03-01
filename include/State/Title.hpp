#ifndef TITLE_HPP
#define TITLE_HPP
#include "Scene.hpp"

#include "Object/AnimatedItems.hpp"

class Title : public Scene{
public:
    void Start(App* app) override;
    void Update() override;
    void End(App* app);

private:
    std::shared_ptr<AnimatedItems> m_Bat;
};
#endif

