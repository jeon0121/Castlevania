#ifndef STAGE3_HPP
#define STAGE3_HPP
#include "Scene.hpp"

#include "Object/AnimatedItems.hpp"

class Stage3 : public Scene{
public:
    void Start(App*) override;
    void Update(App*) override;
    void End(App*) override;
    void BossFight(App*);
    void DropCrystal(App*);
private:
    int mapWidth = 3122;
    std::shared_ptr<Loot> m_crystal;
    Uint64 crystalTime = 0;
};
#endif

