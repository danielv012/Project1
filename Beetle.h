#ifndef BEETLE_H
#define BEETLE_H
#include "Creature.h"

class Beetle : public Creature
{
public:
    Beetle();
    Beetle(int, int);

    Beetle(const Beetle&);
    ~Beetle();

    std::string Move(int[4]) const override;
    std::string Breed(int[4]) const override;
    bool Starve() const;

    int getAge() {return age;}
    void setAge(int a) {age = a;}

    int getTimer() {return timer;}
    void setTimer(int t) {timer = t;}

    Beetle& operator=(const Beetle& obj) //copy assignment operator, makes a deep copy when assigned
    {
        if(this != &obj)
        {
            (*this).age = obj.age;
            (*this).timer = obj.timer;
            return *this;
        }
        return *this;
    }

    void operator++()
    {
        this->setTimer(this->getTimer() + 1); //increases the timer for a beetle object by 1
    }

private:
    int age;
    int timer;
};

#endif // BEETLE_H
