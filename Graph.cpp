#include "Graph.h"

Graph::Graph(std::string fileName, std::ostream &outPut) {
    this->m_fileName = fileName;
    std::ifstream inputFile(this->m_fileName);

    if(inputFile.is_open()){
        // grab the number of nodes and edges from the file
        inputFile >> this->numNodes >> this->numEdges;

        // change the size of the vectors to the number of edges/nodes
        this->edges.resize(static_cast<unsigned long>(this->numEdges));
        this->matrix.resize(static_cast<unsigned long>(this->numNodes));
        this->nodeDegrees.resize(static_cast<unsigned long>(this->numNodes));

        // initialize the matrix and resize the rows to the number of nodes
        for(int toCol = 0; toCol < this->numNodes; toCol++){
            this->matrix[toCol].resize(static_cast<unsigned long>(this->numNodes));

            for(int fromRow = 0; fromRow < this->numNodes; fromRow++){
                this->matrix[toCol][fromRow] = -1;
            }
        }

        // initialize array of node degrees
        for(int i = 0; i < numNodes; ++i) {
            nodeDegrees[i] = 0;
        }

        int count = 0;
        char fromTemp = '\0';
        char toTemp = '\0';
        // read in the edges from the file
        while(count != numEdges){
            inputFile >> fromTemp >> toTemp;
            this->edges[count].set(fromTemp, toTemp);
            this->edges[count].cycleID = -1;
            this->edges[count].used = false;
            // count node degrees
            this->nodeDegrees[fromTemp - 'A']++;
            this->nodeDegrees[toTemp - 'A']++;
            count++;
        }

        inputFile.close();
    }
}

void Graph::computeTour(std::ostream &outPut) {
    if(!areNodeDegreesEven()){
        outPut << "No Euler Tour Available" << std::endl;
        return;
    }

    this->findCycles(outPut);

    this->printMatrix(outPut);
}

void Graph::printMatrix(std::ostream &outPut) {
    outPut << "Graph from " << this->m_fileName << std::endl << "  ";

    for(int i = 0; i < this->numNodes; i++){
        outPut << "  " << static_cast<char>(i + 'A') << "  ";
    }

    outPut << std::endl << "  +";

    for (int k = 0; k < this->numNodes; ++k) {
        outPut << "----+";
    }

    outPut << std::endl;

    for(int i = 0; i < this->numNodes; i++){
        outPut << static_cast<char>(i + 'A') << " |";

        for(int j = 0; j < this->numNodes; j++){
            outPut << std::setw(3) << this->matrix[i][j] << " |";
        }

        outPut << std::endl << "  +";

        for (int k = 0; k < this->numNodes; ++k) {
            outPut << "----+";
        }

        outPut << std::endl;
    }

    outPut << std::endl;

    /*
     * the matrix should look like this with '-' being used to show spacing
     *
     * -----A----B----C    D    E    F----G--
     *   +----+----+----+----+----+----+----+
     * A | -1 | -1 | -1 | -1 | -1 | -1-|--1-|
     *   +----+----+----+----+----+----+----+
     * B-|--1-|--1-|--1-|--1-|--1-|--1-|--1-|
     *   +----+----+----+----+----+----+----+
     * C-|--1-|--1-|--1-|--1-|--1-|--1-|--1-|
     *   +----+----+----+----+----+----+----+
     * D-|--1-|--1-|--1-|--1-|--1-|--1-|--1-|
     *   +----+----+----+----+----+----+----+
     * E-|--1-|--1-|--1-|--1-|--1-|--1-|--1-|
     *   +----+----+----+----+----+----+----+
     * F-|--1-|--1-|--1-|--1-|--1-|--1-|--1-|
     *   +----+----+----+----+----+----+----+
     * G-|--1-|--1-|--1-|--1-|--1-|--1-|--1-|
     *   +----+----+----+----+----+----+----+
     */
}

void Graph::findCycles(std::ostream &outPut) {
    std::vector<int> tempCycle;
    int tempStart;
    int cycleNumber = 0;

    while(!foundCycles()){
        tempStart = cycleStart();
        this->queue.resize(1);
        this->queue[0].resize(1);

        if(tempStart != -1) {
            this->queue[0][0] = tempStart;
            tempCycle = followNodes(this->edges[tempStart].fromNode, this->edges[tempStart].toNode, tempStart, cycleNumber);
            updateCycleID(tempCycle, cycleNumber);
            printCycle(tempCycle, outPut, cycleNumber);
            cycleNumber++;
        }else{
            outPut << "All edges used." << std::endl;
        }
    }
}

bool Graph::areNodeDegreesEven() {
    for(int i = 0; i < numNodes; i++) {
        if(nodeDegrees[i] % 2 != 0){
            return false;
        }
    }

    return true;
}

int Graph::cycleStart() {
    for(int i = 0; i < numEdges; i++) {
        if(this->edges[i].cycleID == -1){
            return i;
        }
    }

    return -1;
}

bool Graph::foundCycles() {
    for(int i = 0; i < numEdges; i++) {
        if(this->edges[i].cycleID == -1){
            return false;
        }
    }
    return true;
}

vector<int> &Graph::followNodes(int &startNode, int currNode, int currEdge, int currCycle) {
    if(currNode == startNode) return queue.front();

    for(int i = 0; i < numEdges; i++) {
        if((this->edges[i].fromNode == currNode || this->edges[i].toNode == currNode) && i != currEdge && this->edges[i].cycleID == -1){
            queue.emplace_back(queue.front());
            queue.back().push_back(i);
        }
    }

    queue.erase(queue.begin());

    if(this->edges[queue.front().back()].fromNode == currNode) {
        return followNodes(startNode, this->edges[queue.front().back()].toNode, queue.front().back(), currCycle);
    }else if(this->edges[queue.front().back()].toNode == currNode){
        return followNodes(startNode, this->edges[queue.front().back()].fromNode, queue.front().back(), currCycle);
    }else if(this->edges[queue.front()[queue.front().size() - 2]].fromNode == this->edges[queue.front()[queue.front().size() - 1]].fromNode){
        return followNodes(startNode, this->edges[queue.front().back()].toNode, queue.front().back(), currCycle);
    }else if(this->edges[queue.front()[queue.front().size() - 2]].fromNode == this->edges[queue.front()[queue.front().size() - 1]].toNode){
        return followNodes(startNode, this->edges[queue.front().back()].fromNode, queue.front().back(), currCycle);
    }else if(this->edges[queue.front()[queue.front().size() - 2]].toNode == this->edges[queue.front()[queue.front().size() - 1]].fromNode){
        return followNodes(startNode, this->edges[queue.front().back()].toNode, queue.front().back(), currCycle);
    }else if(this->edges[queue.front()[queue.front().size() - 2]].toNode == this->edges[queue.front()[queue.front().size() - 1]].toNode){
        return followNodes(startNode, this->edges[queue.front().back()].fromNode, queue.front().back(), currCycle);
    }
}

void Graph::printCycle(std::vector<int> &cycle, std::ostream &outPut, int cycleNumber) {
    outPut << "Cycle " << cycleNumber << ": " << static_cast<char>(this->edges[cycle[0]].fromNode + 'A');

    for(int i = 0; i < cycle.size(); i++) {
        if(this->edges[cycle[i]].fromNode == this->edges[cycle[i - 1]].toNode){
            outPut << " " << static_cast<char>(this->edges[cycle[i]].toNode + 'A');
        }else if(this->edges[cycle[i]].fromNode == this->edges[cycle[i - 1]].fromNode){
            outPut << " " << static_cast<char>(this->edges[cycle[i]].toNode + 'A');
        }else if(this->edges[cycle[i]].toNode == this->edges[cycle[i - 1]].toNode){
            outPut << " " << static_cast<char>(this->edges[cycle[i]].fromNode + 'A');
        }else if(this->edges[cycle[i]].toNode == this->edges[cycle[i - 1]].fromNode){
            outPut << " " << static_cast<char>(this->edges[cycle[i]].fromNode + 'A');
        }else if(i - 1 < 0) {
            outPut << " " << static_cast<char>(this->edges[cycle[i]].toNode + 'A');
        }
    }
    outPut << std::endl << std::endl;
}

void Graph::updateCycleID(std::vector<int> cycleArray, int cycleNumber) {
    for(int i = 0; i < cycleArray.size(); i++) {
        edges[cycleArray[i]].cycleID = cycleNumber;
        matrix[edges[cycleArray[i]].fromNode][edges[cycleArray[i]].toNode] = cycleNumber;
        matrix[edges[cycleArray[i]].toNode][edges[cycleArray[i]].fromNode] = cycleNumber;
    }
}
