// Data Structures Final Project
// Food Delivery Routing Optimization System
// Group Members : 
// Shaheer Haq 21i-1657
// Azeem Ahmad 21i-1720
// Sheharyar Bhatti 21i-2670


#include <iostream>
#include <fstream>
#include <cstring> // For memset function
using namespace std;

const int MAX_SIZE = 100;

// Large value to initialize distances...

const int LARGE_VAL = INT_MAX;

// Grid to store node distances...

int distanceGrid[MAX_SIZE][MAX_SIZE];

// Queue Class...

class DeliveryQueue
{
private:
    int data[MAX_SIZE];
    int front, rear;

public:
    DeliveryQueue()
    {
        front = 0;
        rear = -1;
    }

    void enqueue(int value)
    {
        if (rear == MAX_SIZE - 1)
        {
            cout << "Queue is full." << endl;
            return;
        }
        data[++rear] = value;
    }

    void dequeue()
    {
        if (front > rear)
        {
            cout << "Queue is empty." << endl;
            return;
        }
        front++;
    }

    int peak()
    {
        if (front > rear)
        {
            cout << "Queue is empty." << endl;
            return -1;
        }
        return data[front];
    }

    bool isEmpty()
    {
        return front > rear;
    }
};

// Adjacency list representing the edges of the graph...

struct Path
{
    int target;
    int travelTime;
}

travelGraph[MAX_SIZE * MAX_SIZE];

// Size of each adjacency list...

int graphSize[MAX_SIZE * MAX_SIZE];

// Array to store the shortest path...

int pathDistance[MAX_SIZE * MAX_SIZE];

// Function to calculate node index...

int calculateNodeIndex(int position, int dimension)
{
    int row = (position - 1) / dimension + 1;
    int col = (position - 1) % dimension + 1;

    return (row - 1) * dimension + (col - 1);
}

// Function to initialize the graph...

void setupGraph(int size, int startX, int startY)
{
    // Initializing the distance grid...
    memset(graphSize, 0, sizeof(graphSize));
    for (int i = 0; i < size; i++)
    {
        if (i == calculateNodeIndex(startX, startY))
        {
            pathDistance[i] = 0;
        }
        else
        {
            pathDistance[i] = LARGE_VAL;
        }
    }
}

// Function to find the shortest path using BFS 

void findShortestPath(int start, int dimension)
{
    DeliveryQueue queue;
    queue.enqueue(start);
    pathDistance[start] = 0;

    while (!queue.isEmpty())
    {
        int current = queue.peak();
        queue.dequeue();

        int row = current / dimension;
        int col = current % dimension;

        // Defining the four possible directions: up, down, left, right

        int dx[] = { 0, 0, -1, 1 };
        int dy[] = { -1, 1, 0, 0 };

        for (int i = 0; i < 4; ++i)
        {
            int nextRow = row + dx[i];
            int nextCol = col + dy[i];

            // Checking if the neighbor is within grid boundaries

            if (nextRow >= 0 && nextRow < dimension && nextCol >= 0 && nextCol < dimension)
            {
                int neighborIndex = nextRow * dimension + nextCol;
                int neighborTime = distanceGrid[nextRow][nextCol];

                if (pathDistance[neighborIndex] > pathDistance[current] + 1 + neighborTime)
                {
                    pathDistance[neighborIndex] = pathDistance[current] + 1 + neighborTime;
                    queue.enqueue(neighborIndex);
                }
            }
        }
    }
}

int main()
{
    cout << endl << endl;
    cout << "*************** Welcome to Food Delivery Routing Optimization System ***************             " << endl << endl << endl;

    // Taking Input from the File

    ifstream inputFile("input.txt");
    if (!inputFile.is_open())
    {
        cout << "Failed to open the input file." << endl;
        return 1;
    }

    int totalCases;
    inputFile >> totalCases;

    while (totalCases--)
    {
        int dimension, riders, orders;
        inputFile >> dimension >> riders >> orders;
        cout << "Grid Size: " << dimension << "x" << dimension << endl;
        cout << "Number of Riders: " << riders << endl;
        cout << "Number of Orders: " << orders << endl;
        cout << "-----------------------" << endl;

        // Reading the grid...

        int gridSize = dimension * dimension;
        setupGraph(gridSize, riders, orders);

        for (int i = 0; i < orders; i++)
        {
            string restaurant;
            int location, customerOrders;

            inputFile >> restaurant >> location >> customerOrders;
            cout << "Restaurant: " << restaurant << endl;
            cout << "Location: " << location << endl;
            cout << "Number of Orders: " << customerOrders << endl << endl;
            cout << "-----------------------" << endl;

            int restaurantID = calculateNodeIndex(location, dimension);

            for (int j = 0; j < customerOrders; j++)
            {
                string dish;
                int customerLocation, prepTime;

                inputFile >> dish >> customerLocation >> prepTime;
                cout << "Dish: " << dish << endl;
                cout << "Customer Location: " << customerLocation << endl;
                cout << "Preparation Time: " << prepTime << endl;
                cout << "-----------------------" << endl << endl;


                int customerID = calculateNodeIndex(customerLocation, dimension);

                // Adding edge from restaurant to customer...

                travelGraph[restaurantID * MAX_SIZE + graphSize[restaurantID]].target = customerID;
                travelGraph[restaurantID * MAX_SIZE + graphSize[restaurantID]].travelTime = prepTime;
                graphSize[restaurantID]++;
            }
        }

        // Assuming the starting point is the first restaurant's location

        findShortestPath(calculateNodeIndex(1, dimension), dimension);

        int totalTime = 0;

        // Calculating the total time taken for all customers to get served

        for (int i = 0; i < gridSize; i++)
        {
            totalTime = max(totalTime, pathDistance[i]);
        }

        cout << "============================================================" << endl;
        cout << "Total time taken for all customers to get served: " << totalTime << endl;
        cout << "============================================================" << endl;
        cout << endl;
    }

    inputFile.close();

    return 0;
}
