/*
 Name: Daniel Vayman
 NetID: dav21004
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Creature.h"
#include "Ant.h"
#include "Beetle.h"

using namespace std;

const int SIZE_X = 10;
const int SIZE_Y = 10;

void populateGrid(Creature* grid[SIZE_X][SIZE_Y], string);
void printGrid(Creature* grid[SIZE_X][SIZE_Y], char, char);
void antDistances(Creature* grid[SIZE_X][SIZE_Y], int, int, int[4]);
void beetleDistances(Creature* grid[SIZE_X][SIZE_Y], int, int, int[4]);
void beetleMove(Creature* grid[SIZE_X][SIZE_Y], int, int, int[4]);
void runTurn(Creature* grid[SIZE_X][SIZE_Y], int);
void antMove(Creature* grid[SIZE_X][SIZE_Y], int, int, int[4]);
int checkAntNeighbors(Creature* grid[SIZE_X][SIZE_Y], int, int);
void resetCreatures(Creature* grid[SIZE_X][SIZE_Y]);
void antBreed(Creature* grid[SIZE_X][SIZE_Y], int, int , int[4]);
void creatureDistances(Creature* grid[SIZE_X][SIZE_Y], int, int, int[4]);
void beetleBreed(Creature* grid[SIZE_X][SIZE_Y], int, int , int[4]);

int main()
{
    string fileName = "";
    char antChar = 'a';
    char beetleChar = 'b';
    int turns;

    // inputs file name, ant output character, beetle output character, and the number of turns.
    cout << "What is the filename? ";
    cin >> fileName;
    cout << "Please enter a character to represent ants: ";
    cin >> antChar;
    cout << "Please enter a character to represent beetles: ";
    cin >> beetleChar;
    cout << "How many turns to execute? ";
    cin >> turns;
    cout << endl;
    
    Creature* grid[SIZE_X][SIZE_Y];

    populateGrid(grid, fileName); // fills the grid with Creature objects (beetle, ants)

    for(int i = 1; i <= turns; i++) // for each turn
    {
        cout << "TURN " << i << endl;
        runTurn(grid, i); // runs the turn, including each phase
        printGrid(grid, antChar, beetleChar); // prints the grid using the output characters provided
        resetCreatures(grid); // set all the creatures isMoved values back to zero so they can once again be affected
    }

    return 0;
}

void runTurn(Creature* grid[SIZE_X][SIZE_Y], int turn)
{
    // Beetle run - moves beetles
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            Beetle* tempBeetle = dynamic_cast<Beetle*>(grid[i][j]);
            if(tempBeetle && !tempBeetle->isMoved()) // if the grid object is a beetle and it hasn't been moved
            {
                (*tempBeetle).operator++(); // increase the starve timer on the beetle by 1
                int distances[4]; // 4 cardinal directions, represents distances of ants
                antDistances(grid, i, j, distances); // returns distances of ants in each 4 direction
                beetleMove(grid, i, j, distances); // moves the beetle to the correct space
            }
        }
    }

    // Ant run
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            Ant* tempAnt = dynamic_cast<Ant*>(grid[i][j]);
            if(tempAnt && !tempAnt->isMoved()) // if the grid object is an ant and it hasn't been moved
            {
            int distances[4];
            beetleDistances(grid, i, j, distances); // returns distances of beetles in each 4 direction
            antMove(grid, i, j, distances); // moves the ant to the correct space
            }
        }
    }

    // beetle Starve
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            Beetle* tempBeetle = dynamic_cast<Beetle*>(grid[i][j]);
            if(tempBeetle) //if it's a beetle object
            {
                if(tempBeetle->Starve()) // returns true if the timer is 5, which then the beetle starves
                {
                    delete tempBeetle;
                    grid[i][j] = nullptr; // deletes the beetle off the grid
                }
            }
        }
    }
    
    resetCreatures(grid); // sets objects isMoved to false, allows them to be affected
    // ant breed
    if(turn % 3 == 0)
    {
        for(int i = 0; i < 10; i++)
        {
            for(int j = 0; j < 10; j++)
            {
                Ant* tempAnt = dynamic_cast<Ant*>(grid[i][j]);
                if(tempAnt && !tempAnt->isMoved()) // if it's an ant and it hasn't been affected
                {
                    // breed
                    int distances[4];
                    creatureDistances(grid, i, j, distances); // returns the distances of nearby creatures
                    antBreed(grid, i, j, distances); // takes tese distances and creates an ant object nearby
                }
            }
        }
    }
    
    // beetle breed
    if(turn % 8 == 0)
    {
        for(int i = 0; i < 10; i++)
        {
            for(int j = 0; j < 10; j++)
            {
                Beetle* tempBeetle = dynamic_cast<Beetle*>(grid[i][j]);
                if(tempBeetle && !tempBeetle->isMoved()) // if the object is a beetle and it's able to be affected
                {
                    // breed
                    int distances[4];
                    creatureDistances(grid, i, j, distances); // returns the distances of nearby creatures
                    beetleBreed(grid, i, j, distances); // creates a beetle object nearby depending on returned distances
                }
            }
        }
    }
    
    // ant Breed every 3 interval
    // beetle Breed every 8 interval
}

void antDistances(Creature* grid[SIZE_X][SIZE_Y], int x, int y, int distances[4])
{
    int i = x;
    int j = y;

    //N orthogonal check
    while(j >= 0)
    {
        j--;
        if(j < 0)
        {
            distances[0] = y*-1;
        }
        else if (dynamic_cast<Ant*>(grid[i][j])) //if the object is an ant
        {
            distances[0] = y - j; //set distance
            break;
        }
    }
    j = y;

    //E orthogonal check
    while(i <= 9)
    {
        i++;
        if(i > 9)
        {
            distances[1] = (9-x)*-1;
        }
        else if (dynamic_cast<Ant*>(grid[i][j])) //if the object is an ant
        {
            distances[1] = i - x; //set distance
            break;
        }
    }
    i = x;

    //S orthogonal check
    while(j <= 9)
    {
        j++;
        if(j > 9)
        {
            distances[2] = (9-y)*-1;
        }
        else if (dynamic_cast<Ant*>(grid[i][j])) //if the object is an ant
        {
            distances[2] = j - y; //set distance
            break;
        }
    }
    j = y;
    
    //W orthogonal check
    while(i >= 0)
    {
        i--;
        if(i < 0)
        {
            distances[3] = x*-1;
        }
        else if (dynamic_cast<Ant*>(grid[i][j])) //if the object is an ant
        {
            distances[3] = x - i; //set distance
            break;
        }
    }
}

void beetleDistances(Creature* grid[SIZE_X][SIZE_Y], int x, int y, int distances[4])
{
    int i = x;
    int j = y;

    //N orthogonal check
    while(j >= 0)
    {
        j--;
        if(j < 0)
        {
            distances[0] = y*-1;
        }
        else if (dynamic_cast<Beetle*>(grid[i][j])) //if the object is a beetle
        {
            distances[0] = y - j; //set distance
            break;
        }
    }
    j = y;

    //E orthogonal check
    while(i <= 9)
    {
        i++;
        if(i > 9)
        {
            distances[1] = (9-x)*-1;
        }
        else if (dynamic_cast<Beetle*>(grid[i][j])) //if the object is a beetle
        {
            distances[1] = i - x; //set distance
            break;
        }
    }
    i = x;
    
    //S orthogonal check
    while(j <= 9)
    {
        j++;
        if(j > 9)
        {
            distances[2] = (9-y)*-1;
        }
        else if (dynamic_cast<Beetle*>(grid[i][j])) //if the object is a beetle
        {
            distances[2] = j - y; //set distance
            break;
        }
    }
    j = y;
    
    //W orthogonal check
    while(i >= 0)
    {
        i--;
        if(i < 0)
        {
            distances[3] = x*-1;
        }
        else if (dynamic_cast<Beetle*>(grid[i][j])) //if the object is an ant
        {
            distances[3] = x - i; //set distance
            break;
        }
    }
}

void beetleMove(Creature* grid[SIZE_X][SIZE_Y], int x, int y, int distances[4])
{
    string output = grid[x][y]->Move(distances); //return a string of characters representing viable directions
    
    if(output.size() > 1) //if there is a tie for directions, check for ant with most neighbors
    {
        int maxNeighbors = 0;
        char bestAnt = output[0];
        
        for(char c : output) //for each direction
        {
            int tempNeighbors = 0;
            
            switch (c)
            {
                case 'N': // if it's north
                    tempNeighbors = checkAntNeighbors(grid, x, y - distances[0]); //get the amount of neighbors for that ant
                    if(tempNeighbors > maxNeighbors) //if this ant has the most neighbors
                    {
                        maxNeighbors = tempNeighbors; //set current ant neighbors to the max
                        bestAnt = 'N'; //prioritized ant becomes ant with most neighbors
                    }
                    break;
                case 'E':
                    tempNeighbors = checkAntNeighbors(grid, x + distances[1], y);
                    if(tempNeighbors > maxNeighbors)
                    {
                        maxNeighbors = tempNeighbors;
                        bestAnt = 'E';
                    }
                    break;
                case 'S':
                    tempNeighbors = checkAntNeighbors(grid, x, y + distances[2]);
                    if(tempNeighbors > maxNeighbors)
                    {
                        maxNeighbors = tempNeighbors;
                        bestAnt = 'S';
                    }
                    break;
                case 'W':
                    tempNeighbors = checkAntNeighbors(grid, x - distances[3], y);
                    if(tempNeighbors > maxNeighbors)
                    {
                        maxNeighbors = tempNeighbors;
                        bestAnt = 'W';
                    }
                    break;
                default:
                    break;
            }
        }
        output = bestAnt;
    }
    
    switch(output[0]) //move the beetle depending on the direction
    {
        case 'N':
            if(y == 0) break;
            if(!dynamic_cast<Creature*>(grid[x][y - 1])) //if the space above is open
            {
                grid[x][y]->setMoved(true);
                grid[x][y - 1] = grid[x][y]; //set the space above to point to the new object
                grid[x][y] = nullptr; //empty the previous space
            }
            else if(dynamic_cast<Ant*>(grid[x][y - 1])) //if the space above is an ant
            {
                dynamic_cast<Beetle*>(grid[x][y])->setTimer(0); //starve timer resets
                grid[x][y]->setMoved(true);
                delete grid[x][y - 1]; //deletes the ant
                grid[x][y - 1] = grid[x][y]; //beetle fills the ant space
                grid[x][y] = nullptr; //previous space is emptied
            }
            break;
        case 'E':
            if(x == 9) break;
            if(!dynamic_cast<Creature*>(grid[x + 1][y]))
            {
                grid[x][y]->setMoved(true);
                grid[x + 1][y] = grid[x][y];
                grid[x][y] = nullptr;
            }
            else if(dynamic_cast<Ant*>(grid[x + 1][y]))
            {
                dynamic_cast<Beetle*>(grid[x][y])->setTimer(0);
                grid[x][y]->setMoved(true);
                delete grid[x + 1][y];
                grid[x + 1][y] = grid[x][y];
                grid[x][y] = nullptr;
            }
            break;
        case 'S':
            if(y == 9) break;
            if(!dynamic_cast<Creature*>(grid[x][y + 1]))
            {
                grid[x][y]->setMoved(true);
                grid[x][y + 1] = grid[x][y];
                grid[x][y] = nullptr;
            }
            else if(dynamic_cast<Ant*>(grid[x][y + 1]))
            {
                dynamic_cast<Beetle*>(grid[x][y])->setTimer(0);
                grid[x][y]->setMoved(true);
                delete grid[x][y + 1];
                grid[x][y + 1] = grid[x][y];
                grid[x][y] = nullptr;
            }
            break;
        case 'W':
            if(x == 0) break;
            if(!dynamic_cast<Creature*>(grid[x - 1][y]))
            {
                grid[x][y]->setMoved(true);
                grid[x - 1][y] = grid[x][y];
                grid[x][y] = nullptr;
            }
            else if(dynamic_cast<Ant*>(grid[x - 1][y]))
            {
                dynamic_cast<Beetle*>(grid[x][y])->setTimer(0);
                grid[x][y]->setMoved(true);
                delete grid[x - 1][y];
                grid[x - 1][y] = grid[x][y];
                grid[x][y] = nullptr;
            }
            break;
        default:
            break;
    }
}

void antMove(Creature* grid[SIZE_X][SIZE_Y], int x, int y, int distances[4])
{
    string output = grid[x][y]->Move(distances); //return a string of characters representing viable directions
    switch(output[0])
        {
            case 'N': //if the direction is north
                if((y > 0) && !dynamic_cast<Creature*>(grid[x][y - 1])) //if its not an edge case and its empty
                {
                    grid[x][y]->setMoved(true); //set is moved to true so the new ant doesn't get affected
                    grid[x][y - 1] = grid[x][y]; // new grid space points to the old ant
                    grid[x][y] = nullptr; // old space is emptied
                }
                break;
            case 'E':
                if((x < 9) && !dynamic_cast<Creature*>(grid[x + 1][y]))
                {
                    grid[x][y]->setMoved(true);
                    grid[x + 1][y] = grid[x][y];
                    grid[x][y] = nullptr;
                }
                break;
            case 'S':
                if((y < 9) && !dynamic_cast<Creature*>(grid[x][y + 1]))
                {
                    grid[x][y]->setMoved(true);
                    grid[x][y + 1] = grid[x][y];
                    grid[x][y] = nullptr;
                }
                break;
            case 'W':
                if((x > 0) && !dynamic_cast<Creature*>(grid[x - 1][y]))
                {
                    grid[x][y]->setMoved(true);
                    grid[x - 1][y] = grid[x][y];
                    grid[x][y] = nullptr;
                }
                break;
            case ' ':
                break;
            default:
                break;
        }
}

int checkAntNeighbors(Creature* grid[SIZE_X][SIZE_Y], int x, int y)
{
    int neighbors = 0;

    //North
    if(y - 1 >= 0 && dynamic_cast<Ant*>(grid[x][y - 1])) //if the north object is an ant
    {
        neighbors++; // increase neighbors
    }
    //Northeast
    if(y - 1 >= 0  && x + 1 <= 9 && dynamic_cast<Ant*>(grid[x + 1][y - 1]))
    {
        neighbors++;
    }
    //East
    if(x + 1 <= 9 && dynamic_cast<Ant*>(grid[x + 1][y]))
    {
        neighbors++;
    }
    //Southeast
    if(y + 1 <= 9  && x + 1 <= 9 && dynamic_cast<Ant*>(grid[x + 1][y + 1]))
    {
        neighbors++;
    }
    //South
    if(y + 1 <= 9 && dynamic_cast<Ant*>(grid[x][y + 1]))
    {
        neighbors++;
    }
    //Southwest
    if(y + 1 <= 9  && x - 1 >= 0 && dynamic_cast<Ant*>(grid[x - 1][y + 1]))
    {
        neighbors++;
    }
    //West
    if(x - 1 >= 0 && dynamic_cast<Ant*>(grid[x - 1][y]))
    {
        neighbors++;
    }
    //Northwest
    if(y - 1 >= 0  && x - 1 >= 0 && dynamic_cast<Ant*>(grid[x - 1][y - 1]))
    {
        neighbors++;
    }

    return neighbors;
}

void populateGrid(Creature* grid[SIZE_X][SIZE_Y], string fileName)
{
    ifstream file;
    file.open(fileName);

    string line = "";
    int i = 0;
    int j = 0;
    while(getline(file, line)) //until there are no more lines to read in the file
    {
        for(char x : line)
        {
            switch (x)
            {
            case 'B': //if a 'B' character is read
                grid[i][j] = new Beetle(); //construct a new beetle
                break;
            case 'a':
                grid[i][j] = new Ant();
                break;
            default:
                grid[i][j] = nullptr; //if no character is read, grid space is "empty"
                break;
            }
            i++;
            if(i > 9) break;
        }
        j++;
        if(j > 9) break;
        i = 0;
    }
}

void printGrid(Creature* grid[SIZE_X][SIZE_Y], char antChar = 'a', char beetleChar = 'b')
{
    for(int y = 0; y < 10; y++)
    {
        for(int x = 0; x < 10; x++)
        {
            if(dynamic_cast<Beetle*>(grid[x][y])) // if it's a beetle
            {
                cout << beetleChar; // output beetle character
            }
            else if (dynamic_cast<Ant*>(grid[x][y])) // if it's an ant
            {
                cout << antChar; // output ant character
            }
            else cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void resetCreatures(Creature* grid[SIZE_X][SIZE_Y])
{
    for(int y = 0; y < 10; y++)
    {
        for(int x = 0; x < 10; x++)
        {
            if(grid[x][y] != nullptr)
            {
                grid[x][y]->setMoved(false); //sets each object to ismoved false so it can be affected once again
            }
        }
    }
}

void antBreed(Creature* grid[SIZE_X][SIZE_Y], int x, int y, int distances[4])
{
    Ant* ant = dynamic_cast<Ant*>(grid[x][y]); //current ant object
    string breed_spaces = ant->Breed(distances); //receives directions in which a new ant can be created
    char c = breed_spaces[0];
        switch (c)
        {
            case 'N': //if an empty space is north
                if((y > 0) && !dynamic_cast<Creature*>(grid[x][y - 1])) // if it's not an edge case and it's empty
                {
                    grid[x][y - 1] = new Ant(); //create a new ant at that location
                    grid[x][y - 1]->setMoved(true); //make sure it doesn't get affected
                }
                break;
            case 'E':
                if((x < 9) && !dynamic_cast<Creature*>(grid[x + 1][y]))
                {
                    grid[x + 1][y] = new Ant();
                    grid[x + 1][y]->setMoved(true);
                }
                break;
            case 'S':
                if((y < 9) && !dynamic_cast<Creature*>(grid[x][y + 1]))
                {
                    grid[x][y + 1] = new Ant();
                    grid[x][y + 1]->setMoved(true);
                }
                break;
            case 'W':
                if((x > 0) && !dynamic_cast<Creature*>(grid[x - 1][y]))
                {
                    grid[x - 1][y] = new Ant();
                    grid[x - 1][y]->setMoved(true);
                }
                break;
            default:
                break;
        }
}

void creatureDistances(Creature* grid[SIZE_X][SIZE_Y], int x, int y, int distances[4])
{
    int i = x;
    int j = y;

    //N orthogonal check
    while(j >= 0)
    {
        j--;
        if(j < 0)
        {
            distances[0] = y*-1; //if the check reaches a wall, changes return value to a negative*distance from wall
        }
        else if (dynamic_cast<Creature*>(grid[i][j])) //if there is a creature along the direction
        {
            distances[0] = y - j; //return distance to creature
            break;
        }
    }
    j = y;

    //E orthogonal check
    while(i <= 9)
    {
        i++;
        if(i > 9)
        {
            distances[1] = (9-x)*-1;
        }
        else if (dynamic_cast<Creature*>(grid[i][j]))
        {
            distances[1] = i - x;
            break;
        }
    }
    i = x;
    
    //S orthogonal check
    while(j <= 9)
    {
        j++;
        if(j > 9)
        {
            distances[2] = (9-y)*-1;
        }
        else if (dynamic_cast<Creature*>(grid[i][j]))
        {
            distances[2] = j - y;
            break;
        }
    }
    j = y;
    
    //W orthogonal check
    while(i >= 0)
    {
        i--;
        if(i < 0)
        {
            distances[3] = x*-1;
        }
        else if (dynamic_cast<Creature*>(grid[i][j]))
        {
            distances[3] = x - i;
            break;
        }
    }
}

void beetleBreed(Creature* grid[SIZE_X][SIZE_Y], int x, int y, int distances[4])
{
    Beetle* beetle = dynamic_cast<Beetle*>(grid[x][y]); //beetle object at the location
    string breed_spaces = beetle->Breed(distances); //receives optimal breeding spaces
    char c = breed_spaces[0];
        switch (c)
        {
            case 'N': //if an empty space is north
                if((y > 0) && !dynamic_cast<Creature*>(grid[x][y - 1])) //ensure it's not an edge case and it's empty
                {
                    grid[x][y - 1] = new Beetle(); //empty spot becomes a beetle
                    grid[x][y - 1]->setMoved(true); //new beetle can't be affected until turn is over
                }
                break;
            case 'E':
                if((x < 9) && !dynamic_cast<Creature*>(grid[x + 1][y]))
                {
                    grid[x + 1][y] = new Beetle();
                    grid[x + 1][y]->setMoved(true);
                }
                break;
            case 'S':
                if((y < 9) && !dynamic_cast<Creature*>(grid[x][y + 1]))
                {
                    grid[x][y + 1] = new Beetle();
                    grid[x][y + 1]->setMoved(true);
                }
                break;
            case 'W':
                if((x > 0) && !dynamic_cast<Creature*>(grid[x - 1][y]))
                {
                    grid[x - 1][y] = new Beetle();
                    grid[x - 1][y]->setMoved(true);
                }
                break;
            case ' ':
                break;
            default:
                break;
        }
}
