#include <iostream>
#include <iomanip>
#include "graph.h"
#include <assert.h>
#include <fstream>

using namespace std;

int main () {
    ofstream fout;
	fout.open("tourOut.txt");
	assert(fout);

	Graph g("prog7A.txt", fout);    // originally fout
	g.computeTour(cout);  // If I want the output to appear on console, I just make the parameter "cout"

    Graph g1("prog7B.txt", fout);    // originally fout
    g1.computeTour(cout);

    Graph g2("prog7C.txt", cout);    // originally cout
    g2.computeTour(cout);

    return 0;
}