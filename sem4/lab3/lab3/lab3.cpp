#include <omp.h>
#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

void func() {
    for (int i = 0; i < 500000; i++) rand();
}

void task1() {
    omp_set_num_threads(2);

    cout << "\n=== TASK 1 ===\n";
    cout << "parallel for:\n";

#pragma omp parallel for
    for (int i = 0; i < 100; i++) {
        cout << 0;
        func();
    }
    cout << "\n\nsections + barrier:\n";

#pragma omp parallel
    {
#pragma omp sections nowait
        {
#pragma omp section
            for (int i = 0; i < 10; i++) {
                cout << 1;
                func();
            }

#pragma omp section
            for (int i = 0; i < 20; i++) {
                cout << 2;
                func();
            }
        }

#pragma omp barrier
        for (int i = 0; i < 10; i++) {
            cout << 3;
            func();
        }
    }

    cout << "\n";
}

void task2() {
    omp_set_num_threads(2);

    cout << "\n=== TASK 2 ===\n";
    cout << "parallel for (thread ids):\n";

#pragma omp parallel for
    for (int i = 0; i < 20; i++) {
        cout << omp_get_thread_num();
        func();
    }

    cout << "\n\nsections + barrier:\n";

#pragma omp parallel
    {
#pragma omp single
        {
            cout << "threads: " << omp_get_num_threads() << "\n";
        }

#pragma omp sections nowait
        {
#pragma omp section
            for (int i = 0; i < 10; i++) {
                cout << 1;
                func();
            }

#pragma omp section
            for (int i = 0; i < 20; i++) {
                cout << 2;
                func();
            }
        }

#pragma omp barrier
        for (int i = 0; i < 10; i++) {
            cout << 3;
            func();
        }
    }

    cout << "\n";
}

int global_counter = 0;

void task3() {
    const int N = 20;
    int arr[N] = { 0 };
    global_counter = 0;

    omp_set_num_threads(2);

    cout << "\n=== TASK 3 ===\n";
    cout << "atomic:\n";

#pragma omp parallel for
    for (int i = 0; i < N; i++) {
        arr[i] = i * 10 + omp_get_thread_num();
#pragma omp atomic
        global_counter++;
        cout << arr[i] << " ";
    }

    cout << "\ncounter = " << global_counter << "\n";

    cout << "\nordered:\n";

#pragma omp parallel for ordered schedule(dynamic)
    for (int i = 0; i < 10; i++) {
        int val = i * i;
#pragma omp ordered
        cout << val << " ";
    }

    cout << "\n";
}

void task4() {
    const int N = 10;
    vector<int> shared_array(N, 0);
    int write_index = 0;

    omp_lock_t lock;
    omp_init_lock(&lock);

    omp_set_num_threads(2);

    cout << "\n=== TASK 4 ===\n";

#pragma omp parallel
    {
        int tid = omp_get_thread_num();

        for (int i = 0; i < 5; i++) {
            omp_set_lock(&lock);

            if (write_index < N) {
                shared_array[write_index] = tid * 10 + i;
                write_index++;
            }

            omp_unset_lock(&lock);
        }
    }

    if (omp_test_lock(&lock)) {
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);

    cout << "result array:\n";

    for (int i = 0; i < N; i++) {
        cout << shared_array[i] << " ";
    }

    cout << "\n";
}

int main() {
    int choice = -1;

    while (choice != 0) {
        cout << "\n\n  1 - parallel for\n";
        cout << "  2 - for / sections / barrier\n";
        cout << "  3 - atomic / ordered\n";
        cout << "  4 - Task 4: lock-based sync\n";
        cout << "  0 - Exit\n";
        cout << "Your choice:  \n\n";
        cin >> choice;

        switch (choice) {
        case 1: task1(); break;
        case 2: task2(); break;
        case 3: task3(); break;
        case 4: task4(); break;
        }
    }

    return 0;
}