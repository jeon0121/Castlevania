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
    void HiddenItem(App* app);
private:
    int mapWidth = 3072;
    Uint64 hiddenLootTime = 0;
    bool is_endSound = false;
    std::shared_ptr<ImageItems> m_End;
};
#endif

