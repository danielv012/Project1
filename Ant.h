#ifndef ANT_H
#define ANT_H
#include "Creature.h"

class Ant : public Creature
{
public:
    Ant();
    Ant(int a);

    Ant(const Ant&);
    ~Ant();

    std::string Move(int[4]) const override;
    std::string Breed(int[4]) const override;

    int getAge() {return age;}
    void setAge(int a) {age = a;}

    Ant& operator=(const Ant& obj)
    {
        if(this != &obj)
        {
            this->age = obj.age;
            return *this;
        }
        return *this;
    }

private:
    int age;
};

#endif