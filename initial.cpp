// Tanvir Heer
// Rohan Jobanputra

#include "node.h"
#include "server_side.cpp"
#include "helper_functions.cpp"

//node setup
node::node()
{
	detect = 0;
	Node new_node;
	new_node.value = 0;
	new_node.valid = 1;

	for(int i = 0; i < BITS; i++)
	{
		new_node.Finger_Table[i] = 0;
	}

	for(int i = 0; i < pow(2,BITS); i++)
	{
		new_node.keys.push_back(i);
	}
	Peer * p = new Peer(this, NULL, new_node.value);
	pthread_t server_t;
	pthread_create(&server_t, NULL, server_init_callback, p);
	nodes.push_back(new_node);
	center.push_back(1);
}

// node deconstructor
node::~node()
{
	return;
}

// print function
void node::pretty_print(int index)
{
	cout<<"Index: "<<index<<", Node ID: "<<nodes[index].value<<endl;
	for(int i = 0; i < BITS; i++)
	{
		cout<<"Finger_Table["<<i<<"] = "<<nodes[index].Finger_Table[i]<<" ";
	}
	cout<<endl;
}


//join and insert function
void node::join(int value)
{
	if(node_check(value))
	{
		cout<<"This node already exists. Add another node"<<endl;
		return;
	}
	Node new_node;
	new_node.value = value;
	new_node.valid = 1;
	setup_finger_table(&new_node);
	add_node(new_node);
	Peer * p = new Peer(this, NULL, value);
	pthread_t listen_t;
	pthread_create(&listen_t, NULL, server_init_callback, p);
	request_update_finger_table_join(value);
	insert(value);
	request_insert(value);
	return;
}


//find function
void node::find(int value, int key)
{
	int node_idx = find(value);
	for (size_t i = 0; i < (nodes[node_idx]).keys.size(); ++i)
	{
		if(key == (nodes[node_idx]).keys[i])
		{
			cout<<"Found key: "<<key<<" at Node "<<value<<endl;
			return;
		}
	}

	int largest_succ = -1;
  	vector<int> v(nodes[node_idx].Finger_Table, nodes[node_idx].Finger_Table + BITS);
  	sort(v.begin(), v.end());

	for (int i = 0; i < BITS; i++)
	{
		if(v[i] <= key)
			largest_succ = i;
		else
			break;
	}

	cout<<"Searching for "<<key<<" on node "<<value<<endl;

	if(largest_succ == -1 || ( (value < key) && (v[largest_succ] < value) ) )
	{
		send_update(value, nodes[node_idx].Finger_Table[0], "find @" + int_to_string(key));
	}
	else
	{
		send_update(value, v[largest_succ], "find @" + int_to_string(key));
	}

	return;
}
