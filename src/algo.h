#include"utils.h"
using namespace std;

#ifndef ALGO_H
#define ALGO_H
extern clock_t start, end;

void reset_gains();
int set_cell_gain(Cell *);
bool check_area_constraint(bool);
Cell* move_legal_max_gain_cell_and_update_area();
void update_area(Cell *);
void update_gain_and_bucket_list(Cell *);
bool EARLY_STOP();
void get_best_solution();
void FM_ALGORITHM();
#endif