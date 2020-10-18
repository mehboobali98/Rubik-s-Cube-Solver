#include <iostream>
#include <ctime>
#include <stack>
#include <fstream>
#include <queue>
#include <time.h>
#include <string.h>

using namespace std;

class Cube
{
public:
    int depth;
    int rows;
    int cols;
    int sides;
    float fn;
    int array[54];
    Cube* cube;
    char moves[200];

    Cube()
    {
        rows = 0;
        cols = 0;
        sides = 0;
        depth = 0;
        fn = 0;
        cube = nullptr;
    }

    Cube(int r, int c, int s)
    {
        rows = r;
        cols = c;
        sides = s;
        depth = 0;
        fn = 0;
    }

    Cube(int r, int c, int s, int d)
    {
        rows = r;
        cols = c;
        sides = s;
        depth = d;
    }

    Cube(int r, int c, int s, int d,Cube*p)
    {
        rows = r;
        cols = c;
        sides = s;
        depth = d;
        cube = p;
    }

    void setFnValue(int hn2, float hn1, int stepCost) //setting the heuristic values
    {
        fn = hn1 + hn2 + stepCost;   //hn1: manhattan distance, htn2: hamming distance
                                   //have used them in combination and separately as well to check the results
    }

    //overloaded operator
    bool operator == (const Cube& otherNode) const
    {
        int size = rows * cols * sides;
        for (int i = 0; i < size; i++)
        {
            if (array[i] != otherNode.array[i])
            {
                return false;
            }
        }
        return true;
    }

    bool operator< (const Cube & node)const
    {
        return fn > node.fn;
    }

    //copy constructor
    Cube(const Cube &node)
    {
        cube = node.cube;
        rows = node.rows;
        cols = node.cols;
        sides = node.sides;
        depth = node.depth;
        fn = node.fn;
        strcpy(moves, node.moves);
        //cube = node.cube;
        for (int i = 0; i < 54; i++)
        {
            array[i] = node.array[i];
        }
    }

    void print()
    {
        cout << "fn value: " << fn << endl;
        int count = 0;
        for (int i = 0; i < sides; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                for (int k = 0; k < cols; k++)
                {
                    cout << array[count] << " ";
                    count++;
                }
                cout << endl;
            }
            cout << endl;
        }
    }

    void printMoves()
    {
        cout << moves << endl;
    }

    ~Cube()
    {
        //destructor
    }
};

//prototypes
bool checkNodeExistsInQueue(priority_queue<Cube> &open, Cube c);
bool checkNodeExistsInStack(stack<Cube> &closed,priority_queue<Cube> &open, Cube c);
bool checkChildExists(priority_queue <Cube> open, stack <Cube> closed, Cube c);
void showPriorityQueue(priority_queue <Cube> closed);
void printCube(int *** cube, int rows, int cols, int sides);
void AStarSearch(int ***cube, int ***goalCube, int rows, int cols, int sides, int maxDepth, bool & cutOff);
void iterativeDeepeningAStarSearch(int rows, int cols, int sides, int ***cube, int ***GoalCube);
void createDummyCube(int *** cube, int *array, int rows, int cols, int sides);
void storeIn1D(int *** cube, int *array, int rows, int cols, int sides);
void storeIn3D(int *** cube, int *array, int rows, int cols, int sides);
void printArray(int *array, int size);
void readFromFile(int *array,int *array1, string fileName);
void takeTranspose(int ***cube, int rows, int side);
void createInitialCube(int ***cube, int *array, int rows, int cols, int sides);
int hammingDistance(int *cube, int *goal, int rows, int cols, int sides);
float manhattanDistance(int ***cube, int ***goalCube, int rows, int cols, int sides);
float findShortestDistance(int ***cube, int index_1, int index_2, int index_3, int number);
float distanceBtwCubes(int x1, int y1, int z1, int x2, int y2, int z2);
void showStack(stack <Cube> closed);
void frontClockWise(int ***cube, int cols, int sides);
void frontAntiClockWise(int ***cube, int cols, int sides);
void backClockWise(int ***cube, int cols, int sides);
void backAntiClockWise(int ***cube, int cols, int sides);
void topClockWise(int ***cube, int cols, int sides);
void topAntiClockWise(int ***cube, int cols, int sides);
void bottomClockWise(int ***cube, int cols, int sides);
void bottomAntiClockWise(int ***cube, int cols, int sides);
void leftClockWise(int ***cube, int rows, int sides);
void leftAntiClockWise(int ***cube, int rows, int sides);
void rightClockWise(int ***cube, int rows, int sides);
void rightAntiClockWise(int ***cube, int rows, int sides);
void showMoves(stack <Cube> closed);

int main()
{
    const int sides = 6;
    const int rows = 3;
    const int cols = 3;
    bool cutOff = false;
    int *** cube = new int **[sides];
    int *** goalCube = new int **[sides];
    int *array = new int[rows*cols*sides];
    int *array1 = new int[rows*cols*sides];
    clock_t t1, t2;
    for (int i = 0; i < sides; i++)
    {
        cube[i] = new int *[rows];
        for (int j = 0; j < rows; j++)
        {
            cube[i][j] = new int[cols];
        }
    }

    for (int i = 0; i < sides; i++)
    {
        goalCube[i] = new int *[rows];
        for (int j = 0; j < rows; j++)
        {
            goalCube[i][j] = new int[cols];
        }
    }

    //reading initial and goal state from file
    readFromFile(array,array1, "input.txt");

    //creating initial cubes
    createInitialCube(cube, array, rows, cols, sides);
    createInitialCube(goalCube, array1, rows, cols, sides);

    //printCube(cube, rows, cols, sides);
    //printCube(goalCube,rows,cols,sides);

    rightClockWise(cube,cols,sides);
    frontClockWise(cube, cols, sides);
    //backClockWise(cube, cols, sides);
    topClockWise(cube, cols, sides);
    frontClockWise(cube, cols, sides);
    //leftAntiClockWise(cube,cols,sides);
    //bottomClockWise(cube,cols,sides);

    t1 = clock();
    //iterativeDeepeningAStarSearch(rows, cols, sides, cube, goalCube);
    //^ is the iterative deepening version of the A* Search as the function name suggests.

    AStarSearch(cube,goalCube,rows,cols,sides,20, cutOff);    // 20 is the max depth as we should be able to solve the cube in 20 moves
    t2 = clock();
    float difference((float)t2 - (float)t1);
    float seconds = difference / CLOCKS_PER_SEC;
    cout << "Time taken to find goal: " << seconds << " seconds" << endl;

    //deallocating memory
    for (int i = 0; i < sides; i++)
    {
        for (int j(0); j < rows; j++)
            delete[] cube[i][j];
        delete[] cube[i];
    }

    for (int i = 0; i < sides; i++)
    {
        for (int j(0); j < rows; j++)
            delete[] goalCube[i][j];
        delete[] goalCube[i];
    }
    delete[] goalCube;
    delete[] cube;
    delete[] array;

        /*
    rightAntiClockWise(cube,cols,sides);
    leftClockWise(cube,cols,sides);
    topClockWise(cube,cols,sides);
    bottomAntiClockWise(cube,cols,sides);
    backAntiClockWise(cube,cols,sides);
    frontClockWise(cube,cols,sides);

    bottomAntiClockWise(cube, cols, sides);
    topClockWise(cube, cols, sides);
    leftClockWise(cube, cols, sides);
    rightAntiClockWise(cube, cols, sides);
    backAntiClockWise(cube, cols, sides);
    frontClockWise(cube, cols, sides);
    */

    return 0;
}

//finds distance between two cubes
float distanceBtwCubes(int x1, int y1, int z1, int x2, int y2, int z2)
{
    float distance = 0;
    distance = (abs(x1 - x2) + abs(y1 - y2) + abs(z1 - z2));

    return distance;
}

//calculates shortest distance for a cubie to reach its goal position
float findShortestDistance(int ***cube, int index_1, int index_2, int index_3, int number)
{
    float distanceArray[9];
    int counter = 0;
    float distance = 0;

    for (int i = 0; i < 6; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                if (cube[i][j][k] == number)
                {
                    distance = distanceBtwCubes(index_1, index_2, index_3, i, j, k);
                    distanceArray[counter] = distance;
                    counter++;
                }
            }
        }
    }

    float min = distanceArray[0];
    for (int i = 0; i < counter; i++)
    {
        if (distanceArray[i]<min)
        {
            min = distanceArray[i];
        }
    }
    return min;
}

//heuristic #1
float manhattanDistance(int ***cube, int ***goalCube, int rows, int cols, int sides)
{
    float sumtotal = 0;

    for (int i = 0; i < sides; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            for (int k = 0; k < cols; k++)
            {
                sumtotal += findShortestDistance(goalCube, i, j, k, cube[i][j][k]);
            }
        }
    }

    sumtotal = sumtotal / 8; //dividing by 8 as 4 edges and 4 corners move when a move is made.
    return sumtotal;
}

//heuristic #2 Finding the number of misplaced nodes
int hammingDistance(int *cube, int *goal, int rows, int cols, int sides)
{
    int totalSize = rows * cols * sides;
    int count = 0;

    for (int i = 0; i < totalSize; i++)
    {
        if (cube[i] != goal[i])
        {
            count++;
        }
    }
    return count;
}

//this function can be used if one wants to implement IDA* Search
void iterativeDeepeningAStarSearch(int rows, int cols, int sides, int ***cube, int ***GoalCube)
{
    int depth = 0;
    bool cutOff = false;
    while (cutOff == false)
    {
        cout << "Depth: " << depth << endl;
        AStarSearch(cube, GoalCube, rows, cols, sides, depth, cutOff);
        depth++;
    }
}

//A* Search
void AStarSearch(int ***cube, int ***GoalCube, int rows, int cols, int sides, int maxDepth, bool & cutOff)
{
    priority_queue<Cube> open; //frontier
    stack<Cube> closed;  //stack to keep track of already visited nodes. Could have used Hash Table.
    Cube * c = NULL;
    Cube temp;
    int counter = 0;
    int ***tempCC = new int **[sides];
    float temp1 = 0;
    float temp2 = 0;

    Cube initialCube(rows, cols, sides);
    Cube goalCube(rows, cols, sides);

    for (int i = 0; i < sides; i++)
    {
        tempCC[i] = new int *[rows];
        for (int j = 0; j < rows; j++)
        {
            tempCC[i][j] = new int[cols];
        }
    }

    storeIn1D(cube, initialCube.array, rows, cols, sides);
    storeIn1D(GoalCube, goalCube.array, rows, cols, sides);

    //finding heuristic values for the initial cube
    temp1 = manhattanDistance(cube,GoalCube,rows,cols,sides);
    temp2 = hammingDistance(initialCube.array,goalCube.array,rows,cols,sides);
    initialCube.setFnValue(temp1,temp2,0); //stepCost for the initial node is 0

    open.push(initialCube);

    while (!open.empty())  // there are some nodes which have not been explored
    {
        temp = open.top();
        open.pop();
        closed.push(temp); //pushing it to the closed stack

        if (temp == goalCube)
        {
            cout << "Goal found!!\n" << endl;
            cout << "Nodes expanded: " << closed.size() - 1<< endl;
            cout << endl;
            cutOff = true;
            //showMoves(closed); // printing the moves after the goal has been reached
            break;
        }

        if (temp.depth == maxDepth)
        {
            continue; // skip the iteration
        }

        //Generating child states
        int tempDepth = temp.depth;
        tempDepth++;

        //Front clockwise =1
        createDummyCube(tempCC, temp.array, rows, cols, sides); //storing in 3D so that moves can be applied easily
        frontClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Front Face, ClockWise"); //storing the move made
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides); // heuristic #1
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides); // heuristic #2
            c->setFnValue(temp1, temp2, tempDepth); // h1,h2, stepCost
            open.push(*c);
        }

        //Front anti-clockwise=2
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        frontAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Front Face, AntiClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }

        //back clockwise=3
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        backClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Back Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }

        //back ACW=4
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        backAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Back Face, AntiClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }

        //top CW=5
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        topClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Top Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }

        //top ACW=6
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        topAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Top Face, AntiClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }

        //back CW=7
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        backClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Bottom Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }

        //back ACW=8
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        backAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Bottom Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }

        //left CW=9
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        leftClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Left Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }

        //left ACW=10
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        leftAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Left Face, AntiClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }

        //right cw=11
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        rightClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Right Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }

        //right ACW=12
        createDummyCube(tempCC, temp.array, rows, cols, sides);
        rightAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,&temp);
        strcpy(c->moves, "Right Face, AntiClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            temp1 = hammingDistance(c->array, goalCube.array, rows, cols, sides);
            temp2 = manhattanDistance(tempCC, GoalCube, rows, cols, sides);
            c->setFnValue(temp1, temp2, tempDepth);
            open.push(*c);
        }
        counter++;
        //   cout << counter << endl;
        //   cout << "Open size: " << open.size() << endl;
        //   cout << "Closed size: " << closed.size() << endl;
    }

    //deallocating memory
    for (int i = 0; i < sides; i++)
    {
        for (int j(0); j < rows; j++)
            delete[] tempCC[i][j];
        delete[] tempCC[i];
    }

    delete[] tempCC;
}

//checks if a node which has already been closed, found again but with a smaller fn value.
bool checkNodeExistsInStack(stack<Cube> &closed,priority_queue<Cube> &open, Cube c)
{
    Cube n;
    stack<Cube> temp;
    while(!closed.empty())
    {
        n = closed.top();
        if(c == n && n.fn > c.fn) // the new node has a lower fn value then we will remove it from closed
        {
            // and put it into the open priority queue
            closed.pop();
            open.push(c);
            // cout<<"Replaced in stack"<<endl;
            return true;
        }
        else
        {
            closed.pop();
            temp.push(n); //fill the nodes back
        }
    }

    Cube t;
    while(!temp.empty())
    {
        t = temp.top();
        closed.push(t);
        temp.pop();
    }

    return false;
}

//checks if a node has already been placed in the frontier and it gets replaced if new fn > fn value of node in queue
bool checkNodeExistsInQueue(priority_queue<Cube> &open, Cube c)
{
    Cube n;
    priority_queue<Cube> temp;
    while(!open.empty())
    {
        n = open.top();
        if(c == n && n.fn > c.fn) // the new node has a lower fn value then we will remove it from closed
        {

            open.pop();
            open.push(c);
            //cout<<"Replaced in Queue"<<endl;
            return true;
        }
        else
        {
            open.pop();
            temp.push(n); //fill the nodes back
        }
    }

    Cube t;
    while(!temp.empty())
    {
        t = temp.top();
        open.push(t);
        temp.pop();
    }

    return false;
}


//Moves
void frontClockWise(int ***cube, int cols, int sides)
{
    int side1[3];
    int count = 0;

    for (int j = 0; j < cols; j++)
    {
        side1[count] = cube[5][2][j];  // 3rd row of the top side
        count++;
    }

    for (int i = 0; i < cols; i++)
    {
        cube[5][2][i] = cube[1][i][2];  // left to top
    }

    for (int i = 0; i < cols; i++)
    {
        cube[1][i][2] = cube[4][0][i];  // bottom to left
    }

    for (int i = 0; i < cols; i++)
    {
        cube[4][0][i] = cube[3][i][0];  // right to bottom
    }

    for (int i = 0; i < cols; i++)
    {
        cube[3][i][0] = side1[i];  // right to bottom
    }

    takeTranspose(cube, cols, 0);
}

void frontAntiClockWise(int ***cube, int cols, int sides)
{
    frontClockWise(cube, cols, sides);
    frontClockWise(cube, cols, sides);
    frontClockWise(cube, cols, sides);
}

void backClockWise(int ***cube, int cols, int sides)
{
    int side1[3];
    int count = 0;

    for (int j = 0; j < cols; j++)
    {
        side1[count] = cube[5][0][j];  // 1st row of the top side
        count++;
    }

    for (int i = 0; i < cols; i++)
    {
        cube[5][0][i] = cube[1][i][0];  // left to top
    }

    for (int i = 0; i < cols; i++)
    {
        cube[1][i][0] = cube[4][0][i];  // bottom to left
    }

    for (int i = 0; i < cols; i++)
    {
        cube[4][0][i] = cube[3][i][2];  // right to bottom
    }

    for (int i = 0; i < cols; i++)
    {
        cube[3][i][2] = side1[i];  // front to left
    }

    takeTranspose(cube, cols, 2);
}

void backAntiClockWise(int ***cube, int cols, int sides)
{
    backClockWise(cube, cols, sides);
    backClockWise(cube, cols, sides);
    backClockWise(cube, cols, sides);
}

void topClockWise(int ***cube, int cols, int sides)
{
    int side1[3];
    int count = 0;

    for (int j = 0; j < cols; j++)
    {
        side1[count] = cube[0][0][j];
        count++;
    }

    for (int i = 0; i < cols; i++)
    {
        cube[0][0][i] = cube[3][0][i];  //right to front
    }

    for (int i = 0; i < cols; i++)
    {
        cube[3][0][i] = cube[2][0][i]; //back to right
    }

    for (int i = 0; i < cols; i++)
    {
        cube[2][0][i] = cube[1][0][i]; //left to back
    }

    for (int i = 0; i < cols; i++)
    {
        cube[1][0][i] = side1[i]; //front to left
    }

    takeTranspose(cube, cols, 5);
}

void topAntiClockWise(int ***cube, int cols, int sides)
{
    topClockWise(cube, cols, sides);
    topClockWise(cube, cols, sides);
    topClockWise(cube, cols, sides);
}

void bottomClockWise(int ***cube, int cols, int sides)
{
    int side1[3];
    int count = 0;

    for (int j = 0; j < cols; j++)
    {
        side1[count] = cube[0][2][j];
        count++;
    }

    for (int i = 0; i < cols; i++)
    {
        cube[0][2][i] = cube[1][2][i];  //left to front
    }

    for (int i = 0; i < cols; i++)
    {
        cube[1][2][i] = cube[2][2][i]; //back to left
    }

    for (int i = 0; i < cols; i++)
    {
        cube[2][2][i] = cube[3][2][i]; //right to back
    }

    for (int i = 0; i < cols; i++)
    {
        cube[3][2][i] = side1[i]; //front to right
    }

    takeTranspose(cube, cols, 4);
}

void bottomAntiClockWise(int ***cube, int cols, int sides)
{
    bottomClockWise(cube, cols, sides);
    bottomClockWise(cube, cols, sides);
    bottomClockWise(cube, cols, sides);

}

void leftClockWise(int ***cube, int rows, int sides)
{
    int side[3];
    int count = 0;

    for (int i = 0; i < rows; i++)
    {
        side[count] = cube[0][i][0];
        count++;
    }

    for (int i = 0; i < rows; i++)
    {
        cube[0][i][0] = cube[5][i][0]; // top to front
    }

    for (int i = 0; i < rows; i++)
    {
        cube[5][i][0] = cube[2][i][2]; // back to top
    }

    for (int i = 0; i < rows; i++)
    {
        cube[2][i][2] = cube[4][i][0]; // bottom to back
    }

    for (int i = 0; i < rows; i++)
    {
        cube[4][i][0] = side[i]; // from to bottom
    }

    takeTranspose(cube, rows, 1);
}

void leftAntiClockWise(int ***cube, int rows, int sides)
{
    leftClockWise(cube, rows, sides);
    leftClockWise(cube, rows, sides);
    leftClockWise(cube, rows, sides);
}

void rightClockWise(int ***cube, int rows, int sides)
{
    int side[3];
    int count = 0;

    for (int i = 0; i < rows; i++)
    {
        side[count] = cube[0][i][2];
        count++;
    }

    for (int i = 0; i < rows; i++)
    {
        cube[0][i][2] = cube[4][i][2]; // bottom to front
    }

    for (int i = 0; i < rows; i++)
    {
        cube[4][i][2] = cube[2][i][0]; // back to bottom
    }

    for (int i = 0; i < rows; i++)
    {
        cube[2][i][0] = cube[5][i][2]; // top to back
    }

    for (int i = 0; i < rows; i++)
    {
        cube[5][i][2] = side[i]; // copying from 5th side (bottom) to front side 1st
    }

    takeTranspose(cube, rows, 3);
}

void rightAntiClockWise(int ***cube, int rows, int sides)
{
    rightClockWise(cube, rows, sides);
    rightClockWise(cube, rows, sides);
    rightClockWise(cube, rows, sides);
}

//prints the 3D array as cube
void printCube(int ***cube, int rows, int cols, int sides)
{
    for (int k = 0; k < sides; k++)
    {
        cout << "Side: " << k + 1 << endl;
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cout << cube[k][i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void readFromFile(int *array,int *array1, string fileName)
{
    ifstream fin;
    fin.open(fileName);

    int i = 0;
    int x = 0;
    int count = 0;
    while (fin >> x)
    {
        if(i < 54)
        {
            array[i] = x;
            i++;
        }
        else
        {
            array1[count] = x;
            count++;
        }
    }
    fin.close();
}

void takeTranspose(int ***cube, int rows, int side)
{

    int ** temper = new int*[rows];
    temper[0] = new int[rows];
    temper[1] = new int[rows];
    temper[2] = new int[rows];

    for (int i = 0; i<rows; i++)
        for (int j = 0; j<rows; j++)
            temper[i][j] = cube[side][i][j];

    for (int i = 0; i<rows; i++)
    {
        for (int j = 0; j<rows; j++)
        {
            cube[side][i][j] = temper[2 - j][i];
        }
    }

    for (int i = 0; i < rows; i++)
    {
        delete[] temper[i];
    }
    delete[] temper;
}
void createInitialCube(int ***cube, int *array, int rows, int cols, int sides)
{
    int count = 0;
    //front
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cube[0][i][j] = array[count];
            count++;
        }
    }

    //back
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cube[2][i][j] = array[count];
            count++;
        }
    }

    //top
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cube[5][i][j] = array[count];
            count++;
        }
    }

    //bottom
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cube[4][i][j] = array[count];
            count++;
        }
    }

    //left
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cube[1][i][j] = array[count];
            count++;
        }
    }

    //right
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cube[3][i][j] = array[count];
            count++;
        }
    }
}

//stores the cube in 1D array
void storeIn1D(int *** cube, int * array, int rows, int cols, int sides)
{
    int count = 0;
    for (int i = 0; i < sides; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            for (int k = 0; k < cols; k++)
            {
                array[count] = cube[i][j][k];
                count++;
            }
        }
    }
}

//stores the cube in 3D array
void storeIn3D(int *** cube, int *array, int rows, int cols, int sides)
{
    int count = 0;
    for (int i = 0; i < sides; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            for (int k = 0; k < cols; k++)
            {
                cube[i][j][k] = array[count];
                count++;
            }
        }
    }
}

void showStack(stack <Cube> closed)
{
    Cube n;
    while (!closed.empty())
    {
        n = closed.top();
        n.print();
        //cout << n.fn;
        cout << endl;
        closed.pop();
    }
    cout << endl;
}

void showPriorityQueue(priority_queue <Cube> open)
{
    Cube c;
    while (!open.empty())
    {
        c = open.top();
        //cout << c.fn;
        c.print();
        cout << endl;
        open.pop();
    }
}

void printArray(int *array, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;
}

//wrapper for filling up the cube
void createDummyCube(int *** cube, int *array, int rows, int cols, int sides)
{
    storeIn3D(cube, array, rows, cols, sides);
}

//checking if a node already exists in the frontier and stack for explored nodes
bool checkChildExists(priority_queue <Cube> open, stack<Cube> closed, Cube c)
{
    Cube temp;
    while (!open.empty())
    {
        temp = open.top();
        if (temp == c)
        {
            return true; //child state already exists
        }
        open.pop();
    }

    while (!closed.empty())
    {
        temp = closed.top();
        if (temp == c)
        {
            return true; //child state already exists
        }
        closed.pop();
    }
    return false; //it does not exist
}

void showMoves(stack <Cube> closed)
{
    Cube n;
    while (!closed.empty())
    {
        n = closed.top();
        //n.print();
        cout<<n.moves;
        cout << endl;
        closed.pop();
    }
    cout << endl;
}
