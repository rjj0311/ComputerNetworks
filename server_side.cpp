// Tanvir Heer
// Rohan Jobanputra

// peer architecture setup
struct Peer {
	node * c;
	Node * n;
	int peer_id;

	Peer(node * chr, Node * N, int value)
	{
		c = chr;
		n = N;
		peer_id = value;
	}
};

// int to string function
string int_to_string(int num)
{
    stringstream ss;
    ss << num;
    return ss.str();
};

//get socket address
void* node::get_address(struct sockaddr* sa)
{
	if(sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	else
		return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


//server reply function
int node::server_reply(node * C, int value, int socket)
{
	char buffer[MAX_MSG_L];

	memset(buffer, 0, MAX_MSG_L);
	if(recv(socket, buffer, MAX_MSG_L, 0) == -1)
		return -1;

	string response = string(buffer);
	int index_id = response.find_first_of(NUMBERS);
	int space_index = response.find_first_of(' ');
	int update_index = response.find_first_of("u");
	int index_update_k = response.find_first_of("k");
	int find_index = response.find_first_of("find");
	int join_index = response.find_first_of("j");

	int src_id = atoi(&(response[index_id]));

	int checkfail_index = response.find_first_of("S");
	int fail_index = response.find_first_of("R");

	if(index_update_k >= 0)
	{
		C->insert(value);
	}
	else if(update_index >= 0)
	{
		C->setup_finger_table(&(C->nodes[C->find(value)]));
	}
	else if(find_index >= 0)
	{
		int idx_atr = response.find_first_of('@');
		int key = atoi(&(response[idx_atr + 1]));
		C->find(value, key);
	}
	else if(checkfail_index >= 0)
	{
		C->setup_finger_table(&(C->nodes[C->find(value)]));
		C->send_update(value, FD_ID, "R");
	}
	else if(fail_index >= 0)
	{
		C->center[C->find(src_id)] = 1;
	}
	else if(join_index >= 0)
	{
		int FT_idx = atoi(&(response[space_index+1]));
		C->update_finger_table(src_id, value, FT_idx);
	}
	else
	{
		cout<<response<<endl;
	}

	return 0;
}


//server callback
void * node::server_init_callback(void * t)
{
	Peer * p = (Peer *)t;
	node * c = p->c;
	c->server_init(p->peer_id);
	delete p;
	p = NULL;
	pthread_exit(NULL);
}

//setting up the server
void node::server_init(int value)
{
	string port = int_to_string(SERVER_PORT + value);
	char * port_ = &port[0u];

	receiving_server((char *)port_, value, server_reply);
	pthread_exit(NULL);
}


//server on the receiving end
int node::receiving_server(const char* port_to_bind, int value, int(*handleConnection)(node *, int, int))
{
	int sockfd, new_fd;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	if((rv = getaddrinfo(NULL, port_to_bind, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			perror("server: socket");
			continue;
		}
		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			perror("setsockopt");
			return -1;
		}
		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			perror("server: bind");
			continue;
		}
		break;
	}

	if(p == NULL)
	{
		fprintf(stderr, "server: failed to bind\n");
		return -1;
	}
	freeaddrinfo(servinfo);

	if(listen(sockfd, 10) == -1)
	{
		perror("listen");
		return -1;
	}

	printf("server: connected to port %s.\n", port_to_bind);
	if(value != 256)
		nodes[find(value)].socket_table = sockfd;

	while(value == 256 || nodes[find(value)].valid == 1)
	{
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr*)&their_addr, &sin_size);
		if(new_fd == -1)
		{
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family, get_address((struct sockaddr*)&their_addr), s, sizeof s);

		handleConnection(this, value, new_fd);

		close(new_fd);
	}
	close(sockfd);
	cout<<"Closing server "<<value<<endl;
	pthread_exit(NULL);
}

//delay channel
int delay_channel()
{
	return ((rand()%10) + 1);
}


//send new update to port
void node::send_update(int src_id, int dest_id, string msg)
{
	string port = int_to_string(SERVER_PORT + dest_id);
	char *port_ = &port[0u];
	int socket = server_connection(IP, port_);

	msg = int_to_string(src_id) + " " + msg;

	if(socket != -1)
	{
		char *s_msg = &msg[0u];

		if(src_id != 256 || dest_id != 256)
			usleep(delay_channel()*120000);

		if(send(socket, s_msg, msg.length(), 0) == -1)
		{
			perror("send");
		}
	}
	else
		cout<<"Unable to connect P"<<src_id<<" to P"<<dest_id<<endl;
	close(socket);
	return;
}

//get data from node
void * node::call_data(void * t)
{
	Peer * p = (Peer *)t;
	node * c = p->c;
	c->data(p->n, p->peer_id);
	delete p;
	p = NULL;
	pthread_exit(NULL);
}

//data node
void node::data(Node * n, int peer_id)
{
	int bytes_received;
	char recv_msg[30];
	memset(recv_msg, 0, 30);
	int socket = -1;

	while(socket != -1)
	{
		string cmd;
		if((bytes_received = recv(socket, recv_msg, 29, 0)) == -1)
		{
			perror("recv");
			pthread_exit(NULL);
		}
		if(recv_msg[0] != '\0')
		{
			cout<<"P"<<n->value<<": "<<recv_msg<<" from "<<peer_id<<endl;
			cmd = recv_msg;
			memset(recv_msg, 0, 30);
		}

	}
	cout<<"Exiting the receiving thread at P"<<n->value<<endl;
	pthread_exit(NULL);
}

//peer connection in the server
void node::peer_connection(int node_idx, int dest_id)
{
	string port = int_to_string(SERVER_PORT + dest_id);
	char *port_ = &port[0u];
	int server_socket = server_connection(IP, port_);

	int src_id = nodes[node_idx].value;
	if(server_socket < 0)
	{
		cout<<"Unable to connect "<<src_id<<" to peer "<<dest_id<<endl;
		return;
	}

	cout<<"Connected P"<<src_id<< " to P"<<dest_id<<endl;

	Peer * p = new Peer(this, &nodes[node_idx], dest_id);

	pthread_t t_recv;
	pthread_create(&t_recv, NULL, call_data, p);
	return;
}


// server connection setup
int node::server_connection(const char* domain_or_ip, const char* port_string)
{
	struct addrinfo hints, *servinfo, *p;
	int rv, sockfd;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((rv = getaddrinfo(domain_or_ip, port_string, &hints, &servinfo)) != 0)
	{
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next)
	{
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			continue;
		}

		if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			continue;
		}
		break;
	}

	if(p == NULL)
	{
		fprintf(stderr, "Failed to connect\n");
		return -1;
	}
	freeaddrinfo(servinfo);
	return sockfd;
}
