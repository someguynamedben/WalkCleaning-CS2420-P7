#ifndef WALKCLEANING_GRAPH_H
#define WALKCLEANING_GRAPH_H

#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <iostream>
#include <iomanip>
#include <queue>

#include "edge1.h"

class Graph {
public:
    Graph(std::string fileName, std::ostream &outPut);
    void computeTour(std::ostream &outPut);    // finds Euler tour
private:
    std::vector<Edge> edges;    // list/record of edges from the file
    std::vector<std::vector<int>> matrix;   // cycle matrix
    std::vector<int> nodeDegrees;
    int numEdges;   // total edges from file
    int numNodes;   // total nodes form file
    std::string m_fileName;
    std::vector<std::vector<int>> queue;    // queue of edges to follow
    void findCycles(std::ostream &outPut);
    int cycleStart();
    bool foundCycles();
    vector<int> &followNodes(int &startNode, int currNode, int currEdge, int currCycle);
    void printMatrix(std::ostream &outPut); //print the graph
    bool areNodeDegreesEven();
    void printCycle(std::vector<int> &cycle, std::ostream &outPut, int cycleNumber);
    void updateCycleID(std::vector<int> cycleArray, int cycleNumber);
};


#endif //WALKCLEANING_GRAPH_H
