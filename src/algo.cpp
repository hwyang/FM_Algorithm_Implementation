#include"algo.h"
using namespace std;

bool cmp(const Cell* c1, const Cell* c2){
    return c1->pin < c2->pin;
}

void reset_gains(){
    Bucket_List.clear();
    for(auto& cur_cell : cell_list){
        cur_cell->gain = 0;
        cur_cell->locked = false;
        set_cell_gain(cur_cell);
        Bucket_List[cur_cell->gain].push_back(cur_cell);
    }
}
int set_cell_gain(Cell *cell){
    for(auto& cur_net : cell->connectedNets){
        if(cur_net->cells_in_[cell->partition] == 1) cell->gain++;
        if(cur_net->cells_in_[!(cell->partition)] == 0) cell->gain--;
    }
}
bool check_area_constraint(bool option){
    if(option == true){
        if(area_[PART_B] > upper || area_[PART_B] < lower){
            return false;
        }
        else{
            return true;
        }
    }
    else{
        if((double)abs((int)area_[PART_A] - (int)area_[PART_B]) < ((double)total_area)/10){
            return true;
        }
        else{
            return false;
        }
    }
}
Cell* move_legal_max_gain_cell_and_update_area(){
    map<int, vector<Cell *> >::reverse_iterator iter;
    Cell *moved_cell = NULL;
    Cell *cur_cell = NULL;
    vector<Cell *> *cur_bucket_vector_ptr = NULL;
    int bucket_vector_size = 0;
    for(iter = Bucket_List.rbegin(); iter != Bucket_List.rend(); iter++){
        cur_bucket_vector_ptr = &iter->second;
        bucket_vector_size = cur_bucket_vector_ptr->size();
        //sort(cur_bucket_vector_ptr->begin(), cur_bucket_vector_ptr->end(), cmp);
        for(int i = bucket_vector_size-1; i >= 0; i--){
            cur_cell = (*cur_bucket_vector_ptr)[i];
            if(cur_cell->locked){
                continue;
            }
            cur_cell->partition = !(cur_cell->partition);                                           //try to move cell
            update_area(cur_cell);
            if(!check_area_constraint(false)){                                                      //illegal
                cur_cell->partition = !(cur_cell->partition);                                       //resume cell move
                update_area(cur_cell);
            }
            else{                                                                                   //legal
                cur_cell->locked = true;
                moved_cell = cur_cell;
                (*cur_bucket_vector_ptr).erase(cur_bucket_vector_ptr->begin()+i);
                
                return moved_cell;
            }
        }
    }
    return moved_cell;
}
void update_area(Cell *move_this_cell){
    area_[move_this_cell->partition] += move_this_cell->size;
    area_[!(move_this_cell->partition)] -= move_this_cell->size;
}
void update_gain_and_bucket_list(Cell *moved_cell){
    Net *cur_net = NULL;
    Cell *cur_cell = NULL;
    vector<Cell *> *cur_bucket_vector_ptr = NULL;
    int bucket_vector_size = 0;
    int TO = moved_cell->partition;
    int FROM = !(moved_cell->partition);
    for(auto& cur_net : moved_cell->connectedNets){
        /********************************************find critical net before update criticality******************************************/
        if(cur_net->cells_in_[TO] == 0){
            //increase gains of all free cells on net i
            for(auto& cur_cell : cur_net->adj){
                if(cur_cell->locked) continue;
                else{
                    //find this free cell (moved_cell->connectedNets[i]->adj[j]) in bucket list
                    cur_bucket_vector_ptr = &Bucket_List[cur_cell->gain];
                    bucket_vector_size = cur_bucket_vector_ptr->size();
                    for(int k = bucket_vector_size-1; k >= 0; k--){
                        if((*cur_bucket_vector_ptr)[k] == cur_cell){
                            (*cur_bucket_vector_ptr).erase(cur_bucket_vector_ptr->begin()+k);       // delete it from bucket list
                            cur_cell->gain++;                                                       // update gain
                            Bucket_List[cur_cell->gain].push_back(cur_cell);                        // add it
                            break;
                        }
                    }
                }
            }
        }
        else if(cur_net->cells_in_[TO] == 1){
            //decrease gain of free cell in partition T on net i
            for(auto& cur_cell : cur_net->adj){
                //only consider in partition T
                if(cur_cell->partition == TO){
                    if(cur_cell->locked) continue;
                    else{
                        //find this free cell (moved_cell->connectedNets[i]->adj[j]) in bucket list
                        cur_bucket_vector_ptr = &Bucket_List[cur_cell->gain];
                        bucket_vector_size = cur_bucket_vector_ptr->size();
                        for(int k = bucket_vector_size-1; k >= 0; k--){
                            if((*cur_bucket_vector_ptr)[k] == cur_cell){
                                (*cur_bucket_vector_ptr).erase(cur_bucket_vector_ptr->begin() + k); // delete it from bucket list
                                cur_cell->gain--;                                                   // update gain
                                Bucket_List[cur_cell->gain].push_back(cur_cell);                    // add it
                                break;
                            }
                        }
                    }
                }
            }
        }
        /**************************************************update criticality**********************************************************/

        cur_net->cells_in_[TO]++;
        cur_net->cells_in_[FROM]--;

        /********************************************find critical net after update criticality***************************************/
        if(cur_net->cells_in_[FROM] == 0){
            //decrease gains of all free cells on net i
            for(auto& cur_cell : cur_net->adj){
                if(cur_cell->locked) continue;
                else{
                    //find this free cell (moved_cell->connectedNets[i]->adj[j]) in bucket list
                    cur_bucket_vector_ptr = &Bucket_List[cur_cell->gain];
                    bucket_vector_size = cur_bucket_vector_ptr->size();
                    for(int k = bucket_vector_size-1; k >= 0; k--){
                        if((*cur_bucket_vector_ptr)[k] == cur_cell){
                            (*cur_bucket_vector_ptr).erase(cur_bucket_vector_ptr->begin() + k);     // delete it from bucket list
                            cur_cell->gain--;                                                       // update gain
                            Bucket_List[cur_cell->gain].push_back(cur_cell);                        // add it
                            break;
                        }
                    }
                }
            }
        }
        else if(cur_net->cells_in_[FROM] == 1){
            //increase gain of free cells in partition F on net i
            for(auto& cur_cell : cur_net->adj){
                //only consider in partition F
                if(cur_cell->partition == FROM){
                    if(cur_cell->locked) continue;
                    else{
                        //find this free cell (moved_cell->connectedNets[i]->adj[j]) in bucket list
                        cur_bucket_vector_ptr = &Bucket_List[cur_cell->gain];
                        bucket_vector_size = cur_bucket_vector_ptr->size();
                        for(int k = bucket_vector_size-1; k >= 0; k--){
                            if((*cur_bucket_vector_ptr)[k] == cur_cell){
                                (*cur_bucket_vector_ptr).erase(cur_bucket_vector_ptr->begin() + k); // delete it from bucket list
                                cur_cell->gain++;                                                   // update gain
                                Bucket_List[cur_cell->gain].push_back(cur_cell);                    // add it
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

bool EARLY_STOP(){
    static int early_stop_flag = 10;
    if(cut > min_cut) early_stop_flag--;
    else early_stop_flag = early_stop_flag+1 >= 10 ? 10 : early_stop_flag+1;
    if(early_stop_flag == 0) return true;
    else return false;
}

void get_best_solution(){
    for(int i = best_index + 1; i < move_sequence.size(); i++){
        for(int j = 0; j < cell_list[move_sequence[i]]->connectedNets.size(); j++){
            Net* cur_net = cell_list[move_sequence[i]]->connectedNets[j];
            cur_net->cells_in_[cell_list[move_sequence[i]]->partition]--;
            cur_net->cells_in_[!(cell_list[move_sequence[i]]->partition)]++;
        }
        area_[cell_list[move_sequence[i]]->partition] -= cell_list[move_sequence[i]]->size;
        cell_list[move_sequence[i]]->partition = !(cell_list[move_sequence[i]]->partition);
        area_[cell_list[move_sequence[i]]->partition] += cell_list[move_sequence[i]]->size;
    }
}

void FM_ALGORITHM(){
    Cell *moved_cell = NULL;
    min_cut = cut;
    best_index = 0;
    while(!move_sequence.empty()){
        move_sequence.pop_back();
    }
    reset_gains();
    while(1){
        moved_cell = move_legal_max_gain_cell_and_update_area();
        if(moved_cell == NULL){
            break;
        }
        cut -= moved_cell->gain;
        update_gain_and_bucket_list(moved_cell);
        move_sequence.push_back(moved_cell->ID);
        if(cut < min_cut){
            min_cut = cut;
            best_index = move_sequence.size() - 1;
        }
        if(EARLY_STOP()) break;
    }
    get_best_solution();
    cut = min_cut;
}