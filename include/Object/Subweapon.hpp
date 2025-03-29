#ifndef SUBWEAPON_HPP
#define SUBWEAPON_HPP

class Subweapon : public AnimatedItems {
public:
   Subweapon();

   virtual void Use() = 0;
};

#endif