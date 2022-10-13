#include "Beetle.h"

Beetle::Beetle()
{
    age = 0;
    timer = 0;
}

Beetle::Beetle(int a, int t)
{
    age = a;
    timer = t;

}

Beetle::Beetle(const Beetle &obj)
{
    this->age = obj.age;
    this->timer = obj.timer;
}

Beetle::~Beetle()
{
    // destructor executes
}

/**
 * @brief takes distances of ants in each orthogonal directions, returns direction of nearest ant(s)
 * doesn't check for a tie in nearest ants, as a solution to a tie requires access to the grid
 * 
 * @param distances array of int distances for 4 cardinal directions
 * @return std::string of characters representing cardinal directions of nearby ants or nearest ant
 */
std::string Beetle::Move(int distances[4]) const
{
    std::string nearest_ant = "";
    int nearest_distance = 10;
    
    std::string farthest_edge = "";
    int farthest_distance = 0;
    
    std::string output = "";
    
    int ant_amount = 0;
    
    for(int i = 0; i < 4; i++)
    {
        int distance = distances[i];
        
        if(distance > 0) // if there is an ant found in the direction
        {
            ant_amount++; // increase amount of ants found
            if(distance < nearest_distance) // if this ant is closest
            {
                nearest_distance = distance; // change nearest ant distance to current ant
                switch (i)
                {
                    case 0:
                        nearest_ant = "N";
                        break;
                    case 1:
                        nearest_ant = "E";
                        break;
                    case 2:
                        nearest_ant = "S";
                        break;
                    case 3:
                        nearest_ant = "W";
                        break;
                    default:
                        break;
                }
            }
            else if(distance == nearest_distance) // if there is a tie
            {
                switch (i) //add direction character to the collection of nearest ants
                {
                    case 0:
                        nearest_ant += "N";
                        break;
                    case 1:
                        nearest_ant += "E";
                        break;
                    case 2:
                        nearest_ant += "S";
                        break;
                    case 3:
                        nearest_ant += "W";
                        break;
                    default:
                        break;
                }
            }
        }
        else if(distance <= 0) // if there is no ant (edge)
        {
            if(distance < farthest_distance) //if the edge is farther than the current farthest edge
            {
                farthest_distance = distance; //new farthest edge
                switch (i) //add farthest edge direction
                {
                    case 0:
                        farthest_edge = "N";
                        break;
                    case 1:
                        farthest_edge = "E";
                        break;
                    case 2:
                        farthest_edge = "S";
                        break;
                    case 3:
                        farthest_edge = "W";
                        break;
                    default:
                        break;
                }
            }
            else if(distance == farthest_distance) //if there is a tie for farthest edge
            {
                switch (i) //add farthest edge direction to the string of farthest edges
                {
                    case 0:
                        farthest_edge += "N";
                        break;
                    case 1:
                        farthest_edge += "E";
                        break;
                    case 2:
                        farthest_edge += "S";
                        break;
                    case 3:
                        farthest_edge += "W";
                        break;
                    default:
                        break;
                }
            }
        }
    }
    
    if(ant_amount >= 1) //if there are ants
    {
        output = nearest_ant;
    }
    else //move towards farthest edge if there are no ants
    {
        output = farthest_edge;
    }
    
    return output;
}

/**
 * @brief Takes the distances of ants from the beetle in all 4 directions.
 * If there is not an adjacent ant in an orthogonal direction, a character
 * indicating the directions vacancy for a new beetle is added to the emptySpace string.
 * 
 * @param distances array of ints for each cardinal direction
 * @return std::string 
 */
std::string Beetle::Breed(int distances[4]) const
{
    std::string emptySpaces = ""; //string of chars representing the adjacent ortho directions without a beetle
    for(int i = 0; i < 4; i++)
    {
        if(distances[i] > 1 || distances[i] < 0) //if the adjacent space is empty
        {
            switch (i)
            { // add the direction to the list of viable spaces
            case 0:
                emptySpaces+='N';
                break;
            case 1:
                emptySpaces+='E';
                break;
            case 2:
                emptySpaces+='S';
                break;
            case 3:
                emptySpaces+='W';
                break;
            default:
                break;
            }
        }
        if(emptySpaces.size() == 1) break; //leave once a viable space is found
    }
    return emptySpaces;
}

bool Beetle::Starve() const
{
    if(this->timer == 5) //if the timer is 5, it's been 5 turns since the beetle has eaten, kill beetle
    {
        return true;
    }
    else return false;
}



