#include <iostream>
#include <windows.h>
#include <time.h>

#define N 100000000
#define blockSize 305120
#define threads 4

using std::cout;
using std::endl;

int pos = 0;
double pi = 0.0;
LPCRITICAL_SECTION lpCriticalSection = new CRITICAL_SECTION;

DWORD WINAPI MyThreadFunction(LPVOID lpParam);

int main() {
    DWORD start, end;
    HANDLE threadArray[threads ];
    int position[threads ];
    double executionSpeed = 0.0;
    pi = 0.0;
    InitializeCriticalSection(lpCriticalSection);

    for (int i = 0; i < threads ; ++i) {
        position[i] = blockSize * i;
        pos = position[i];
        threadArray[i] = CreateThread(
            NULL,               // дескриптор защиты
            0,                  // use default stack size  
            MyThreadFunction,   // thread function name
            &position[i],       // аргументы для поточной функции
            CREATE_SUSPENDED,   // создается остановленным
            NULL);              // returns the thread identifier
    }
    start = clock();

    for (int i = 0; i < threads ; ++i)
        ResumeThread(threadArray[i]);
    WaitForMultipleObjects(threads , threadArray, TRUE, INFINITE);

    pi /= (long double)N;
    DeleteCriticalSection(lpCriticalSection);
    end = clock();
    executionSpeed += end - start;
    for (int i = 0; i < threads ; ++i)
        CloseHandle(threadArray[i]);

    printf("pi: %.10lf\n", pi);
    cout << "Execution speed: " << (long double)(executionSpeed) / CLOCKS_PER_SEC << endl;

}

DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
    int* first = (int*)lpParam;
    int end = *first + blockSize;
    long double x, tepm;

    while (*first < N) {
        tepm = 0.0;
        for (int i = *first; (i < end) && (i < N); ++i) {
            x = (i + 0.5) / N;
            tepm += (4 / (1 + x * x));
        }
        EnterCriticalSection(lpCriticalSection);
        pi += tepm;
        pos += blockSize;
        *first = pos;
        LeaveCriticalSection(lpCriticalSection);
        end = *first + blockSize;
    }
    return 0;
}