#include <omp.h>
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

void func() {
    for (int i = 0; i < 500000; i++) rand();
}

void task1() {
    cout << "\n= TASK 1 (test program) =\n";

    omp_set_num_threads(2);

    cout << "\n--- parallel for ---\n";
#pragma omp parallel for
    for (int i = 0; i < 100; i++) {
        cout << 0; func();
    }
    cout << endl;

    cout << "\n--- sections + barrier ---\n";
#pragma omp parallel
    {
#pragma omp sections nowait
        {
#pragma omp section
            for (int i = 0; i < 10; i++) { cout << 1; func(); }
#pragma omp section
            for (int i = 0; i < 20; i++) { cout << 2; func(); }
        }
#pragma omp barrier
        for (int i = 0; i < 10; i++) { cout << 3; func(); }
    }
    cout << endl;
}

void task2() {
    cout << "\n= TASK 2 =\n";

    omp_set_num_threads(2);

    cout << "\n--- Directive FOR (thread ids) ---\n";
#pragma omp parallel for
    for (int i = 0; i < 20; i++) {
        cout << omp_get_thread_num();
        func();
    }
    cout << endl;

    cout << "\n--- Directive SECTIONS + BARRIER ---\n";
#pragma omp parallel
    {
#pragma omp single
        {
            cout << "Number of threads: " << omp_get_num_threads() << endl;
        }

#pragma omp sections nowait
        {
#pragma omp section
            for (int i = 0; i < 10; i++) { cout << 1; func(); }
#pragma omp section
            for (int i = 0; i < 20; i++) { cout << 2; func(); }
        }

#pragma omp barrier
        for (int i = 0; i < 10; i++) { cout << 3; func(); }
    }
    cout << endl;
}

int global_counter = 0;

void task3() {
    cout << "\n= TASK 3 =\n";

    const int N = 20;
    int arr[N] = {0};
    global_counter = 0;

    omp_set_num_threads(2);

    cout << "\n--- Directive ATOMIC ---\n";
#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        arr[i] = i * 10 + omp_get_thread_num();
#pragma omp atomic
        global_counter++;
        cout << "[Thread " << omp_get_thread_num()
             << "] arr[" << i << "]=" << arr[i]
             << "  counter=" << global_counter << "\n";
    }
    cout << "Final counter (must be " << N << "): " << global_counter << "\n";
    cout << "\n--- Directive ORDERED ---\n";
#pragma omp parallel for ordered schedule(dynamic)
    for (int i = 0; i < 10; i++) {
        int val = i * i;   // parallel computation
#pragma omp ordered
        {
            cout << "[Thread " << omp_get_thread_num()
                 << "] i=" << i << "  val=" << val << "\n";
        }
    }
}

void task4() {
    cout << "\n= TASK 4 =\n";

    const int N = 10;
    vector<int> shared_array(N, 0);
    int write_index = 0;

    omp_lock_t lock;
    omp_init_lock(&lock);

    omp_set_num_threads(2);

    cout << "\n--- OMP_SET_LOCK / OMP_UNSET_LOCK ---\n";
#pragma omp parallel
    {
        int tid = omp_get_thread_num();
        for (int i = 0; i < 5; i++) {

            omp_set_lock(&lock);

            if (write_index < N) {
                shared_array[write_index] = tid * 10 + i;
                cout << "[Thread " << tid << "] write arr["
                     << write_index << "] = " << shared_array[write_index] << "\n";
                write_index++;
            }

            omp_unset_lock(&lock);
        }
    }

    cout << "\n--- OMP_TEST_LOCK (non-blocking) ---\n";
    if (omp_test_lock(&lock)) {
        cout << "Lock acquired successfully (test_lock returned true)\n";
        omp_unset_lock(&lock);
    } else {
        cout << "Lock is busy, skipping...\n";
    }

    omp_destroy_lock(&lock);

    cout << "\nResulting array: ";
    for (int i = 0; i < N; i++) cout << shared_array[i] << " ";
    cout << "\n";
}

int main() {
    int choice = -1;

    while (choice != 0) {
        cout << "  1\n";
        cout << "  2 - for / sections / barrier\n";
        cout << "  3 - atomic / ordered\n";
        cout << "  4 - Task 4: lock-based sync\n";
        cout << "  0 - Exit\n";
        cout << "Your choice:  ";
        cin >> choice;

        switch (choice) {
            case 1: task1(); break;
            case 2: task2(); break;
            case 3: task3(); break;
            case 4: task4(); break;
            case 0: cout << "end\n"; break;
            default: cout << "wrong\n"; break;
        }
    }

    return 0;
}