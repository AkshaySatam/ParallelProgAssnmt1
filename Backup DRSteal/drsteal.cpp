// Rec-MM ( X, Y ) { X and Y are n × n matrices,
// where n = 2k for integer k ≥ 0 }
// 2. if n = 1 then
// 4. else
// 5. Z11 ← Rec-MM ( X11, Y11 ) + Rec-MM ( X12, Y21 )
// 6. Z12 ← Rec-MM ( X11, Y12 ) + Rec-MM ( X12, Y22 )
// 7. Z21 ← Rec-MM ( X21, Y11 ) + Rec-MM ( X22, Y21 )
// 8. Z22 ← Rec-MM ( X21, Y12 ) + Rec-MM ( X22, Y22 )
// 10. return Z

// https://github.com/sajjadpoores/matrix_multiplication2/blob/master/matrix%20mul.cpp



#include <iostream>
#include <chrono>
#include <cilk/cilk.h>
#include "ThreadPool.h"
#include <unistd.h>
#include <map>
using namespace std;
using namespace std::chrono;

// void matrix_mul(int **a, int **b, int **c, int n, thread_pool* tp);
// void matrix_sum(int **a,int **b, int **c, int n){
// 	for (int i = 0; i < n;i++)
// 	for (int j = 0; j < n; j++){
// 		c[i][j] = a[i][j] + b[i][j];
// 	}
// }

// void eval11(int** a11, int** b11, int** a12, int** b21, int** c11, int n){

// 	int **temp1 = new int*[(n/2)];
// 	int **temp2 = new int*[(n/2)];
// 	for (int i = 0; i < (n/2); i++){
// 		temp1[i] = new int[(n/2)];
// 		temp2[i] = new int[(n/2)];
// 	}

// 	matrix_mul(a11, b11, temp1, (n/2), NULL);
// 	matrix_mul(a12, b21, temp2, (n/2), NULL);
// 	matrix_sum(temp1, temp2, c11, (n/2));

// 	for (int i = 0; i < (n/2); i++){
// 		delete[] temp1[i];
// 		delete[] temp2[i];	
// 	}
// }

// void eval12(int** a11, int** b12, int** a12, int** b22, int** c12, int n){

// 	int **temp1 = new int*[(n/2)];
// 	int **temp2 = new int*[(n/2)];
// 	for (int i = 0; i < (n/2); i++){
// 		temp1[i] = new int[(n/2)];
// 		temp2[i] = new int[(n/2)];
// 	}

// 	matrix_mul(a11, b12, temp1, (n/2), NULL);
// 	matrix_mul(a12, b22, temp2, (n/2), NULL);
// 	matrix_sum(temp1, temp2, c12, (n/2));

// 	for (int i = 0; i < (n/2); i++){
// 		delete[] temp1[i];
// 		delete[] temp2[i];	
// 	}
// }

// void eval21(int** a21, int** b11, int** a22, int** b21, int** c21, int n){

// 	int **temp1 = new int*[(n/2)];
// 	int **temp2 = new int*[(n/2)];
// 	for (int i = 0; i < (n/2); i++){
// 		temp1[i] = new int[(n/2)];
// 		temp2[i] = new int[(n/2)];
// 	}

// 	matrix_mul(a21, b11, temp1, (n/2), NULL);
// 	matrix_mul(a22, b21, temp2, (n/2), NULL);
// 	matrix_sum(temp1, temp2, c21, (n/2));

// 	for (int i = 0; i < (n/2); i++){
// 		delete[] temp1[i];
// 		delete[] temp2[i];	
// 	}

// }


// void eval22(int** a21, int** b12, int** a22, int** b22, int** c22, int n){

// 	int **temp1 = new int*[(n/2)];
// 	int **temp2 = new int*[(n/2)];
// 	for (int i = 0; i < (n/2); i++){
// 		temp1[i] = new int[(n/2)];
// 		temp2[i] = new int[(n/2)];
// 	}

// 	matrix_mul(a21, b12, temp1, (n/2), NULL);
// 	matrix_mul(a22, b22, temp2, (n/2), NULL);
// 	matrix_sum(temp1, temp2, c22, (n/2));

// 	for (int i = 0; i < (n/2); i++){
// 		delete[] temp1[i];
// 		delete[] temp2[i];	
// 	}
// }

// void evalspec(int** a11, int** b11, int** a12, int** b21, int** c11, int n, thread_pool* tp){

// 	int **temp1 = new int*[(n/2)];
// 	int **temp2 = new int*[(n/2)];
// 	for (int i = 0; i < (n/2); i++){
// 		temp1[i] = new int[(n/2)];
// 		temp2[i] = new int[(n/2)];
// 	}

// 	matrix_mul(a11, b11, temp1, (n/2), tp);
// 	matrix_mul(a12, b21, temp2, (n/2), tp);
// 	matrix_sum(temp1, temp2, c11, (n/2));

// 	for (int i = 0; i < (n/2); i++){
// 		delete[] temp1[i];
// 		delete[] temp2[i];	
// 	}
// }

// class myjob : public job{
// public:

// 	int** a11;
// 	int** b11; 
// 	int** a12; 
// 	int** b21; 
// 	int** c11; 
// 	int n;
// 	thread_pool* tp;
// 	volatile int* sync_wait;
// 	pthread_mutex_t* sync_lock;
// 	// myjob(int id, int** _a11, int** _b11, int** _a12, int** _b21, int** _c11, int _n, thread_pool* _tp, int* _sync_wait, pthread_mutex_t* _sync_lock):

// 	// myjob(int id):job(id)
// 	// {}

// 	// myjob(int id, int** _a11, int** _b11, int** _a12, int** _b21, int** _c11, int _n, thread_pool* _tp):
// 	myjob(int id, int** _a11, int** _b11, int** _a12, int** _b21, int** _c11, int _n, thread_pool* _tp, volatile int* _sync_wait, pthread_mutex_t* _sync_lock):
// 	job(id),
// 	a11(_a11),
// 	b11(_b11),
// 	a12(_a12),
// 	b21(_b21),
// 	c11(_c11),
// 	n(_n),
// 	tp(_tp),
// 	sync_wait(_sync_wait),
// 	sync_lock(_sync_lock)	
// 	{
// 		// cout << "Constructor"<< endl;
// 	}


// 	void working(void* param){
// 		int tid = *((int *)param);
// 		// cout << "Printing from thread " << tid << " Job Id " << jobID << endl;
// 		int **temp1 = new int*[(n/2)];
// 		int **temp2 = new int*[(n/2)];
// 		for (int i = 0; i < (n/2); i++){
// 			temp1[i] = new int[(n/2)];
// 			temp2[i] = new int[(n/2)];
// 		}

// 		matrix_mul(a11, b11, temp1, (n/2), tp);
// 		matrix_mul(a12, b21, temp2, (n/2), tp);
// 		matrix_sum(temp1, temp2, c11, (n/2));

// 		for (int i = 0; i < (n/2); i++){
// 			delete[] temp1[i];
// 			delete[] temp2[i];	
// 		}

// 		cout << 
// 		// guard by lock
// 		pthread_mutex_lock(sync_lock);
// 		(*sync_wait)--;
// 		// cout << "~~~~~~--- Going to unlock from " << tid << " " << jobID << " " <<  *sync_wait << endl;
// 		pthread_mutex_unlock(sync_lock);
// 		// pthread_cond_signal(sync_cond);   
// 	}
// };

int totaljobs = 0;

// #define _PARALLEL_

// void matrix_mul(int **a, int **b, int **c, int n, thread_pool* tp){
// 	if (n == 1)
// 	{
// 		c[0][0] = a[0][0] * b[0][0];
// 	}
// 	else
// 	{
// 		int **a11 = new int*[(n/2)];
// 		int **a12 = new int*[(n/2)];
// 		int **a21 = new int*[(n/2)];
// 		int **a22 = new int*[(n/2)];

// 		int **b11 = new int*[(n/2)];
// 		int **b12 = new int*[(n/2)];
// 		int **b21 = new int*[(n/2)];
// 		int **b22 = new int*[(n/2)];

// 		int **c11 = new int*[(n/2)];
// 		int **c12 = new int*[(n/2)];
// 		int **c21 = new int*[(n/2)];
// 		int **c22 = new int*[(n/2)];



// 		for (int i = 0; i < (n/2); i++){
// 			a11[i] = new int[(n/2)];
// 			a12[i] = new int[(n/2)];
// 			a21[i] = new int[(n/2)];
// 			a22[i] = new int[(n/2)];

// 			b11[i] = new int[(n/2)];
// 			b12[i] = new int[(n/2)];
// 			b21[i] = new int[(n/2)];
// 			b22[i] = new int[(n/2)];

// 			c11[i] = new int[(n/2)];
// 			c12[i] = new int[(n/2)];
// 			c21[i] = new int[(n/2)];
// 			c22[i] = new int[(n/2)];

// 		}

// 		for (int i = 0; i < (n/2); i++){
// 			for (int j = 0; j < (n/2); j++){
// 				a11[i][j] = a[i][j];
// 				a12[i][j] = a[i][j + (n/2)];
// 				a21[i][j] = a[i + (n/2)][j];
// 				a22[i][j] = a[i + (n/2)][j + (n/2)];

// 				b11[i][j] = b[i][j];
// 				b12[i][j] = b[i][j + (n/2)];
// 				b21[i][j] = b[i + (n/2)][j];
// 				b22[i][j] = b[i + (n/2)][j + (n/2)];
// 			}
// 		}
		
// 		#ifdef _PARALLEL_

// 		volatile int* sync_wait = new int;
// 		*sync_wait = 3;
// 		pthread_mutex_t* sync_lock = new pthread_mutex_t;
// 		pthread_mutex_init(sync_lock, NULL);


// 		// cilk_spawn eval11(a11, b11, a12, b21, c11, n);
// 		myjob* j1 = new myjob(++totaljobs, a11, b11, a12, b21, c11, n, tp, sync_wait, sync_lock);
// 		tp->assignJob(j1);



// 		// cilk_spawn eval11(a11, b12, a12, b22, c12, n);
// 		myjob* j2 = new myjob(++totaljobs, a11, b12, a12, b22, c12, n, tp, sync_wait, sync_lock);
// 		tp->assignJob(j2);


// 		// cilk_spawn eval11(a21, b11, a22, b21, c21, n);
// 		myjob* j3 = new myjob(++totaljobs, a21, b11, a22, b21, c21, n, tp, sync_wait, sync_lock);
// 		tp->assignJob(j3);


// 		// cilk_spawn eval11(a21, b12, a22, b22, c22, n);
// 		evalspec(a21, b12, a22, b22, c22, n, tp);
// 		// myjob* j4 = new myjob(++totaljobs, a21, b12, a22, b22, c22, n, tp, sync_wait, sync_lock);
// 		// tp->assignJob(j4);


// 		// cout << "-- Waiting for unlock\n";
// 		while(1){
// 			//Wait
// 			pthread_mutex_lock(sync_lock);
// 			// cout << *sync_wait << endl;
// 			if(*sync_wait == 0){
// 				pthread_mutex_unlock(sync_lock);
// 				break;
// 			}
// 			else
// 			{
// 				pthread_mutex_unlock(sync_lock);
// 				// sleep(2);
// 			}
// 		}

// 		// cout << "--- Unlocked" << endl;
// 		// cilk_sync;

// 		#else

// 		// eval11(a11, b11, a12, b21, c11, n);

// 		// eval11(a11, b12, a12, b22, c12, n);

// 		// eval11(a21, b11, a22, b21, c21, n);

// 		// eval11(a21, b12, a22, b22, c22, n);		

// 		#endif

// 		for (int i = 0; i < n / 2; i++){
// 			for (int j = 0; j < n / 2; j++){
// 				c[i][j] = c11[i][j];
// 				c[i][j + (n / 2)] = c12[i][j];
// 				c[i + (n / 2)][j] = c21[i][j];
// 				c[i + (n / 2)][j + (n / 2)] = c22[i][j];
// 			}
// 		}
	

			
// 		for (int i = 0; i < (n/2); i++){
// 			delete[] a11[i];
// 			delete[] a12[i];
// 			delete[] a21[i];
// 			delete[] a22[i];

// 			delete[] b11[i];
// 			delete[] b12[i];
// 			delete[] b21[i];
// 			delete[] b22[i];

// 			delete[] c11[i];
// 			delete[] c12[i];
// 			delete[] c21[i];
// 			delete[] c22[i];
			
// 		}
// 	}
// }


// void matrix_sumn(int **a, int **c, int n){
// 	for (int i = 0; i < n;i++)
// 	for (int j = 0; j < n; j++){
// 		c[i][j] += a[i][j];
// 	}
// }

// void evaln(int** a11, int** b11, int** c11, int n){

// 	// int **temp1 = new int*[(n/2)];
// 	// int **temp2 = new int*[(n/2)];
// 	// for (int i = 0; i < (n/2); i++){
// 	// 	temp1[i] = new int[(n/2)];
// 	// 	temp2[i] = new int[(n/2)];
// 	// }

// 	matrix_mul(a11, b11, c11, (n/2), NULL);
// 	// matrix_sum(temp1, temp2, c11, (n/2));

// 	// for (int i = 0; i < (n/2); i++){
// 	// 	delete[] temp1[i];
// 	// 	delete[] temp2[i];	
// 	// }
// }


// void matrix_mul(int **a, int **b, int **c, int n){

// 	if (n == 1)
// 	{
// 		c[0][0] += a[0][0] * b[0][0];
// 	}
// 	else
// 	{
// 		int **a11 = new int*[(n/2)];
// 		int **a12 = new int*[(n/2)];
// 		int **a21 = new int*[(n/2)];
// 		int **a22 = new int*[(n/2)];

// 		int **b11 = new int*[(n/2)];
// 		int **b12 = new int*[(n/2)];
// 		int **b21 = new int*[(n/2)];
// 		int **b22 = new int*[(n/2)];

// 		int **c11 = new int*[(n/2)];
// 		int **c12 = new int*[(n/2)];
// 		int **c21 = new int*[(n/2)];
// 		int **c22 = new int*[(n/2)];



// 		for (int i = 0; i < (n/2); i++){
// 			a11[i] = new int[(n/2)];
// 			a12[i] = new int[(n/2)];
// 			a21[i] = new int[(n/2)];
// 			a22[i] = new int[(n/2)];

// 			b11[i] = new int[(n/2)];
// 			b12[i] = new int[(n/2)];
// 			b21[i] = new int[(n/2)];
// 			b22[i] = new int[(n/2)];

// 			c11[i] = new int[(n/2)];
// 			c12[i] = new int[(n/2)];
// 			c21[i] = new int[(n/2)];
// 			c22[i] = new int[(n/2)];

// 		}

// 		for (int i = 0; i < (n/2); i++){
// 			for (int j = 0; j < (n/2); j++){
// 				a11[i][j] = a[i][j];
// 				a12[i][j] = a[i][j + (n/2)];
// 				a21[i][j] = a[i + (n/2)][j];
// 				a22[i][j] = a[i + (n/2)][j + (n/2)];

// 				b11[i][j] = b[i][j];
// 				b12[i][j] = b[i][j + (n/2)];
// 				b21[i][j] = b[i + (n/2)][j];
// 				b22[i][j] = b[i + (n/2)][j + (n/2)];

// 				c11[i][j] = 0;
// 				c12[i][j] = 0;
// 				c21[i][j] = 0;
// 				c22[i][j] = 0;
// 			}
// 		}


// 		cilk_spawn matrix_mul(a11, b11, c11, n/2);

// 		cilk_spawn matrix_mul(a11, b12, c12, n/2);

// 		cilk_spawn matrix_mul(a21, b11, c21, n/2);

// 		cilk_spawn matrix_mul(a21, b12, c22, n/2);			

// 		cilk_sync;

// 		cilk_spawn matrix_mul(a12, b21, c11, n/2);

// 		cilk_spawn matrix_mul(a12, b22, c12, n/2);

// 		cilk_spawn matrix_mul(a22, b21, c21, n/2);

// 		cilk_spawn matrix_mul(a22, b22, c22, n/2);	

// 		cilk_sync;

// 		for (int i = 0; i < n / 2; i++){
// 			for (int j = 0; j < n / 2; j++){
// 				c[i][j] += c11[i][j];
// 				c[i][j + (n / 2)] += c12[i][j];
// 				c[i + (n / 2)][j] += c21[i][j];
// 				c[i + (n / 2)][j + (n / 2)] += c22[i][j];
// 			}
// 		}		

// 		for (int i = 0; i < (n/2); i++){
// 			delete[] a11[i];
// 			delete[] a12[i];
// 			delete[] a21[i];
// 			delete[] a22[i];

// 			delete[] b11[i];
// 			delete[] b12[i];
// 			delete[] b21[i];
// 			delete[] b22[i];

// 			delete[] c11[i];
// 			delete[] c12[i];
// 			delete[] c21[i];
// 			delete[] c22[i];
// 		}

// 	}

// }


// ---------------------------------------
// void matrix_mul(int **a, int **b, int **c, int ai, int aj, int bi, int bj, int ci, int cj,int n){

// 	if (n == 1)
// 	{
// 		c[ci][cj] += a[ai][aj] * b[bi][bj];
// 	}
// 	else
// 	{

// 		cilk_spawn matrix_mul(a, b, c, ai, aj, bi, bj, ci, cj, n/2);

// 		cilk_spawn matrix_mul(a, b, c, ai, aj, bi, bj+n/2, ci, cj + n/2, n/2);

// 		cilk_spawn matrix_mul(a, b, c, ai+n/2, aj, bi, bj, ci+n/2, cj, n/2);

// 		cilk_spawn matrix_mul(a, b, c, ai+n/2, aj, bi, bj+n/2, ci+n/2, cj+n/2, n/2);			

// 		cilk_sync;

// 		cilk_spawn matrix_mul(a, b, c, ai, aj+n/2, bi+n/2, bj, ci, cj, n/2);

// 		cilk_spawn matrix_mul(a, b, c, ai, aj+n/2, bi+n/2, bj+n/2, ci, cj + n/2, n/2);

// 		cilk_spawn matrix_mul(a, b, c, ai+n/2, aj+n/2, bi+n/2, bj, ci+n/2, cj, n/2);

// 		cilk_spawn matrix_mul(a, b, c, ai+n/2, aj+n/2, bi+n/2, bj+n/2, ci+n/2, cj+n/2, n/2);	

// 		cilk_sync;

// 	}

// }
// -----------------------------------------------------------

// class secondjob: job{
// public:
// 	int* sync2;
// 	int** a;
// 	int** b; 
// 	int** c; 
// 	int ai; int aj; int bi; int bj; int ci; int cj;
// 	int n;
// 	thread_pool* tp;
// 	pthread_mutex_t* sync_lock;

// 	secondjob(int id, int** _a, int** _b, int** _c, int _ai, int _aj, int _bi, int _bj, int _ci, int _cj, int _n, int* _sync2, thread_pool* _tp, pthread_mutex_t* _sync_lock)
// 	:job(id)
// 	{
// 		sync2 = _sync2;
// 		a = _a;
// 		b = _b; 
// 		c = _c; 
// 		ai = _ai; 
// 		aj = _aj; 
// 		bi = _bi; 
// 		bj = _bj; 
// 		ci = _ci; 
// 		cj = _cj;
// 		n = _n;
// 		tp = _tp;
// 		sync_lock = _sync_lock;		
// 	}

// 	void working(void* param){

// 		if (n == 1)
// 		{
// 			c[ci][cj] += a[ai][aj] * b[bi][bj];
// 		}
// 		else
// 		{
// 			//create _a12, _b21, _c11

// 			int* _sync2 = new int;
// 			*_sync2 = 4;
// 			pthread_mutex_t* _sync_lock = new pthread_mutex_t;
// 			pthread_mutex_init(_sync_lock, NULL);			

// 			//asign 4 jobs
// 			secondjob* j1 = new secondjob(++totaljobs, a, b, c, ai, aj+n/2, bi+n/2, bj, ci, cj, n/2, _sync2, tp, _sync_lock);
// 			tp->assignJob(j1);	

// 			secondjob* j2 = new secondjob(++totaljobs, a, b, c, ai, aj+n/2, bi+n/2, bj+n/2, ci, cj + n/2, n/2, _sync2, tp, _sync_lock);
// 			tp->assignJob(j2);	

// 			secondjob* j3 = new secondjob(++totaljobs, a, b, c, ai+n/2, aj+n/2, bi+n/2, bj, ci+n/2, cj, n/2, _sync2, tp, _sync_lock);
// 			tp->assignJob(j3);	

// 			secondjob* j4 = new secondjob(++totaljobs, a, b, c, ai+n/2, aj+n/2, bi+n/2, bj+n/2, ci+n/2, cj+n/2, n/2, _sync2, tp, _sync_lock);
// 			tp->assignJob(j4);						

// 			pthread_mutex_lock(sync_lock);
// 			*sync2--;
// 			pthread_mutex_unlock(sync_lock);
// 			if (*sync2 == 0)
// 			{
// 				// complete
// 				cout << "Complete";			
// 			}
// 		}
// 	}


// };

class superjob;
map<int*, superjob*> hashy;
pthread_mutex_t hashLock;

class superjob: public job{
public:
	int type;
	int* sync1;
	int** a;
	int** b; 
	int** c; 
	int ai; int aj; int bi; int bj; int ci; int cj;
	int n;
	thread_pool* tp;
	pthread_mutex_t* sync_lock;


	superjob(int _type, int id, int** _a, int** _b, int** _c, int _ai, int _aj, int _bi, int _bj, int _ci, int _cj, int _n, int* _sync1, thread_pool* _tp, pthread_mutex_t* _sync_lock)
	:job(id)
	{
		type = _type;
		sync1 = _sync1;
		a = _a;
		b = _b; 
		c = _c; 
		ai = _ai; 
		aj = _aj; 
		bi = _bi; 
		bj = _bj; 
		ci = _ci; 
		cj = _cj;
		n = _n;
		tp = _tp;
		sync_lock = _sync_lock;		
	}

	void firstjob(int tid){
		if (n == 1)
		{
			c[ci][cj] += a[ai][aj] * b[bi][bj];

		}
		else
		{

			//baby create _a11, _b11, _c11


			int* _sync1 = new int;
			*_sync1 = 4;
			pthread_mutex_t* _sync_lock = new pthread_mutex_t;
			pthread_mutex_init(_sync_lock, NULL);		
			//lock
			pthread_mutex_lock(&hashLock);
			hashy[_sync1] = this;
			pthread_mutex_unlock(&hashLock);

			//asign 4 jobs
			superjob* j1 = new superjob(1, ++totaljobs, a, b, c, ai, aj, bi, bj, ci, cj, n/2, _sync1, tp, _sync_lock);
			tp->assignJob(j1, tid);	

			superjob* j2 = new superjob(1, ++totaljobs, a, b, c, ai, aj, bi, bj+n/2, ci, cj + n/2, n/2, _sync1, tp, _sync_lock);
			tp->assignJob(j2, tid);	

			superjob* j3 = new superjob(1, ++totaljobs, a, b, c, ai+n/2, aj, bi, bj, ci+n/2, cj, n/2, _sync1, tp, _sync_lock);
			tp->assignJob(j3, tid);	

			superjob* j4 = new superjob(1, ++totaljobs, a, b, c, ai+n/2, aj, bi, bj+n/2, ci+n/2, cj+n/2, n/2, _sync1, tp, _sync_lock);
			tp->assignJob(j4, tid);						

		
		}


		pthread_mutex_lock(sync_lock);
		(*sync1)--;
		pthread_mutex_unlock(sync_lock);
		if (*sync1 == 0)
		{
			cout << "start 2nd part\n";
			// assign part 2
			pthread_mutex_lock(&hashLock);
			if (hashy.find(sync1) == hashy.end())
			{
				cout << "Bug while executing 2\n";
				return;
			}
			superjob* j = hashy[sync1];
			pthread_mutex_unlock(&hashLock);

			//create sub jobs


			int* _sync2 = new int;
			*_sync2 = 4;
			pthread_mutex_t* __sync_lock = new pthread_mutex_t;
			pthread_mutex_init(__sync_lock, NULL);		

			superjob* sj1 = new superjob(2, ++totaljobs, j->a, j->b, j->c, j->ai, j->aj+j->n/2, j->bi+j->n/2, j->bj, j->ci, j->cj, j->n/2, _sync2, j->tp, __sync_lock);
			tp->assignJob(sj1, tid);	

			superjob* sj2 = new superjob(2, ++totaljobs, j->a, j->b, j->c, j->ai, j->aj+j->n/2, j->bi+j->n/2, j->bj+j->n/2, j->ci, j->cj + j->n/2, j->n/2, _sync2, tp, __sync_lock);
			tp->assignJob(sj2, tid);	

			superjob* sj3 = new superjob(2, ++totaljobs, j->a, j->b, j->c, j->ai+j->n/2, j->aj+j->n/2, j->bi+j->n/2, j->bj, j->ci+j->n/2, j->cj, j->n/2, _sync2, tp, __sync_lock);
			tp->assignJob(sj3, tid);	

			superjob* sj4 = new superjob(2, ++totaljobs, j->a, j->b, j->c, j->ai+j->n/2, j->aj+j->n/2, j->bi+j->n/2, j->bj+j->n/2, j->ci+j->n/2, j->cj+j->n/2, j->n/2, _sync2, tp, __sync_lock);
			tp->assignJob(sj4, tid);		


		}
		

	}


	void secondjob(int tid){
		if (n == 1)
		{
			c[ci][cj] += a[ai][aj] * b[bi][bj];
		}
		else
		{
			//create _a12, _b21, _c11

			int* _sync2 = new int;
			*_sync2 = 4;
			pthread_mutex_t* _sync_lock = new pthread_mutex_t;
			pthread_mutex_init(_sync_lock, NULL);			

			//asign 4 jobs
			superjob* j1 = new superjob(2, ++totaljobs, a, b, c, ai, aj+n/2, bi+n/2, bj, ci, cj, n/2, _sync2, tp, _sync_lock);
			tp->assignJob(j1, tid);	

			superjob* j2 = new superjob(2, ++totaljobs, a, b, c, ai, aj+n/2, bi+n/2, bj+n/2, ci, cj + n/2, n/2, _sync2, tp, _sync_lock);
			tp->assignJob(j2, tid);	

			superjob* j3 = new superjob(2, ++totaljobs, a, b, c, ai+n/2, aj+n/2, bi+n/2, bj, ci+n/2, cj, n/2, _sync2, tp, _sync_lock);
			tp->assignJob(j3, tid);	

			superjob* j4 = new superjob(2, ++totaljobs, a, b, c, ai+n/2, aj+n/2, bi+n/2, bj+n/2, ci+n/2, cj+n/2, n/2, _sync2, tp, _sync_lock);
			tp->assignJob(j4, tid);						
		}


		pthread_mutex_lock(sync_lock);
		(*sync1)--;
		pthread_mutex_unlock(sync_lock);
		if (*sync1 == 0)
		{
			// complete
			cout << "Complete\n";			
		}
	
	}

	void working(void* param){
		int tid = *((int *)param);
		if (type == 1)
		{
			firstjob(tid);
		}
		else
		{
			secondjob(tid);
		}

	}


};




void matrix_mul(int **a, int **b, int **c, int ai, int aj, int bi, int bj, int ci, int cj,int n, thread_pool* tp){



	int* _sync1 = new int;
	*_sync1 = 4;	
	pthread_mutex_t* _sync_lock = new pthread_mutex_t;
	pthread_mutex_init(_sync_lock, NULL);		

	superjob* j1 = new superjob(1, ++totaljobs, a, b, c, ai, aj, bi, bj, ci, cj, n, _sync1, tp, _sync_lock );
	tp->assignJob(j1, 0);
	// if (n == 1)
	// {
	// 	c[ci][cj] += a[ai][aj] * b[bi][bj];
	// }
	// else
	// {

	// 	cilk_spawn matrix_mul(a, b, c, ai, aj, bi, bj, ci, cj, n/2);

	// 	cilk_spawn matrix_mul(a, b, c, ai, aj, bi, bj+n/2, ci, cj + n/2, n/2);

	// 	cilk_spawn matrix_mul(a, b, c, ai+n/2, aj, bi, bj, ci+n/2, cj, n/2);

	// 	cilk_spawn matrix_mul(a, b, c, ai+n/2, aj, bi, bj+n/2, ci+n/2, cj+n/2, n/2);			

	// 	cilk_sync;

	// 	cilk_spawn matrix_mul(a, b, c, ai, aj+n/2, bi+n/2, bj, ci, cj, n/2);

	// 	cilk_spawn matrix_mul(a, b, c, ai, aj+n/2, bi+n/2, bj+n/2, ci, cj + n/2, n/2);

	// 	cilk_spawn matrix_mul(a, b, c, ai+n/2, aj+n/2, bi+n/2, bj, ci+n/2, cj, n/2);

	// 	cilk_spawn matrix_mul(a, b, c, ai+n/2, aj+n/2, bi+n/2, bj+n/2, ci+n/2, cj+n/2, n/2);	

	// 	cilk_sync;

	// }

}


void shareon(thread_pool* tp){

	// sleep(5);
	tp->shareon();
}


void sleep10(){
	sleep(10);
}



int main(int argc, char const *argv[])
{
	thread_pool tp(100);
	tp.start();
	

	int n = 4;
	int** x = new int*[n];
	int** y = new int*[n];
	int** z = new int*[n];

	for (int i = 0; i < n; ++i)
	{
		x[i] = new int[n];
		y[i] = new int[n];
		z[i] = new int[n];
		for (int j = 0; j < n; ++j)
		{
			x[i][j] = j; 
			y[i][j] = j;
			z[i][j] = 0;
		}
	}

	// shareon(&tp);

	// cilk_sync;

	auto start = high_resolution_clock::now();
	matrix_mul(x, y, z, 0, 0, 0, 0, 0, 0, n, &tp);



	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	cout <<"Matrix multiplication : "<<duration.count() << endl;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout << z[i][j] << " ";
		}
		cout << endl;
	}

	sleep(2);
	shareon(&tp);
	// sleep(10);
	cilk_spawn sleep10();

	cilk_sync;

	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			cout << z[i][j] << " ";
		}
		cout << endl;
	}

	// int xy;
	// cin >> xy;

	return 0;
}
