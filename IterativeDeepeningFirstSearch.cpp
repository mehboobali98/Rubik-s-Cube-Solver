#include <iostream>
#include <ctime>
#include <stack>
#include <fstream>
#include <vector>
#include <string.h>
#include <time.h>

using namespace std;

class Cube
{
public:
    int depth;
    int rows;
    int cols;
    int sides;
    int *array;
    Cube *parent;
    char moves[200];

    Cube()
    {
        rows = 0;
        cols = 0;
        sides = 0;
        depth = 0;
        array = nullptr;
        parent = nullptr;
    }

    Cube(int r, int c, int s)
    {
        rows = r;
        cols = c;
        sides = s;
        depth = 0;
        array = new int[r * c * s];
        parent = nullptr;
    }

    Cube(int r, int c, int s, int d, Cube * p)
    {
        rows = r;
        cols = c;
        sides = s;
        depth = d;
        array = new int[r * c * s];
        parent = p;
    }

    bool operator == (const Cube& otherNode) const
    {
        int size = rows * cols * sides;
        for (int i = 0; i<size; i++)
        {
            if (array[i] != otherNode.array[i])
            {
                return false;
            }
        }
        return true;
    }
    //copy constructor
    Cube(const Cube &node)
    {
        rows = node.rows;
        cols = node.cols;
        sides = node.sides;
        depth = node.depth;
        strcpy(moves,node.moves);

        array = new int[rows * cols * sides];

        for (int i = 0; i<(rows*cols*sides); i++)
        {
            array[i] = node.array[i];
        }
    }

    void print()
    {
        int count = 0;
        for (int i = 0; i<sides; i++)
        {
            for (int j = 0; j<rows; j++)
            {
                for (int k = 0; k<cols; k++)
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
        cout<<moves<<endl;
    }
    ~Cube()
    {
        if (array != nullptr)
        {
            delete[]array;
        }
    }
};

//function prototypes
void printCube(int *** cube, int rows, int cols, int sides);
void DLDFS(int ***cube, int ***goalCube, int rows, int cols, int sides, int maxDepth, bool & cutOff);
void iterativeDeepening(int rows, int cols, int sides, int ***cube, int ***GoalCube);
void createDummyCube(int *** cube, int *array, int rows, int cols, int sides);
bool checkChildExists(stack <Cube*> open, stack<Cube*> closed, Cube c);
void storeIn1D(int *** cube, int *array, int rows, int cols, int sides);
void storeIn3D(int *** cube, int *array, int rows, int cols, int sides);
void printArray(int *array, int size);
void readFromFile(int *array,int *array1, string fileName);
void takeTranspose(int ***cube, int rows, int side);
void createInitialCube(int ***cube, int *array, int rows, int cols, int sides);
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
void printMoves(Cube *c);

int main()
{
    const int sides = 6;
    const int rows = 3;
    const int cols = 3;
    int *** cube = new int **[sides];
    int *** goalCube = new int **[sides];
    int *array = new int[rows*cols*sides];
    int *array1 = new int[rows*cols*sides];
    clock_t t1,t2;

    //allocating memory for cubes
    for (int i = 0; i<sides; i++)
    {
        cube[i] = new int *[rows];
        for (int j = 0; j<rows; j++)
        {
            cube[i][j] = new int[cols];
        }
    }

    for (int i = 0; i<sides; i++)
    {
        goalCube[i] = new int *[rows];
        for (int j = 0; j<rows; j++)
        {
            goalCube[i][j] = new int[cols];
        }
    }

    //reading from file
    readFromFile(array,array1,"input.txt");

    //creating initial and goal cubes
    createInitialCube(cube, array, rows, cols, sides);
    createInitialCube(goalCube, array1, rows, cols, sides);

    //moves
    rightClockWise(cube,cols,sides);
    frontClockWise(cube,cols,sides);
    backClockWise(cube,cols,sides);
    //topClockWise(cube,cols,sides);
    //frontClockWise(cube,cols,sides);
    //bottomAntiClockWise(cube,cols,sides);
    //leftClockWise(cube,rows,sides);

    t1 = clock();
    iterativeDeepening(rows, cols, sides, cube, goalCube);
    t2 = clock();
    float diff ((float)t2-(float)t1);
    float seconds = diff / CLOCKS_PER_SEC;
    cout<<"Time taken to find goal: "<<seconds<<" seconds"<<endl;

    //deallocating memory
    for(int i = 0; i < sides; i++)
    {
        for(int j(0); j < rows; j++)
            delete[] cube[i][j];
        delete[] cube[i];
    }

    for(int i = 0; i < sides; i++)
    {
        for(int j(0); j < rows; j++)
            delete[] goalCube[i][j];
        delete[] goalCube[i];
    }
    delete[] goalCube;
    delete[] cube;
    delete[] array;

    return 0;
}

//wrapper function
void iterativeDeepening(int rows, int cols, int sides, int ***cube, int ***GoalCube)
{
    int depth = 0;
    bool cutOff = false;
    while (cutOff == false)
    {
        cout << "Depth: " << depth << endl;
        DLDFS(cube, GoalCube, rows, cols, sides, depth, cutOff);
        depth++;
    }
}

//depth limited depth first search
void DLDFS(int ***cube, int ***GoalCube, int rows, int cols, int sides, int maxDepth, bool & cutOff)
{
    stack<Cube*> open;   // frontier
    stack<Cube*> closed; // for already visited nodes
    Cube *c;
    Cube *temp;
    int ***tempCC = new int **[sides];
    Cube *initialCube = new Cube(rows, cols, sides);
    Cube goalCube(rows, cols, sides);

    for (int i = 0; i < sides; i++)
    {
        tempCC[i] = new int *[rows];
        for (int j = 0; j < rows; j++)
        {
            tempCC[i][j] = new int[cols];
        }
    }

    storeIn1D(cube, initialCube->array, rows, cols, sides);
    storeIn1D(GoalCube, goalCube.array, rows, cols, sides);

    open.push(initialCube);

    while (!open.empty())  // there are some nodes which have not been explored
    {
        temp = open.top();
        open.pop();
        closed.push(temp); //pushing it to the closed stack
        if (*temp == goalCube)
        {
            cout << "Goal found!!\n" << endl;
            cout << "Nodes expanded: "<<closed.size()<<endl;
            cout<<endl;
            cutOff = true;
            printMoves(temp); // printing the moves after the goal has been reached
            break;
        }

        if (temp->depth == maxDepth)
        {
            continue; // skip the iteration
        }

        //Generating child states
        int tempDepth = temp->depth;
        tempDepth++;

        //Front clockwise
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        frontClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth, temp);
        strcpy(c->moves,"Front Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }

        //Front anti-clockwise
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        frontAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth, temp);
        strcpy(c->moves,"Front Face, AntiClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }

        //back clockwise
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        backClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,temp);
        strcpy(c->moves,"Back Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }

        //back ACW
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        backAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,temp);
        strcpy(c->moves,"Back Face, AntiClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }

        //top CW
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        topClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,temp);
        strcpy(c->moves,"Top Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }

        //top ACW
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        topAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,temp);
        strcpy(c->moves,"Top Face, AntiClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }

        //bottom CW
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        bottomClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,temp);
        strcpy(c->moves,"Bottom Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }

        //bottom ACW
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        bottomAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,temp);
        strcpy(c->moves,"Bottom Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);

        }

        //left CW
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        leftClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,temp);
        strcpy(c->moves,"Left Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }

        //left ACW
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        leftAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,temp);
        strcpy(c->moves,"Left Face, AntiClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }

        //right cw
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        rightClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,temp);
        strcpy(c->moves,"Right Face, ClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }

        //right ACW
        createDummyCube(tempCC, temp->array, rows, cols, sides);
        rightAntiClockWise(tempCC, cols, sides);
        c = new Cube(rows, cols, sides, tempDepth,temp);
        strcpy(c->moves,"Right Face, AntiClockWise");
        storeIn1D(tempCC, c->array, rows, cols, sides);
        if (!(checkChildExists(open, closed, *c)))
        {
            open.push(c);
        }
    }

    cout<<"Closed size: "<<closed.size()<<endl;

    //deallocating memory
    for(int i = 0; i < sides; i++)
    {
        for(int j(0); j < rows; j++)
            delete[] tempCC[i][j];
        delete[] tempCC[i];
    }

    delete[] tempCC;
    delete initialCube;
}

void printMoves(Cube *c)
{
    //printing the moves
    cout<<"Moves to reach goal: "<<endl;
    Cube *tt = c;
    stack<Cube> moveStack;
    while(tt!=nullptr)
    {
        moveStack.push(*tt);
        tt=tt->parent;
    }
    Cube cc;
    while(!moveStack.empty())
    {
        cc = moveStack.top();
        cc.printMoves();
        moveStack.pop();
    }
    cout<<endl;
    delete tt;
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
    frontClockWise(cube,cols,sides);
    frontClockWise(cube,cols,sides);
    frontClockWise(cube,cols,sides);
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
    backClockWise(cube,cols,sides);
    backClockWise(cube,cols,sides);
    backClockWise(cube,cols,sides);
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
    topClockWise(cube,cols,sides);
    topClockWise(cube,cols,sides);
    topClockWise(cube,cols,sides);
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
    bottomClockWise(cube,cols,sides);
    bottomClockWise(cube,cols,sides);
    bottomClockWise(cube,cols,sides);
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
    leftClockWise(cube,rows,sides);
    leftClockWise(cube,rows,sides);
    leftClockWise(cube,rows,sides);
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
    rightClockWise(cube,rows,sides);
    rightClockWise(cube,rows,sides);
    rightClockWise(cube,rows,sides);
}

void printCube(int ***cube, int rows, int cols, int sides)
{
    for (int k = 0; k<sides; k++)
    {
        cout << "Side: " << k + 1 << endl;
        for (int i = 0; i<rows; i++)
        {
            for (int j = 0; j<cols; j++)
            {
                cout << cube[k][i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

void readFromFile(int *array, int *array1, string fileName)
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

    for(int i =0; i<rows; i++)
        for(int j =0; j<rows; j++)
            temper[i][j] = cube[side][i][j];

    for (int i = 0; i<rows; i++)
    {
        for (int j = 0; j<rows; j++)
        {
            cube[side][i][j] = temper[2-j][i];
        }
    }

    //deallocating memory
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
    for (int i = 0; i<rows; i++)
    {
        for (int j = 0; j<cols; j++)
        {
            cube[0][i][j] = array[count];
            count++;
        }
    }

    //back
    for (int i = 0; i<rows; i++)
    {
        for (int j = 0; j<cols; j++)
        {
            cube[2][i][j] = array[count];
            count++;
        }
    }

    //top
    for (int i = 0; i<rows; i++)
    {
        for (int j = 0; j<cols; j++)
        {
            cube[5][i][j] = array[count];
            count++;
        }
    }

    //bottom
    for (int i = 0; i<rows; i++)
    {
        for (int j = 0; j<cols; j++)
        {
            cube[4][i][j] = array[count];
            count++;
        }
    }

    //left
    for (int i = 0; i<rows; i++)
    {
        for (int j = 0; j<cols; j++)
        {
            cube[1][i][j] = array[count];
            count++;
        }
    }

    //right
    for (int i = 0; i<rows; i++)
    {
        for (int j = 0; j<cols; j++)
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
    for (int i = 0; i<sides; i++)
    {
        for (int j = 0; j<rows; j++)
        {
            for (int k = 0; k<cols; k++)
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
    for (int i = 0; i<sides; i++)
    {
        for (int j = 0; j<rows; j++)
        {
            for (int k = 0; k<cols; k++)
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
        closed.pop();
    }
    cout << endl;
}

void printArray(int *array, int size)
{
    for (int i = 0; i<size; i++)
    {
        cout << array[i] << " ";
    }
    cout << endl;
}

//simply stores Cube values into 3D array for moves
void createDummyCube(int *** cube, int *array, int rows, int cols, int sides)
{
    storeIn3D(cube, array, rows, cols, sides);
}

//check if a child already exists in the frontier and the closed stack
bool checkChildExists(stack <Cube*> open, stack<Cube*> closed, Cube c)
{
    Cube *temp;
    while (!open.empty())
    {
        temp = open.top();
        if (*temp == c)
        {
            return true; //child state already exists
        }
        open.pop();
    }

    while (!closed.empty())
    {
        temp = closed.top();
        if (*temp == c)
        {
            return true; //child state already exists
        }
        closed.pop();
    }
    return false; //it does not exist
}
