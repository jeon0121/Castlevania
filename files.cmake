set(SRC_FILES
    App.cpp

    Object/Enemies/Bat.cpp
    Object/Enemies/Zombie.cpp
    Object/Enemies/Leopard.cpp
    Object/Enemies/Fishman.cpp

    Object/LootType/Item/Bag.cpp
    Object/LootType/Item/Chicken.cpp
    Object/LootType/Item/Crystal.cpp
    Object/LootType/Item/Heart.cpp
    Object/LootType/Item/Rosary.cpp
    Object/Subweapon/Axe.cpp
    Object/Subweapon/HolyWater.cpp
    Object/Subweapon/Dagger.cpp
    Object/Subweapon/Stopwatch.cpp
    Object/LootType/Whip/Whip.cpp
    Object/AnimatedItems.cpp
    Object/Block.cpp
    Object/Character.cpp
    Object/EnemiesManager.cpp
    Object/Enemy.cpp
    Object/HitableBlock.cpp
    Object/Loot.cpp
    Object/Stair.cpp
    Object/Torch.cpp

    State/Menu.cpp
    State/Scene.cpp
    State/Title.cpp
    State/Stage0.cpp
    State/Stage1.cpp
    State/Stage2a.cpp
    State/Stage2b.cpp
    State/Stage3.cpp

    Utility/Time.cpp
    Utility/Position.cpp
)

set(INCLUDE_FILES
    App.hpp
    State.hpp

    Object/Enemies/Bat.hpp
    Object/Enemies/Zombie.hpp
    Object/Enemies/Leopard.hpp
    Object/Enemies/Fishman.hpp

    Object/LootType/Loot.hpp
    Object/LootType/Item/Bag.hpp
    Object/LootType/Item/Chicken.hpp
    Object/LootType/Item/Crystal.hpp
    Object/LootType/Item/Heart.hpp
    Object/LootType/Item/Pot.hpp
    Object/LootType/Item/Rosary.hpp
    Object/Subweapon/Axe.hpp
    Object/Subweapon/HolyWater.hpp
    Object/Subweapon/Dagger.hpp
    Object/Subweapon/Stopwatch.hpp
    Object/Subweapon/IUseSubweapon.hpp
    Object/LootType/Whip/Whip.hpp
    
    Object/AnimatedItems.hpp
    Object/Block.hpp
    Object/Character.hpp
    Object/EnemiesManager.hpp
    Object/Enemy.hpp
    Object/HitableBlock.hpp
    Object/ImageItems.hpp
    Object/Loot.hpp
    Object/ObjectData.hpp
    Object/Stair.hpp
    Object/Torch.hpp

    State/Menu.hpp
    State/Scene.hpp
    State/Title.hpp
    State/Stage0.hpp
    State/Stage1.hpp

    Utility/Time.hpp
    Utility/Position.hpp
)

set(TEST_FILES
)
