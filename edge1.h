#include <sstream>
#include <assert.h>

using namespace std;

class Edge {
public:
     int toNode;  // Subscript of one endpoint in node array.  Nodes are stored as numbers, but printed as characters.
	 int fromNode; // Subscript of other endpoint in node array
	 int cycleID;  // Cycle which the edge is a member of, -1 if it is included in no cycle
	 bool used;    // true if edge is used in final tour

   // Create a string version of Edge
   // Edge endpoints are stored as numbers, but printed as characters.
   string toString() {
       ostringstream os;  // allows string to act like stream to use stream operations
       char to = toNode + 'A';
       char from = fromNode + 'A';
       os << "   " << from << "-" << to << "(" << cycleID << ")";
       return os.str();
   }  

   // if oneNode is an endpoint, return other endpoint
   int getOtherEndpoint(int oneNode) {
       if (fromNode == oneNode) return toNode;
       assert(toNode == oneNode);
       return fromNode;
   }

   // Set initial values of an edge from Node f to Node t
   void set(char from, char to) {
       fromNode = from - 'A';
       toNode = to - 'A';
       cycleID = -1;
       used = false;
       //cout << "creating Edge " << toString()<<endl;
   }
};