#include <cstdlib>
#include <iostream>
#include <omp.h>
#include <ctime>
using namespace std;

void test1(int row, int col){
    int** a;
    a = new int*[row];
    for (int i=0; i<row; i++){
        a[i] = new int[col];
        for (int j=0; j<col; j++){
            a[i][j] = rand() % 10;
        }
    }
    int** b;
    b = new int*[row];
    for (int i=0; i<row; i++){
        b[i] = new int[col];
        for (int j=0; j<col; j++){
            b[i][j] = rand() % 10;
        }
    }
    int** result;
    result = new int*[row];
    for (int i=0; i<row; i++){
        result[i] = new int[col];
    }
    for (int i =0; i<row; i++){
        for (int j=0; j<col; j++){
            for (int k=0; k<col; k++){
               result[i][j] += a[i][k] *b[k][j]; 
            }
        }
    }

}

void test2(int row, int col, int p){
    int** a;
    a = new int*[row];
    for (int i=0; i<row; i++){
        a[i] = new int[row];
        for (int j=0; j<col; j++){
            a[i][j] = rand() % 10;
        }
    }
    int** b;
    b = new int*[row];
    for (int i=0; i<row; i++){
        b[i] = new int[col];
        for (int j=0; j<col; j++){
            b[i][j] = rand() % 10;
        }
    }
    int** result;
    result = new int*[row];
    for (int i=0; i<row; i++){
        result[i] = new int[col];
    }

    omp_set_num_threads(p);
    #pragma omp parallel
    {
        int i, j, k;
        #pragma omp for
        for (i =0; i<row; i++){
            for (j=0; j<col; j++){
                int dot = 0;
                for (k=0; k<col; k++){
                    dot += a[i][k] *b[k][j]; 
                }
                result[i][j] = dot;
            }
        }
    }

}

void test3(int row, int col, int p){
    int** a;
    a = new int*[row];
    for (int i=0; i<row; i++){
        a[i] = new int[col];
        for (int j=0; j<col; j++){
            a[i][j] = rand() % 10;
        }
    }
    int** b;
    b = new int*[row];
    for (int i=0; i<row; i++){
        b[i] = new int[col];
        for (int j=0; j<col; j++){
            b[i][j] = rand() % 10;
        }
    }
    int** result;
    result = new int*[row];
    for (int i=0; i<row; i++){
        result[i] = new int[col];
    }

    omp_set_num_threads(p);
    #pragma omp parallel
    {
        int i, j, k;
        #pragma omp for schedule(static)
        for (i =0; i<row; i++){
            for (j=0; j<col; j++){
                int dot = 0;
                for (k=0; k<col; k++){
                    dot += a[i][k] *b[k][j]; 
                }
                result[i][j] = dot;
            }
        }
    }
}

void test4(int row, int col, int p){
    int** a;
    a = new int*[row];
    for (int i=0; i<row; i++){
        a[i] = new int[col];
        for (int j=0; j<col; j++){
            a[i][j] = rand() % 10;
        }
    }
    int** b;
    b = new int*[row];
    for (int i=0; i<row; i++){
        b[i] = new int[col];
        for (int j=0; j<col; j++){
            b[i][j] = rand() % 10;
        }
    }
    int** result;
    result = new int*[row];
    for (int i=0; i<row; i++){
        result[i] = new int[col];
    }

    omp_set_num_threads(p);
    int i,j,k;
    #pragma omp parallel
    {
        int i, j, k, jj, kk;
        for (int jj=0; jj<row; jj+=p){
            for (int kk=0; kk<col; kk+=p){
                #pragma omp for schedule(auto)
                for (int i=0; i<row; i++){
                    for(int j=jj; j< ((jj+p) > row ? row:(jj+p)); j++){
                        int temp =0; 
                        for (int k=kk; k<((kk+p)>col?col:(kk+p)); k++){
                            temp += a[i][k]*b[k][j];
                        }
                        result[i][j] += temp;
                    }
                }
            }
        }
    }
}

int main(int argc, char* argv[]){
    int row = 1000;
    int col = 1000;
    int p = 4;
    clock_t start = clock();
    test1(row, col);
    clock_t end = clock();
    cout<< "Total time for Serial: " << end - start <<endl;

    start = clock();
    test2(row, col, p);
    end = clock();
    cout <<"Total time for MM-Par: " << end - start <<endl;

    start = clock();
    test3(row, col, p);
    end = clock();
    cout<<"Total time for MM-1D: " <<end - start <<endl;

    start = clock();
    test4(row, col, p);
    end=clock();
    cout <<"Total time for MM-2D: " <<end - start <<endl;
    return 0;
}

