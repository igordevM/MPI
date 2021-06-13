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

int main(int argc, char* argv[]) {
    const int message = 12;
    int message_sec = 24;
    const int sz = 1;
    unsigned short M = 1;

    MPI_Init(&argc, &argv);
    auto state = initial_state();
    int root_message;
    if (state.proc_rank != 0)
        cout << "#" << state.proc_rank << " is sending " << message << " to the parent!" << "\n";
    MPI_Reduce(&message, &root_message, sz, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    while(M-- > 0) {
        if (state.proc_rank == 0)
            cout << "The message was recieved " << "and it's: " << root_message << "\n";
        MPI_Bcast(&message_sec, sz, MPI_INT, 0, MPI_COMM_WORLD);
        if (state.proc_rank == 0)
            cout << "Send everyone " << message_sec << endl;
        if (state.proc_rank != 0) {
            cout << "Got " << message_sec << " message from parent by process #" << state.proc_rank << "\n";
            cout << "#" << state.proc_rank << " is sending " << message_sec << " to the parent again!" << "\n";
        }
        MPI_Reduce(&message_sec, &root_message, sz, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
        if (state.proc_rank == 0)
            cout << "The 2 message was recieved " << "and it's: " << root_message << "\n";
    }

    MPI_Finalize();

    return 0;
}
