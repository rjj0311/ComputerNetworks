# ComputerNetworks  - CMPE252

The project can be compiled by using the "make" command.
Run ./init to see the output

These functions were called in main()

cout<<"running CMPE252A - Project A\n";
C.join(1); // the first node to join the node network.
C.join(3); // the second node to join the node network.
C.join(5); // the third node
C.pretty_print(2);
C.join(2); //the join function joins and inserts the key
C.pretty_print(2);
C.pretty_print(2);
C.find(2,2);


This was the output-
running CMPE252A - Project A
server: connected to port 2001.
server: connected to port 2000.
server: connected to port 2003.
server: connected to port 2005.
Index: 2, Node ID: 3
Finger_Table[0] = 5 Finger_Table[1] = 5 Finger_Table[2] = 0 Finger_Table[3] = 0 Finger_Table[4] = 0 Finger_Table[5] = 0 Finger_Table[6] = 0 Finger_Table[7] = 0
server: connected to port 2002.
Index: 2, Node ID: 2
Finger_Table[0] = 3 Finger_Table[1] = 5 Finger_Table[2] = 0 Finger_Table[3] = 0 Finger_Table[4] = 0 Finger_Table[5] = 0 Finger_Table[6] = 0 Finger_Table[7] = 0
Index: 2, Node ID: 2
Finger_Table[0] = 3 Finger_Table[1] = 5 Finger_Table[2] = 0 Finger_Table[3] = 0 Finger_Table[4] = 0 Finger_Table[5] = 0 Finger_Table[6] = 0 Finger_Table[7] = 0
Found key: 2 at Node 2

We joined node 1, 3, and 5. The server was set to port 2000. These nodes are added with respect to node 0. Keys are also added using the join() function.
We print out the values using the pretty_print() function.
We can find the keys in the finger table using the find() function.




project inspired by github user bhanu13 as reference
