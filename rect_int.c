/*
Author: Josh Morris
Professor: Dr. Pettey
Class: CSCI 4330
Lab 5

A program that performs rectangular integration of the equation
sqrt(3sin^3(x/2)) from 0 to PI. There are as many subintervals
as there are processes.
*/

#include <stdio.h>
#include <math.h>
#include <mpi.h>

float myFunction(float x) {
	return sqrt(3 * pow(sin(x / 2.0), 3));
}

int main(int argc, char * argv[]) {
	int nproc, myrank;
	float sum, myArea, width, mid, height;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nproc); // get how many processes
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank); // get process num

	width = M_PI / nproc;
	mid = width * (float)myrank + width / 2.0;
	height = myFunction(mid);
	myArea = height * width;

	if(myrank == 0) {
		sum = myArea;
		MPI_Send(&sum, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);

		MPI_Recv(&sum,1,MPI_INT,nproc-1,1,MPI_COMM_WORLD,&status);
		printf("The area under the curve is %f \n", sum);
	} else {
		MPI_Recv(&sum, 1, MPI_INT, myrank - 1, 1, MPI_COMM_WORLD, &status);
		
		sum += myArea;
		
		if (myrank == nproc-1) {
			MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
		} else {
			MPI_Send(&sum, 1 , MPI_INT, myrank + 1, 1, MPI_COMM_WORLD);
		}
	}

	MPI_Finalize();

	return 0;
}