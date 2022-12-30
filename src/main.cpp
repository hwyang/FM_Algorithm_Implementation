#include"utils.h"
#include"algo.h"
using namespace std;

vector<Cell *> cell_list;
vector<Net *> net_list;
//unsigned int total_cells;
//unsigned int total_nets;
unsigned int total_area = 0;
unsigned int smax = 0;
double upper = 0.0;
double lower = 0.0;
unsigned int area_[2] = {0};
int cut = 0, min_cut = 0;
int best_index = -1;
vector<int> move_sequence;
vector<Cell *> A;
vector<Cell *> B;
map<string, int> cellID;
//map<int, string> cellName;
map<int, vector<Cell *> > Bucket_List;
clock_t start;



int main(int argc, char *argv[]){
	//if(argc < 4) cout << "required 3 inputs files";
	//for(int i = 0; i < argc; i++){
	//	cout << "[" << i << "]: " << argv[i] << endl;
	//}
	start = clock();

	cell_read(argv[2]);
	net_read(argv[1]);
	//printf("I/O time: %f\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);
	// print_nets();
	// print_cells();
	// cout << area_[PART_A] << ' ' << area_[PART_B] << ' ' << total_area;
	//start = clock();
	cout << "INIT CUT = " << cut << endl;
	for(int i = 0; i < 100; i++){
		if((((double) (clock() - start)) / CLOCKS_PER_SEC) >= 280) break;
		int difference = cut;
		FM_ALGORITHM();
		difference -= cut;
		if(difference == 0) break;
	}
	cout << "MIN CUT = " << min_cut << endl;
	//printf("Computation time: %f\n", ((double) (clock() - start)) / CLOCKS_PER_SEC);
	output_file(argv[3]);
	deletion();
	return 0;
}
