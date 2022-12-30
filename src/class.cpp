#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<fstream>
#include<string>
#include<vector>
#include<algorithm>
#include<cmath>
#include<time.h>
#include<map>
#define NO_PART -1
#define PART_A 0
#define PART_B 1
using namespace std;

#ifndef CLASS_H
#define CLASS_H

class Net;
class Cell;

class Net{
	public:
		Net(){
			netname = 'x';
			//int size = 0;
			this->cells_in_[PART_A] = 0;
			this->cells_in_[PART_B] = 0;
		}
		string netname;
		//int size;
		int cells_in_[2];
		vector<Cell *> adj;
};

class Cell{
	public:
		Cell(int ID, string cellname, int size):ID(ID), cellname(cellname), size(size){
			this->gain = 0;
			//this->pin = 0;
			this->locked = false;
			this->partition = NO_PART;
		}
		int ID;
		string cellname;
		bool locked;
		int gain;
		int size;
		//int pin;
		int partition;
		vector<Net *> connectedNets;
};
#endif