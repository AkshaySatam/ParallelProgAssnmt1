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
using namespace std;
using namespace std::chrono;

// void matrix_mul(int **a, int **b, int **c, int n);
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

// 	matrix_mul(a11, b11, temp1, (n/2));
// 	matrix_mul(a12, b21, temp2, (n/2));
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

// 	matrix_mul(a11, b12, temp1, (n/2));
// 	matrix_mul(a12, b22, temp2, (n/2));
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

// 	matrix_mul(a21, b11, temp1, (n/2));
// 	matrix_mul(a22, b21, temp2, (n/2));
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

// 	matrix_mul(a21, b12, temp1, (n/2));
// 	matrix_mul(a22, b22, temp2, (n/2));
// 	matrix_sum(temp1, temp2, c22, (n/2));

// 	for (int i = 0; i < (n/2); i++){
// 		delete[] temp1[i];
// 		delete[] temp2[i];	
// 	}
// }

// #define _PARALLEL_
// void matrix_mul(int **a, int **b, int **c, int n){
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
// 		cilk_spawn eval11(a11, b11, a12, b21, c11, n);



// 		cilk_spawn eval12(a11, b12, a12, b22, c12, n);



// 		cilk_spawn eval21(a21, b11, a22, b21, c21, n);



// 		cilk_spawn eval22(a21, b12, a22, b22, c22, n);
	
// 		cilk_sync;
// 		#else

// 		eval11(a11, b11, a12, b21, c11, n);

// 		eval12(a11, b12, a12, b22, c12, n);

// 		eval21(a21, b11, a22, b21, c21, n);

// 		eval22(a21, b12, a22, b22, c22, n);		

// 		#endif

// 		for (int i = 0; i < n / 2; i++)
// 			for (int j = 0; j < n / 2; j++){
// 				c[i][j] = c11[i][j];
// 				c[i][j + (n / 2)] = c12[i][j];
// 				c[i + (n / 2)][j] = c21[i][j];
// 				c[i + (n / 2)][j + (n / 2)] = c22[i][j];
// 			}
	

			
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

void matrix_mul(int **a, int **b, int **c, int ai, int aj, int bi, int bj, int ci, int cj,int n){

	if (n == 1)
	{
		c[ci][cj] += a[ai][aj] * b[bi][bj];
	}
	else
	{

		cilk_spawn matrix_mul(a, b, c, ai, aj, bi, bj, ci, cj, n/2);

		cilk_spawn matrix_mul(a, b, c, ai, aj, bi, bj+n/2, ci, cj + n/2, n/2);

		cilk_spawn matrix_mul(a, b, c, ai+n/2, aj, bi, bj, ci+n/2, cj, n/2);

		cilk_spawn matrix_mul(a, b, c, ai+n/2, aj, bi, bj+n/2, ci+n/2, cj+n/2, n/2);			

		cilk_sync;

		cilk_spawn matrix_mul(a, b, c, ai, aj+n/2, bi+n/2, bj, ci, cj, n/2);

		cilk_spawn matrix_mul(a, b, c, ai, aj+n/2, bi+n/2, bj+n/2, ci, cj + n/2, n/2);

		cilk_spawn matrix_mul(a, b, c, ai+n/2, aj+n/2, bi+n/2, bj, ci+n/2, cj, n/2);

		cilk_spawn matrix_mul(a, b, c, ai+n/2, aj+n/2, bi+n/2, bj+n/2, ci+n/2, cj+n/2, n/2);	

		cilk_sync;

	}

}

int main2(int argc, char const *argv[])
{
	int n = 16;
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

	auto start = high_resolution_clock::now();
	matrix_mul(x, y, z, 0, 0, 0, 0, 0, 0, n);
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
	return 0;
}