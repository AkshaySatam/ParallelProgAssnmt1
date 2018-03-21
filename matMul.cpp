#include <iostream>
#include <stdlib.h>
#include <chrono>
#include <algorithm>
#include <papi.h>
#include <cilk/cilk.h>
using namespace std;
using namespace std::chrono;

int  n = 1024;
void readInput(int  (*mat) [n][n]);
void iter_mm_ijk(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n);
void iter_mm_ikj(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n);
void iter_mm_jki(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n);
void iter_mm_jik(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n);
void iter_mm_kij(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n);
void iter_mm_kji(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n);
void initialize(int  (*z) [n][n]);

int  main2(){
	int  x [n][n];
	int  y [n][n];
	int  z [n][n];
	int  (*p_x) [n][n] = &x;
	int  (*p_y) [n][n] = &y;
	int  (*p_z) [n][n] = &z;
	//int events[3] = {PAPI_L1_TCM,PAPI_L2_TCM,PAPI_L3_TCM};
	//long long values[3];
	int events[1] = {PAPI_L1_TCM};
	long long values[1];
	int ret;	

	if (PAPI_num_counters() < 2) {
   		fprintf(stderr, "No hardware counters here, or PAPI not supported.\n");
   		exit(1);
	}
	
	readInput(p_x);
	readInput(p_y);
	initialize(p_z);
	if ((ret = PAPI_start_counters(events, 1)) != PAPI_OK) {
   		fprintf(stderr, "PAPI failed to start counters: %s\n", PAPI_strerror(ret));
   		exit(1);
	}
	
	iter_mm_ijk(p_z,p_x,p_y,n);

	//adInput(p_x);
	//adInput(p_y);
	initialize(p_z);
	iter_mm_ikj(p_z,p_x,p_y,n);	

	//adInput(p_x);
	//adInput(p_y);
	initialize(p_z);
	iter_mm_jik(p_z,p_x,p_y,n);	

	//adInput(p_x);
	//adInput(p_y);
	initialize(p_z);
	iter_mm_jki(p_z,p_x,p_y,n);	

	//readInput(p_x);
	//readInput(p_y);
	initialize(p_z);
	iter_mm_kij(p_z,p_x,p_y,n);	

	//readInput(p_x);
	//readInput(p_y);
	initialize(p_z);
	iter_mm_kji(p_z,p_x,p_y,n);
	
	if ((ret = PAPI_read_counters(values, 1)) != PAPI_OK) {
		fprintf(stderr, "PAPI failed to read counters: %s\n", PAPI_strerror(ret));
		exit(1);
	}
	
	cout<<"L1 : "<<values[0]<<" L2: "<<values[0]<<" L3: "<<values[0]<<endl;
	cout<<"Done with multiplication";
	return 0;
}

void readInput(int  (*mat) [n][n]){
	int  min = 10;
	int  max = 100;
//	cout<<"Input Matrix";
	//Code referred from : https://stackoverflow.com/questions/5008804/generating-random-int eger-from-a-range
	for(int  i=0;i<n;i++){
	for(int  j=0;j<n;j++){
	int  randInt = min + (rand() % static_cast<int>(max - min + 1));
		(*mat)[i][j]=randInt;
	//	(*mat)[i][j]=j;
	//	cout<<" "<<(*mat)[i][j]<<" ";
	//	(*mat)[i][j]=1;
	}
	//	cout<<endl;
	}
}

void printMatrix(int (*z) [n][n]){
	
	for(int  i=0;i<n;i++){
	for(int  j=0;j<n;j++){
		cout<<" "<<(*z)[i][j]<<" ";
	}
		cout<<endl;
	}
}
void initialize(int  (*z) [n][n]){
	
	for(int  i=0;i<n;i++){
	for(int  j=0;j<n;j++){
		(*z)[i][j]=0;
	}
	}
}
void __attribute__ ((optimize("O0"))) iter_mm_ijk(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n){
	int op [n][n];
	initialize(&op);
	auto start = high_resolution_clock::now();
	for(int  i=0;i<n;i++){
	for(int  j=0;j<n;j++){
	for(int  k=0;k<n;k++){
	//	(*z)[i][j]= (*z)[i][j] + (*x)[i][k] * (*y)[k][j];
		op[i][j]= op[i][j] + (*x)[i][k] * (*y)[k][j];
	}
	}
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout <<"Matrix ijk: "<<duration.count() << endl;
//	printMatrix(&op);
}
void __attribute__ ((optimize("O0"))) iter_mm_ikj(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n){
	int op [n][n];
	initialize(&op);
	auto start = high_resolution_clock::now();
	for(int  i=0;i<n;i++){
	for(int  k=0;k<n;k++){
	for(int  j=0;j<n;j++){
	//	(*z)[i][j]= (*z)[i][j] + (*x)[i][k] * (*y)[k][j];
		op[i][j]= op[i][j] + (*x)[i][k] * (*y)[k][j];
	}
	}
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout <<"Matrix ikj: "<<duration.count() << endl;
//	printMatrix(&op);
}
void __attribute__ ((optimize("O0"))) iter_mm_jik(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n){
	auto start = high_resolution_clock::now();
	for(int  j=0;j<n;j++){
	for(int  i=0;i<n;i++){
	for(int  k=0;k<n;k++){
		(*z)[i][j]= (*z)[i][j] + (*x)[i][k] * (*y)[k][j];
	}
	}
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout <<"Matrix jik: "<<duration.count() << endl;
//	printMatrix(z);
}
void __attribute__ ((optimize("O0"))) iter_mm_jki(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n){
	auto start = high_resolution_clock::now();
	for(int  j=0;j<n;j++){
	for(int  k=0;k<n;k++){
	for(int  i=0;i<n;i++){
		(*z)[i][j]= (*z)[i][j] + (*x)[i][k] * (*y)[k][j];
	}
	}
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout <<"Matrix jki: "<<duration.count() << endl;
//	printMatrix(z);
}
void __attribute__ ((optimize("O0"))) iter_mm_kij(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n){
	auto start = high_resolution_clock::now();
	for(int  k=0;k<n;k++){
	for(int  i=0;i<n;i++){
	for(int  j=0;j<n;j++){
		(*z)[i][j]= (*z)[i][j] + (*x)[i][k] * (*y)[k][j];
	}
	}
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout <<"Matrix kij: "<<duration.count() << endl;
//	printMatrix(z);
}
void __attribute__ ((optimize("O0"))) iter_mm_kji(int  (*z) [n][n],int (*x) [n][n],int (*y) [n][n],int n){
	auto start = high_resolution_clock::now();
	for(int  k=0;k<n;k++){
	for(int  j=0;j<n;j++){
	for(int  i=0;i<n;i++){
		(*z)[i][j]= (*z)[i][j] + (*x)[i][k] * (*y)[k][j];
	}
	}
	}
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout <<"Matrix kji: "<<duration.count() << endl;
//	printMatrix(z);
}
