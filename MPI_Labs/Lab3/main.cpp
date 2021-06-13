#include <iostream>
#include "mpi.h"


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

//Send to parent process
void star(const int* msg, const int& size, MPI_State& state) {
    cout << "#" << state.proc_rank << " is sending " << *msg << " to the parent!" << "\n";
    MPI_Send(msg, size, MPI_INT, 0, 0, MPI_COMM_WORLD);
}

int main(int argc, char* argv[]) {
    int message = 12;
    int send_buf[4] = {0, 12, 12, 12};
    int received_buf;
    int gathered_buf[3];
    const int sz = 1;
    unsigned short M = 1;

    MPI_Init(&argc, &argv);
    auto state = initial_state();
    while(M-- > 0) {
        MPI_Scatter(send_buf, sz, MPI_INT, &received_buf, sz, MPI_INT, 0, MPI_COMM_WORLD);
        if (state.proc_rank == 0)
            cout << "Send everyone " << message << endl;

        if (state.proc_rank != 0) {
            cout << "Got " << message << " from parent by process #" << state.proc_rank << "\n";
            received_buf += state.proc_rank;
        }

        MPI_Gather(&received_buf, sz, MPI_INT, gathered_buf, sz, MPI_INT, 0, MPI_COMM_WORLD);

        if (state.proc_rank == 0) {
            for (int i = 1; i < state.proc_num; i++)
                cout << "Got " << gathered_buf[i] << " from process #"<< i << " by parent" << "\n";
        }

    }

    MPI_Finalize();

    return 0;
}