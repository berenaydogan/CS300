// Beren Aydoğan - HW1

#include <iostream>
#include <vector>
#include <fstream>
#include "StackTemp.h"
#include "randgen.cpp"
#include "randgen.h"

using namespace std;

// Cell struct to construct the structure for cells with walls
struct Cell {
    int x, y; // x and y coordinates of the Cell in the vector, the leftmost column and the buttommost row are 0
    int l, r, u, d; // the walls of the Cell: l -> left, r-> right, u->up, d->down, if the wall exist 1 else 0
    bool isVisited; // the flag that indicates if a Xell is visited: true if visited, false if unvisider

    Cell() :
        l(1), r(1), u(1), d(1), isVisited(false) {} // Default constructor of Cell struct
};

// Maze struct to construct the structure for maze using Cell struct and a 2D vector
struct Maze {
    int rows; // number of rows in Maze
    int cols; // number of colums in Maze
    vector<vector<Cell> > maze; // a 2D vector of Cells to create a maze

    // Parameterized contructor of Maze class, it require the number of rows and columns to contruct a Maze
    Maze(int rows, int cols) {
        this->rows = rows; // Initializes the rows member variable to the parameter "rows"
        this->cols = cols;  // Initializes the cols member variable to the parameter "cols"
        this->maze = vector<vector<Cell> >(rows, vector<Cell>(cols)); // Initialize the maze member variable to a 2D vector of Cells

        // Through iteration, set every Cell's x and y values according to their coordinates in the vector
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                (this->maze)[i][j].x = j; // Set x value to column value
                (this->maze)[i][j].y = i; // Set y value to row value
            }
        }
    }
};

// Function that checks if given Cell in the given Maze has any unvisited neighbor cells. 
// Returns true if it does, false if it doesn't
bool anyUnvisitedNeighbors(const Cell & currentCell, const Maze & myMaze) {
    bool anyUnvisitedNeighbor = false; // The flag that indicates the existance of any unvisited neighbors; true if there is, else false
    vector<vector<int>> steps = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Steps to neighboring cells

    // Iterate through each possible neighboring cell 
    for (const vector<int> step : steps) {
        int newRow = currentCell.y + step[0];
        int newCol = currentCell.x + step[1];

        // Check if the new coordinates are within the bounds of the maze
        if (newRow >= 0 && newRow < myMaze.rows && newCol >= 0 && newCol < myMaze.cols) {
            // If the new coordinates are within the bounds of the maze check if it is unvisited
            if (!(myMaze.maze[newRow][newCol].isVisited)) {
                anyUnvisitedNeighbor = true; // If unvisited, turn the flag to true
                break; // Break from the loop
            }
        }
    }
    return anyUnvisitedNeighbor; // Return the flag
}

// Function that checks which neighbors of given Cell in the given Maze are unvisited, selects one randomly
// Returns a reference to the selected unvisited neighbor
Cell & chooseNeighbor(const Cell & currentCell, Maze & myMaze) {
    RandGen rand; // Random number generator object to generate a number for the random selection of the neighboring unvisited cells
    vector<Cell> unvisitedNbs; // Vector to store unvisited neighbors of the cell
    vector<vector<int>> steps = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Steps to neighboring cells

    for (const vector<int> step : steps) {
        int newRow = currentCell.y + step[0];
        int newCol = currentCell.x + step[1];
        // Check if the new coordinates are within the bounds of the maze
        if (newRow >= 0 && newRow < myMaze.rows && newCol >= 0 && newCol < myMaze.cols) {
            // If the new coordinates are within the bounds of the maze check if it is unvisited
            if (!(myMaze.maze[newRow][newCol].isVisited)) {
                unvisitedNbs.push_back(myMaze.maze[newRow][newCol]); // If unvisited, push to the vector unvisitedNbs
            }
        }
    }
    
    int nbIdx = rand.RandInt(0, unvisitedNbs.size() - 1); // Generate a random number from the indexes of unvisitedNbs vector
    return myMaze.maze[unvisitedNbs[nbIdx].y][unvisitedNbs[nbIdx].x]; // Return the selected cell of the maze
}

// Function that knocks down the walls between two cells according to the relationship of their coordinates
void knockDownWall(Cell & currentCell, Cell & cellToVisit) {
    
    if (currentCell.x == cellToVisit.x && currentCell.y == cellToVisit.y + 1) {
        currentCell.d = 0;
        cellToVisit.u = 0;
    }
    else if (currentCell.x == cellToVisit.x && currentCell.y == cellToVisit.y - 1) {
        currentCell.u = 0;
        cellToVisit.d = 0;
    }
    else if (currentCell.x == cellToVisit.x + 1 && currentCell.y == cellToVisit.y) {
        currentCell.l = 0;
        cellToVisit.r = 0;
    }
    else if (currentCell.x == cellToVisit.x - 1 && currentCell.y == cellToVisit.y) {
        currentCell.r = 0;
        cellToVisit.l = 0;
    }
}

// Function that checks if given Cell in the given Maze has any unvisited and reachable (meaning there are no walls between them) neighbor cells. 
// Returns true if it does, false if it doesn't
bool anyUnvisitedandReachableCells(const Cell & currentCell, const Maze & myMaze) {
    if (currentCell.u == 0 && !myMaze.maze[currentCell.y + 1][currentCell.x].isVisited)
        return true;
    if (currentCell.d == 0 && !myMaze.maze[currentCell.y - 1][currentCell.x].isVisited)
        return true;
    if (currentCell.l == 0 && !myMaze.maze[currentCell.y][currentCell.x - 1].isVisited)
        return true;
    if (currentCell.r == 0 && !myMaze.maze[currentCell.y][currentCell.x + 1].isVisited)
        return true;
    
    return false;
}

// Function that checks which neighbors of given Cell in the given Maze are unvisited and reachable (meaning there are no walls between them), selects one randomly
// Returns a reference to the selected unvisited neighbor
Cell & chooseStep(Cell & currentCell, Maze & myMaze) {
    RandGen rand;
    vector<Cell> possibleSteps; 
    int chosenStep;

    // Check all neighbors and push them to the possibleSteps vector
    if (currentCell.u == 0 && myMaze.maze[currentCell.y + 1][currentCell.x].isVisited == false) 
            possibleSteps.push_back(myMaze.maze[currentCell.y + 1][currentCell.x]);
    if (currentCell.d == 0 && !myMaze.maze[currentCell.y - 1][currentCell.x].isVisited) 
            possibleSteps.push_back(myMaze.maze[currentCell.y - 1][currentCell.x]);
    if (currentCell.l == 0 && !myMaze.maze[currentCell.y][currentCell.x - 1].isVisited)
        if (myMaze.maze[currentCell.y][currentCell.x - 1].isVisited == false)
            possibleSteps.push_back(myMaze.maze[currentCell.y][currentCell.x - 1]);
    if (currentCell.r == 0 && !myMaze.maze[currentCell.y][currentCell.x + 1].isVisited)
        if (myMaze.maze[currentCell.y][currentCell.x + 1].isVisited == false)
            possibleSteps.push_back(myMaze.maze[currentCell.y][currentCell.x + 1]);

    // Randomly choose a step
    chosenStep = rand.RandInt(0, possibleSteps.size() - 1);

    // Mark the chosen step as visited
    myMaze.maze[possibleSteps[chosenStep].y][possibleSteps[chosenStep].x].isVisited = true;

    // Return the chosen step
    return myMaze.maze[possibleSteps[chosenStep].y][possibleSteps[chosenStep].x];
}

// Function that does things 
void addCell(Cell & currentCell, Maze & myMaze, StackTemp<Cell&>  &stack) {
    // Pick the cell to be visited using chooseNeighbor() function
    Cell &cellToBeVisited = chooseNeighbor(currentCell, myMaze);

    // Knock down the walls between the current cell and the cell to be visited using knockDownWall() function
    knockDownWall(currentCell, cellToBeVisited);

    // Mark the cell to be visited as visited
    cellToBeVisited.isVisited = true;

    // Push the cell to be visited to the stack
    stack.push(cellToBeVisited);
}

// Function to generate a random maze
void generateRandomMaze(Maze & myMaze, StackTemp<Cell&> &stack) {

    // Push the initial cell to the stack
    stack.push(myMaze.maze[0][0]);

    // Mark the pushed cell as visited
    myMaze.maze[0][0].isVisited = true;

    // Push a new cell to the stack 
    addCell(stack.returnTop(), myMaze, stack);

    // While stack is not empty
    while (!(stack.isEmpty())) {
        // While the stack is not empty and there aren't any unvisited neighbors of the current cell, pop the top element of the stack
        while ((!(stack.isEmpty())) && (!(anyUnvisitedNeighbors(stack.returnTop(), myMaze)))) {
            stack.pop();
        }
        // If the stack isn't empty after all pops push a new cell to the stack 
        if (!(stack.isEmpty())) {
            addCell(stack.returnTop(), myMaze, stack);
        }
    }
}

// Function to set the isVisited member variable of all cells to false
void clearVisits(Maze & myMaze) {

    // Iterate through the maze vector
    for(int i = 0; i < myMaze.rows; i++) {
        for(int j = 0; j < myMaze.cols; j++) {
            // Mark the current cell as unvisited
            myMaze.maze[i][j].isVisited = false;
        }
    }
}

// Function to find a path between the given points of the maze
void pathFinder(Maze & myMaze, StackTemp<Cell&> & pathStack, int chosenID, int entryX, int entryY, int exitX, int exitY) {

    // Push the entry point to the stack
     pathStack.push(myMaze.maze[entryY][entryX]);

    // Mark the pushed cell as visited
    myMaze.maze[entryY][entryX].isVisited = true;

    // Push the step to the stack
     pathStack.push(chooseStep(pathStack.returnTop(), myMaze));

    // 
    while (!( pathStack.returnTop().x == exitX &&  pathStack.returnTop().y == exitY)) {
        while (!(anyUnvisitedandReachableCells( pathStack.returnTop(), myMaze))) {
             pathStack.pop();
        }
         pathStack.push(chooseStep(pathStack.returnTop(), myMaze));
    }
}

int main() {
    
    int numberOfMazes, rows, cols, chosenID, entryX, entryY, exitX, exitY; // Declare variables to be used

    // Take the number of mazes to be generated as user input
    cout << "Enter the number of mazes: ";
    cin >> numberOfMazes;

    // Take the number of rows and columns of the mazes to be generated as user input
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> rows >> cols;

    vector<Maze> mazes(numberOfMazes, Maze(rows, cols)); // Declare a 2D vector for the maze

    // For loop to generate the requested number of random mazes
    for(int ID = 1; ID <= numberOfMazes; ID++) {

        Maze &myMaze = mazes[ID - 1]; // Declare a vector to keep the generated mazes

        // Declare a stack to push the maze cells as reference 
        // so they will be modified in the maze while they are modified using the stack
        StackTemp<Cell&> mazeStack;

        generateRandomMaze(myMaze, mazeStack); // Call generateRandomMaze() function

        // Write out the details of the maze by cells in a txt file
        ofstream outMazeFile("maze_" + to_string(ID) + ".txt");
        outMazeFile << rows << " " << cols << endl;

        // Iterate through the maze vector to print details
        for(int i = 0; i < rows; i++) {
            for(int j = 0; j < cols; j++) {
                outMazeFile << "x=" << myMaze.maze[i][j].x << " y=" << myMaze.maze[i][j].y;
                outMazeFile << " l=" << myMaze.maze[i][j].l << " r=" << myMaze.maze[i][j].r;
                outMazeFile << " u=" << myMaze.maze[i][j].u << " d=" << myMaze.maze[i][j].d << endl;
            }
        }

        outMazeFile.close();  // Close the output file
    }

    // Inform the user that all mazes were generated
    cout << "All mazes are generated." << endl << endl;

    // Take the maze ID from the created mazes to check for path as user input
    cout << "Enter a maze ID between 1 to " << numberOfMazes << " inclusive to find a path: ";
    cin >> chosenID;

    // Take the x and y coordinates of the starting point of the path as user input
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> entryX >> entryY;

    // Take the x and y coordinates of the exit point of the path as user input
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> exitX >> exitY;

    Maze mazeToBeDiscovered = mazes[chosenID - 1]; // Declare a variable to keep a copy of the mazeToBeDiscovered

    clearVisits(mazeToBeDiscovered); // Set all cells in the mazeToBeDiscovered as unvisited 

    StackTemp<Cell&> pathStack; // Declare a pathStack to push the steps
    
    pathFinder(mazeToBeDiscovered, pathStack, chosenID, entryX, entryY, exitX, exitY); // Call pathFinder() function

    // Write out the steps of the path by coordinates in a txt file
    string pathFileName = "maze_" + to_string(chosenID) + "_" + to_string(entryX) + "_" + to_string(entryY) + "_" + to_string(exitX) +"_" + to_string(exitY) + ".txt";
    ofstream outPathFile(pathFileName);

    pathStack.reverseStack(); // Reverse stack for easier print
    
    // Print the steps on the stack by popping until empty
    Cell currStep;
    while(!(pathStack.isEmpty())) {
        currStep = pathStack.returnPop();
        outPathFile << currStep.x << " " << currStep.y << endl;
    }

    outPathFile.close();  // Close the output file

    return 0;
}
