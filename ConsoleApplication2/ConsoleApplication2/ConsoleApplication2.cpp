#include <iostream>
#include <windows.h>
#include <time.h>
#include <omp.h>

#define N 100000000
#define blockSize 305120
#define threads 16

using std::cout;
using std::endl;

int main() {
    DWORD start;
    DWORD end;

    long double pi = 0, executionSpeed = 0;
    
    start = clock();

#pragma omp parallel shared(t1) reduction (+:pi) num_threads(threads)
        {
#pragma omp for schedule(dynamic, blockSize) nowait
            for (int i = 0; i < N; ++i) {
                long double x = (i + 0.5) / N;
                pi += 4 / (1 + x * x);
            }
        }

        pi /= (long double)N;
        end = clock();
        executionSpeed = end - start;
   

    cout << "pi: " << pi << endl;
    cout << "Execution speed: " << (long double)(executionSpeed) / CLOCKS_PER_SEC << endl;
}