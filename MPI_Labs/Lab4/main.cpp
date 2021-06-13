#include "mpi.h"
#include <iostream>
#include <cmath>
#include <locale.h>
#include <time.h>

#define N 2

using namespace std;

struct MPI_State {
    int proc_num;
    int proc_rank;
    MPI_Status status;
};

MPI_State initial_state() {
    MPI_State state;
    MPI_Comm_size(MPI_COMM_WORLD, &state.proc_num);
    MPI_Comm_rank(MPI_COMM_WORLD, &state.proc_rank);

    return state;
}

int *create_buff(int length)
{
    int *buff = new int[length];
    for (int i = length - 1; i >= 0; i--)
    {
        buff[i] = 0;
    }
    return buff;
}
int *create_num(int proc_rank, int size)
{
    int *num = new int[size];
    for (int i = size - 1; i >= 0; i--)
    {
        if (i < N ) num[i] = proc_rank + (rand() % 4 + 1);
        else num[i] = 0;
    }
    return num;
}
void print_num(int *num, int length)
{
    for (int i = length - 1; i >= 0; i--)
    {
        cout << num[i];
    }
    cout << '\n';
}
void mult(int* num_1, int* num_2, int* res, int size_num_1, int size_num_2)
{
    cout << "num1 = "; print_num(num_1, size_num_1);
    cout << "num2 = "; print_num(num_2, size_num_2);
    for (int ix = 0; ix < size_num_1; ix++)
    {
        for (int jx = 0; jx < size_num_2; jx++)
        {
            res[ix + jx] += num_1[ix] * num_2[jx];
        }
    }

    for (int ix = 0; ix < size_num_1 + size_num_2; ix++)
    {
        res[ix + 1] += res[ix] / 10;
        res[ix] %= 10;
    }
}

void copyNum(int* num, int* buf, int size) {
    for (int i = 0; i < size - 1; i++) {
        buf[i] = num[i];
    }
}

int* copyNum(int* num, int size) {
    int* res = create_buff(size);
    for (int i = 0; i < size - 1; i++) {
        res[i] = num[i];
    }
    return res;
}

int* multSomeNums(double numCount, int multNumCount, int proc_rank) {
    int* ans = create_buff(N * numCount);
    int* num_1 = create_num(2, N * numCount);
    for (int i = 0; i < multNumCount - 1; i++) {
        mult(num_1, create_num(i, N), ans, N * numCount, N);
        copyNum(ans, num_1, N * numCount);
        if (i < multNumCount - 2)
            copyNum(create_buff(N * numCount), ans, N * numCount);
    }
    cout << "ans of proc " << proc_rank << " = "; print_num(ans, N * numCount);
    return ans;
}

bool isNumZero(int* num, int size) {
    bool ans = true;
    for (int i = 0; i < size - 1; i++) {
        if (num[i] != 0)
            ans = false;
    }
    return ans;
}

int main(int argc, char* argv[])
{
    void star(int argc, char* argv[], double numCount);

    double numCount = 9;

    if (numCount <= 4) {
        int* ans = multSomeNums(numCount, numCount, 0);
        cout << "ans = "; print_num(ans, N * numCount);
    } else
        star(argc, argv, numCount);

    return 0;
}
void star(int argc, char* argv[], double numCount) {
    MPI_Init(&argc, &argv);
    auto state = initial_state();
    srand(time(NULL));
    int* procValue = create_buff(N * numCount);
    int* firstProcValue = create_buff(N * numCount);
    int* secondProcValue = create_buff(N * numCount);
    int* thirdProcValue = create_buff(N * numCount);

    int medianNum = ceil(numCount / 4);

    if (state.proc_rank == 0) {
        procValue = multSomeNums(numCount, medianNum, state.proc_rank);
    } else {
        if (!(numCount - medianNum * state.proc_rank == 1 ||
              numCount - medianNum * state.proc_rank <= 0)) {
            if (numCount - medianNum * (state.proc_rank + 1) == 1) {
                procValue = multSomeNums(numCount, numCount - medianNum * state.proc_rank, state.proc_rank);
            } else {
                if (state.proc_rank == 3) {
                    procValue = multSomeNums(numCount, numCount - medianNum * state.proc_rank, state.proc_rank);
                } else {
                    procValue = multSomeNums(numCount, medianNum, state.proc_rank);
                }
            }
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Datatype type_1; // создание нового типа
    MPI_Type_contiguous(N * numCount, MPI_INT, &type_1);
    MPI_Type_commit(&type_1);

    if (state.proc_rank != 0)
        MPI_Send(procValue, 1, type_1, 0, 1, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (state.proc_rank == 0) {
        MPI_Recv(firstProcValue, 1, type_1, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &state.status);
        MPI_Recv(secondProcValue, 1, type_1, 2, MPI_ANY_TAG, MPI_COMM_WORLD, &state.status);
        MPI_Recv(thirdProcValue, 1, type_1, 3, MPI_ANY_TAG, MPI_COMM_WORLD, &state.status);

        int* mainAns = create_buff(N * numCount);
        mult(procValue, firstProcValue, mainAns, N * numCount, N * numCount);
        copyNum(mainAns, procValue, N * numCount);
        if (!isNumZero(secondProcValue, N * numCount)) {
            copyNum(create_buff(N * numCount), mainAns, N * numCount);
            mult(procValue, secondProcValue, mainAns, N * numCount, N * numCount);
            copyNum(mainAns, procValue, N * numCount);
            if (!isNumZero(thirdProcValue, N * numCount)) {
                copyNum(create_buff(N * numCount), mainAns, N * numCount);
                mult(procValue, thirdProcValue, mainAns, N * numCount, N * numCount);
                copyNum(mainAns, procValue, N * numCount);
            }
        }

        cout << "mainAns is "; print_num(mainAns, N * numCount);
    }

    delete[] procValue;
    delete[] firstProcValue;
    delete[] secondProcValue;
    delete[] thirdProcValue;
    MPI_Type_free(&type_1);

    MPI_Finalize();
}