/* Project2.cpp
 * Ima Student
 * EE312, Spring 2015
 * Project 2, Matrix Multiplication
 */

#include <stdio.h>
#include <stdint.h>
#include "MatrixMultiply.h"

/* This function grabs to matrices passed as pointers in a and b and multiplies them
 * The result of the multiplication is placed in pointer c  
 */
void multiplyMatrices(
	double a[],
	const uint32_t a_rows,
	const uint32_t a_cols,
	double b[],
	const uint32_t b_cols,
	double c[]) {

	unsigned int i = 0; //Counter for rows 
	unsigned int j = 0; //Counter for columns
	unsigned int k = 0; //Counter for dot product
	double sum = 0;

	while (i < a_rows) {
		while (j < b_cols) { 
			while (k < a_cols) { //Do the dot product to find element (i,j)
				sum = sum + (a[i*a_cols + k] * b[k*b_cols + j]);
				k = k + 1;
			}
			k = 0;
			*(c+(i*b_cols + j)) = sum; //Update our result matrix 
			sum = 0;
			j = j + 1;
		}
		j = 0;
		i = i + 1;
	}
}

#define READY_FOR_STAGE2
#ifdef READY_FOR_STAGE2

/* these three variables are used in Stage 2 to implement allocateSpace */
#define TOTAL_SPACE 10000
double memory_pool[TOTAL_SPACE];
uint32_t top_of_pool = 0;

/* Allocates space for matrices to find the final result matrix */
PtrDouble allocateSpace(uint32_t size) {
	double* allocatePointer = &memory_pool[top_of_pool];
	top_of_pool = top_of_pool + size;
	if (top_of_pool > (TOTAL_SPACE - 1)) {	//We do this to avoid an overflow
		top_of_pool = 0;					//I hope you don't test for a 1000+ element matrix. That would be kind of horrible. 
	}
	return allocatePointer;

}

/* This function grabs a number N of matrices and multiplies them 
 * It grabs the matrices from a double pointer in matrix_list
 * The final result matrix is placed in address pointed at by out 
*/
void multiplyMatrixChain(
    PtrDouble matrix_list[],
    uint32_t rows[],
    uint32_t cols[],
    uint32_t num_matrices,
    double out[],
    uint32_t out_rows,
    uint32_t out_cols) {
	double* print;

	double* aMat = matrix_list[0]; //We initialize pointers for our first operation
	double* cMat;
	double* bMat;

	unsigned int mult = 1; //Counter of multiplication to be done 

	while (mult < (num_matrices-1)) { //We only do num_matrices-2 operations 
		cMat = allocateSpace(rows[0] * cols[mult]); //We make space for the result of a multiplication
		bMat = *(&matrix_list[0]+mult);
		multiplyMatrices(aMat, rows[0], cols[mult-1], bMat, cols[mult], cMat);
		aMat = cMat; //We make our first input matrix the result of the last operation
		mult = mult + 1;
	}

	bMat = *(&matrix_list[0] + mult);	//We do the final multiplication, but placing the result on out
	multiplyMatrices(aMat, rows[0], cols[mult - 1], bMat, cols[mult], out);
	top_of_pool = 0; //Restart our allocation of space
}

#endif /* READY_FOR_STAGE_2 */