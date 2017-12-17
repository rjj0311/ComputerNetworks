// Tanvir Heer
// Rohan Jobanputra

#include "node.h"

int main()
{
	node C;

  cout<<"running CMPE252A - Project A\n";
	C.join(1); // the first node to join the node network.
	C.join(3); // the second node to join the node network.
	C.join(5); // the third node
	C.pretty_print(2);
	C.join(2); //the join function joins and inserts the key
	C.pretty_print(2);

	C.pretty_print(2);
	C.find(2,2);
		//C.pretty_print(3);

	//C.join(C); // the second node join the node network.
	//C.find(5); // key query
	return 0;
}
