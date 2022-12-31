#include"utils.h"
#include"algo.h"
using namespace std;

void random_placement(Cell *cell){
	if (area_[PART_A] == area_[PART_B]){
		int PART = rand() % 2;
    	if(PART != 0 && PART != 1){
			cout << PART;
        	cout << "error in random placement for cell" << cell->ID << endl;
        	return;
    	}
		area_[PART] += cell->size;
        cell->partition = PART;
	}
	else if (area_[PART_A] < area_[PART_B]){
		area_[PART_A] += cell->size;
        cell->partition = PART_A;
	}
	else{
		area_[PART_B] += cell->size;
        cell->partition = PART_B;
	}
}

void cell_read(const char *argv){
	ifstream file;
	string cellname;
	unsigned int cellsize;
	int count = 0;
	file.open(argv);
	if(!file){
		cout << "Cannot read:" << argv << endl;
		return ;
	}
	total_area = 0;
	while(file >> cellname >> cellsize){
		Cell *cell = new Cell(count, cellname, cellsize);
		cell->partition = PART_B;
		area_[PART_B] += cellsize;
		total_area += cellsize;
		if (cellsize > smax) smax = cellsize;
		cell_list.push_back(cell);
		cellID[cellname] = count;
		count++;
	}
	file.close();
	return ;
}

void net_read(const char *argv){
	ifstream file;
	string temp;
	upper = 0.46 * total_area + smax;
    lower = 0.46 * total_area - smax;
	file.open(argv);
	if(!file){
		cout << "Cannot read:" << argv << endl;
		return;
	}
	while(file >> temp){
		file >> temp;
		Net* net = new Net();
		net->netname = temp;
		char c;
		file >> c;
		while(file >> c){
			if(c == '}') break;
			string str;
			file >> str;
			Cell *connectedCell= cell_list[cellID[c+str]];
			net->adj.push_back(connectedCell);
			if(connectedCell->partition == PART_B){
				if(!check_area_constraint(true)){
					connectedCell->partition = PART_A;
					area_[PART_A] += connectedCell->size;
					area_[PART_B] -= connectedCell->size;
				}
			}
			net->cells_in_[connectedCell->partition]++;
			connectedCell->connectedNets.push_back(net);
		}
		net_list.push_back(net);
		if(net->cells_in_[PART_A] * net->cells_in_[PART_B]) cut++;
	}
	file.close();
	return;
}

void print_cells(){
	printf("| CELL ID | CELL NAME | CELL SIZE | CELL PART | CELL GAIN | CONNETED NETS |\n");
	for(int i = 0; i < cell_list.size(); i++){
		print_cell(cell_list[i]);
	}
	return;
}
void print_nets(){
	for(int i = 0; i < net_list.size(); i++){
		print_net(net_list[i]);
	}
	return;
}
void print_cell(Cell *cell){
	printf("%5d %11s %11d %11d %11d     ", cell->ID, cell->cellname.c_str(), cell->size, cell->partition, cell->gain);
	for(int j = 0; j < cell->connectedNets.size(); j++){
		cout << cell->connectedNets[j]->netname << ' ';
	}
	cout << endl;
}
void print_net(Net *net){
	cout << net->netname << ' ';
	cout << "|" << net->cells_in_[PART_A] << ' ' << net->cells_in_[PART_B] << ' ';
	for(int j = 0; j < net->adj.size(); j++){
		cout << net->adj[j]->cellname << ' ';
	}
	cout << endl;
}
void print_bucket(){
	map<int, vector<Cell *> >::reverse_iterator iter;
	cout << "BUCKET LIST: \n";
	for(iter = Bucket_List.rbegin(); iter != Bucket_List.rend(); iter++){
		cout << "gain = " << iter->first << ": ";
		for(int i = 0; i < iter->second.size();i++){
			cout << iter->second[i]->cellname << ' ';
		}
		cout << endl;
	}
}

void deletion(){
	for(int i = 0; i < net_list.size(); i++){
		delete net_list[i];
	}
	for(int i = 0; i < cell_list.size(); i++){
		delete cell_list[i];
	}
}

void output_file(const char *argv){
	ofstream file;
    for(auto& cur_cell : cell_list){
        if(cur_cell->partition == PART_A){
            A.push_back(cur_cell);
        }
        else if(cur_cell->partition == PART_B){
            B.push_back(cur_cell);
        }
    }
	file.open(argv);
	if(!file){
		cout << "Cannot write:" << argv << endl;
		return;
	}
	file << "cut_size " << min_cut << endl;
	file << "A " << A.size() << endl;
	for(int i = 0; i < A.size(); i++){
		file << A[i]->cellname << endl;
	}
	file << "B " << B.size() << endl;
	for(int i = 0; i < B.size(); i++){
		file << B[i]->cellname << endl;
	}
	file.close();
}