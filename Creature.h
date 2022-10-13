#ifndef CREATURE_H
#define CREATURE_H
#include <string>

class Creature
{
public:
    Creature(/* args */) {/* consturctor */};
    virtual ~Creature() {/* destructor*/};
    bool isMoved() {return moved;}
    void setMoved(bool val) {moved = val;}

    virtual std::string Move(int[4]) const 
    {
    return "";
    };
    virtual std::string Breed(int[4]) const
    {
    return "";
    };
private:
    bool moved;
};

#endif // CREATURE_H
