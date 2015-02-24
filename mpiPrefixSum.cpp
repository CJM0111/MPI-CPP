/**
 * Author: Chris McDonald
 * Program: An implementation of the parallel prefix sum algorithm
 * Using the Work-Depth model for parallel programming (assuming an arbitrary or infinite number of processors),
 * the serial prefix sum algorithm can be improved from running in O(n) time with O(n) work to O(log(n)) time
 * without increasing the total work done by the algorithm.
 * This is done by seperating the work for each level of the array between the processors so
 * the computation for the prefix sum algorithm is parallelized on each level of the array.
 * The time is accumulated by the for-loops iterating through the height of the tree which is O(log(n)),
 * so because two for-loops are iterated through, the total time is O(2*log(n)), which reduces to O(log(n)).
**/

#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <math.h>
#include <time.h>
#include <mpi.h> // Message Passing Interface
using namespace std;

int main(int argc, char * argv[])
{
    // "Parallel" variables
    int my_rank;			// My CPU number for this process
    int p;					// Number of CPUs
    
    // Start MPI
    MPI_Init(&argc, &argv);
    
    // Find out my rank!
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    // Find out the number of processes!
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    
    /**
     * ~~For testing purposes only~~
     * Sets a "seed" value for the random number generator
     * so the random numbers will always be the same
     srand(1);
    **/
    
    // "Serial" variables
    int n = 64; // The size of the original array. This can be 2^x for any value of x
    int logN = log(n * 1.0) / log(2.0); // Computes log(n), which is used as the height of the array
    int A[n]; // Original array
    int B[logN+1][n]; // Second array uses array A to compute the sum
    int C[logN+1][n]; // Final array uses array B to compute the prefix sum
    int tempArray[n]; // Temporary array used to store the local solutions for each processor
    
    // Set all positions in the arrays to 0
    for(int h=0; h<logN+1; h++){
        for(int i=0; i<n; i++){
            B[h][i] = 0;
        }
    }
    for(int h=0; h<logN+1; h++){
        for(int i=0; i<n; i++){
            C[h][i] = 0;
        }
    }
    for(int i = 0; i < n; i++){
        tempArray[i] = 0;
    }
    
    // Generate random numbers from 1-10 to fill the initial array with
    if(my_rank == 0){
        for(int i=0; i<n; i++){
            A[i] = 1 + rand() % 10;
        }
    }
    
    // Broadcast the array A from the root processor 0 to all processors
    MPI_Bcast(&A[0], n, MPI_INT, 0, MPI_COMM_WORLD);
    
    // Display the initial array A
    if(my_rank ==0){
        cout << "A: ";
        for(int i=0; i<n; i++){
            cout << A[i] << " ";
        }
        cout << endl;
    }
    
    // Fill the bottom level of array B with the original array A
    for(int i=0; i<n; i++){
        B[0][i] = A[i];
    }
    
    // Iterate up the height of the array B to compute the sum of array A using pairwise summation
    int arraySizeUp = n/2; // Size of the array for the given height
    // Bottom up - where h is the height
    for(int h=1; h<logN+1; h++){
        // Use striping to seperate the work between processors
        for(int i=my_rank; i < n/pow(2,h); i+=p){
            B[h][i] = B[h-1][2*i] + B[h-1][2*i+1];
            tempArray[i] = B[h][i];
        }
        // Fill the array B with the sum of the values in tempArray from array position 0 to arraySizeUp
        MPI_Allreduce(&(tempArray[0]), &(B[h][0]), arraySizeUp, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        arraySizeUp = arraySizeUp/2;
    }
    
    // Display the second array B with the pairwise sum
    if(my_rank==0){
        cout << "B: " << endl;
        for(int h=0; h<logN+1; h++){
            for(int i=0; i<n; i++){
                cout << B[h][i] << " ";
            }
            cout << endl;
        }
    }
    
    // Iterate down the height of the array C to compute the prefix sum of array A
    int arraySizeDown = 1; // Size of the array for the given height
    // Top down - where h is the height
    for(int h=logN; h>=0; h--){
        // Use striping to seperate the work between processors
        for(int i=my_rank; i < n/pow(2,h); i+= p){
            if(i==0){
                C[h][0] = B[h][0];
                tempArray[i] = C[h][0];
            }
            else if(i%2 == 0){
                C[h][i] = B[h][i] + C[h+1][(i-1)/2];
                tempArray[i] = C[h][i];
            }
            else{
                C[h][i] = C[h+1][i/2];
                tempArray[i] = C[h][i];
            }
        }
        // Fill the array C with the sum of the values in tempArray from array position 0 to arraySizeDown
        MPI_Allreduce(&(tempArray[0]), &(C[h][0]), arraySizeDown, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
        arraySizeDown = arraySizeDown*2;
    }
    
    // Display the final array C with the prefix sum
    if(my_rank == 0){
        cout << "C: " << endl;
        for(int h=logN; h>=0; h--){
            for(int i=0; i<n; i++){
                cout << C[h][i] << " ";
            }
            cout << endl;
        }
    }

    // Shut down MPI
    MPI_Finalize();
    
    return 0;
}
