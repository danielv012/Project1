#include "Ant.h"

Ant::Ant()
{
    age = 0;
}

Ant::Ant(int a)
{
    setAge(a);
}

Ant::Ant(const Ant &obj)
{
    this->age = obj.age;
}

Ant::~Ant()
{
    // destructor executes
}

/**
 * @brief given distances of beetles, returns the direction the ant should travel, depending on nearest beetle.
 * The functino also takes care of cases such as ties and no beetles.
 * 
 * @param distances array of distances to the nearest beetle, negative if there is no orthogonal beetle found.
 * Abs of negative value is the distance to the edge of the grid in that direction.
 * 
 * [0] - N (North)
 * [1] - E (East)
 * [2] - S (South)
 * [3] - W (West)
 * 
 * @return char 
 */
std::string Ant::Move(int distances[4]) const
{
    std::string nearest_beetle = "";
    int nearest_distance = 10;
    
    std::string farthest_beetle = "";
    int farthest_distance = 0;
    
    std::string no_beetle = "";
    
    std::string output = "";
    
    int beetle_amount = 0; // walls are also named beetles because the ant cannot travel towards
    
    for(int i = 0; i < 4; i++)
    {
        int distance = distances[i];
        
        if(distance > 0) // if there is a beetle found in the direction
        {
            beetle_amount++; // increase amount of beetle found
            if(distance < nearest_distance) // if this beetle is closest
            {
                nearest_distance = distance; // change nearest beetle distance to current beetle
                switch (i)
                {
                    case 0:
                        nearest_beetle = "N";
                        break;
                    case 1:
                        nearest_beetle = "E";
                        break;
                    case 2:
                        nearest_beetle = "S";
                        break;
                    case 3:
                        nearest_beetle = "W";
                        break;
                    default:
                        break;
                }
            }
            else if(distance == nearest_distance) //if there is a beetle tie
            {
                switch (i) //add beetle direction to the string of beetle directions
                {
                    case 0:
                        nearest_beetle += "N";
                        break;
                    case 1:
                        nearest_beetle += "E";
                        break;
                    case 2:
                        nearest_beetle += "S";
                        break;
                    case 3:
                        nearest_beetle += "W";
                        break;
                    default:
                        break;
                }
            }
            
            if(distance > farthest_distance) //if this beetle is farthest
            {
                farthest_distance = distance; //far beetle distance becomes new farthest distance
                
                switch (i)
                {
                    case 0:
                        farthest_beetle = "N";
                        break;
                    case 1:
                        farthest_beetle = "E";
                        break;
                    case 2:
                        farthest_beetle = "S";
                        break;
                    case 3:
                        farthest_beetle = "W";
                        break;
                    default:
                        break;
                }
            }
            else if(distance == farthest_distance) //if there's a tie
            {
                switch (i) //add far beetle direction to a string of beetle directions
                {
                    case 0:
                        farthest_beetle += "N";
                        break;
                    case 1:
                        farthest_beetle += "E";
                        break;
                    case 2:
                        farthest_beetle += "S";
                        break;
                    case 3:
                        farthest_beetle += "W";
                        break;
                    default:
                        break;
                }
            }
        }
        else if(distance < 0) // if there is no beetle
        {
            switch (i) //set move direction to the direction of no beetle
            {
                case 0:
                    no_beetle += "N";
                    break;
                case 1:
                    no_beetle += "E";
                    break;
                case 2:
                    no_beetle += "S";
                    break;
                case 3:
                    no_beetle += "W";
                    break;
                default:
                    break;
            }
        }
    }
    
    if(beetle_amount == 0) //if there are no beetles
    {
        output = ""; //stand still
    }
    else if(beetle_amount == 4) //if there are beetles in all directions
    {
        output = farthest_beetle; //move towards farthest eetle
    }
    else if(beetle_amount == 3) //if there are 3 beetles
    {
        output = no_beetle; //move towards direction of no beetle
    }
    else if(nearest_beetle.size() > 1) //if there is a tie for nearest beetles
    {
        if(nearest_beetle.size() <= 3) //move towards direction of no beetle
        {
            output = no_beetle;
        }
        else if(nearest_beetle.size() == 4) // if there is a 4 way tie, stand still
        {
            output = "";
        }
    }
    else if (nearest_beetle.size() == 1) //if there is one beetle
    {
        switch(nearest_beetle[0]) //move away from it by setting the character to be opposite
        {
            case 'N':
                output = "S";
                break;
            case 'E':
                output = "W";
                break;
            case 'S':
                output = "N";
                break;
            case 'W':
                output = "E";
                break;
        }
    }
    return output;
}

/**
 * @brief Takes the distances of beetles from the ant in all 4 directions.
 * If there is not an adjacent beetle in an orthogonal direction, a character
 * indicating the directions vacancy for a new ant is added to the emptySpace string.
 * 
 * @param distances distances for each cardinal direction
 * @return std::string 
 */
std::string Ant::Breed(int distances[4]) const
{
    std::string emptySpaces = ""; //string of chars representing the adjacent ortho directions without a beetle
    for(int i = 0; i < 4; i++)
    {
        if(distances[i] > 1 || distances[i] < 0) //if the adjacent space is empty
        {
            switch (i) //set empty space to the direction
            {
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
        if(emptySpaces.size() == 1) break; //once a viable space has been found, leave loop
    }
    return emptySpaces;
}
