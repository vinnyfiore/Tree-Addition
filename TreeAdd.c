#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#include <stdbool.h>

int main(){
    int sum, comm_sz, my_rank, i, next, value, modNum, runNum, temp;
    bool sent;

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    srand(time(NULL)+my_rank);
    value = rand()%11;
    printf("%d, ", value);

    if(comm_sz==128){
        runNum=7;
    }
    else if(comm_sz==64){
        runNum=6;
    }
    else if(comm_sz==32){
        runNum=5;
    }
    else if(comm_sz==16){
        runNum=4;
    }
    else if(comm_sz==8){
        runNum=3;
    }
    else if(comm_sz==4){
        runNum=2;
    }
    else if(comm_sz==2){
        runNum=1;
    }

    modNum=2;
    next=1;
    sent=false;
    sum=0;
    temp=0;
    sum=value;

    for(i=0; i<runNum; i++){
        if (my_rank % modNum==0 ){
            MPI_Recv(&temp, 1, MPI_INT, my_rank+next, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            sum+=temp;
        }
        else{
            if(sent==false){
                MPI_Send(&sum, 1, MPI_INT, my_rank-next, 1, MPI_COMM_WORLD);
                sent=true;
            }
        }
        modNum = modNum*2;
        next = next*2;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    if(my_rank==0) {
        printf("The total is: %d \n", sum);
    }

    MPI_Finalize();
    return 0;
}
