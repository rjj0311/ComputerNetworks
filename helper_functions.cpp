// Tanvir Heer
// Rohan Jobanputra


bool node::node_check(int value)
{
	for(size_t i = 0; i<nodes.size(); i++)
	{
		if(nodes[i].value == value)
			return true;
	}
	return false;
}

int node::predecessor_id(int value)
{
	int pred_idx = nodes.size() - 1;
	for(size_t i = 0; i<nodes.size(); i++)
	{
		if(nodes[i].value < value)
			pred_idx = i;
		else
			break;
	}
	return pred_idx;
}

int node::successor_id(int value)
{
	int succ_idx = 0;
	for(size_t i = 0; i<nodes.size(); i++)
	{
		if(nodes[i].value > value)
		{
			succ_idx = i;
			break;
		}
	}
	return succ_idx;
}

int node::calc_finger_table(int idx, int node_id)
{
	int peer = first_peer((node_id + (int)pow(2, idx)) % (int)pow(2, BITS));
	return peer;
}

int node::first_peer(int num)
{
	for(size_t i = 0; i<nodes.size(); i++)
	{
		if(nodes[i].value >= num)
			return nodes[i].value;
	}
	return nodes[0].value;
}

void node::setup_finger_table(Node * n)
{
	int value = n->value;
	for(size_t i = 0; i<BITS; i++)
	{
		n->Finger_Table[i] = calc_finger_table(i, value);
	}
	return;
}

void node::add_node(Node n)
{
	nodes.push_back(n);
	center.push_back(1);
	sort(nodes.begin(), nodes.end());
	return;
}


int node::find(int node_id)
{
	for(size_t i = 0; i<nodes.size(); i++)
	{
		if(nodes[i].value == node_id)
			return i;
	}
	return -1;
}

void node::request_update_finger_table(int value)
{
	for(size_t i = 0;i<nodes.size(); i++)
	{
		if(nodes[i].value != value)
		{
			send_update(value, nodes[i].value, "u");
		}
	}
}

void node::request_update_finger_table_join(int value)
{
	for(int i = 0; i<BITS; i++)
	{
		int num = (int)(value - pow(2, i));
		if(num < 0)
		{
			num = num + 256;
		}
		int pred_id = nodes[join_predecessor_id(num) ].value;
		if(pred_id == value)
		{
			pred_id = nodes[predecessor_id(pred_id)].value;
		}
		string msg = int_to_string(i) + " j";
		send_update(value, pred_id, msg);
	}
}

int node::join_predecessor_id(int value)
{
	int pred_idx = nodes.size() - 1;
	for(size_t i = 0; i<nodes.size(); i++)
	{
		if(nodes[i].value <= value)
			pred_idx = i;
		else
			break;
	}
	return pred_idx;
}



void node::request_insert(int value)
{
	int	succ_id = nodes[find(value)].Finger_Table[0];
	send_update(value, succ_id, "uk");
	return;
}

void node::update_finger_table(int new_node_id, int curr_node_id, int idx)
{
	if((nodes[find(curr_node_id)].Finger_Table[idx] != new_node_id) && /*(new_node_id == calc_ft_entry(idx, curr_node_id))*/((new_node_id >= curr_node_id && new_node_id <= calc_finger_table(idx, curr_node_id)) || (new_node_id <= curr_node_id && (((curr_node_id + (int)pow(2, idx)) % 256) <= new_node_id))))
	{
		nodes[find(curr_node_id)].Finger_Table[idx] = calc_finger_table(idx, curr_node_id);
		int pred_id = nodes[predecessor_id(curr_node_id)].value;
		if(pred_id == new_node_id)
		{
			pred_id = nodes[predecessor_id(pred_id)].value;
		}
		send_update(new_node_id, pred_id, int_to_string(idx) + " j");

	}

}

void node::insert(int value)
{
	int	pred_id = nodes[predecessor_id(value)].value;
	vector<int> v;
	for(int i = pred_id + 1; i<= value; i++)
		v.push_back(i);

	if(find(value) == 0)
	{
		v.clear();
		for(int i = 0; i<= value; i++)
			v.push_back(i);

		for(int i = pred_id + 1; i<= 255; i++)
			v.push_back(i);
	}


	nodes[find(value)].update_keys = nodes[predecessor_id(value)].keys;

	nodes[find(value)].keys = v;
	key_update(value);
	return;
}

void node::key_update(int value)
{
	int succ_id = nodes[find(value)].Finger_Table[0];
	nodes[find(succ_id)].update_keys = nodes[find(value)].keys;
	return;
}
