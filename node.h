// Tanvir Heer
// Rohan Jobanputra
#include <stdio.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include <sys/time.h>
#include <cstdlib>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "math.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <unistd.h>

#define SERVER_PORT 2000
#define BITS 8
#define IP "localhost"
#define NUMBERS "0123456789"

#define MAX_MSG_L 100
#define FD_ID 256

using namespace std;

struct Node {
	int value;
	int Finger_Table[BITS];
	int socket_table;
	int valid;

	vector<int> keys;
	vector<int> update_keys;

	bool operator < (const Node & n) const
	{
		return this->value < n.value;
	}

	bool operator == (const Node & n) const
	{
		return (this->value == n.value);
	}
};


class node
{
public:
	node();
	~node();


	//function prototypes
	int calc_finger_table(int i, int node_id);
	void join(int value);
	void find(int node, int key);
	void pretty_print(int i);
	bool node_check(int value);
	void add_node(Node n);
	void setup_finger_table(Node * n);
	void update_finger_table(int new_node_id, int curr_node_id, int i);
	void peer_connection(int node_idx, int dest_id);
	int server_connection(const char* domain_or_ip, const char* port_string);
	void request_update_finger_table(int value);
	void request_update_finger_table_join(int value);
	void insert(int value);
	vector<Node> nodes;
	vector<int> center;
	int detect;
	void request_insert(int value);
	void key_update(int value);
	int join_predecessor_id(int value);
	int first_peer(int num);
	int predecessor_id(int value);
	int successor_id(int value);
	int find(int node_id);
	void server_init(int value);
	static void * server_init_callback(void * t);
	void* get_address(struct sockaddr* sa);
	void send_update(int src_id, int dest_id, string msg);
	static int server_reply(node * C, int value, int socket);
	int receiving_server(const char* port_to_bind, int value, int(*handleConnection)(node *, int, int));
	static void * call_data(void * t);
	void data(Node * n, int peer_id);

};
