#include"class.cpp"
using namespace std;


#ifndef UTILS_H
#define UTILS_H
extern vector<Cell *> cell_list;
extern vector<Net *> net_list;
extern unsigned int total_nets;
extern unsigned int total_area, area_[2];
extern unsigned int smax;
extern double upper, lower;
extern int cut, min_cut;
extern int best_index;
extern vector<int> move_sequence;
extern vector<Cell *> A;
extern vector<Cell *> B;
extern map<string, int> cellID;
extern map<int, vector<Cell *> > Bucket_List;
extern clock_t start;



Net* split(string str, char del);
void random_placement(Cell *);
void cell_read(const char *argv);
void net_read(const char *argv);
void print_cells();
void print_nets();
void print_cell(Cell *);
void print_net(Net *);
void print_bucket();
void deletion();
void output_file(const char *argv);
#endif