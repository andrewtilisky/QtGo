#include "rules.h"

__device__ bool hasLiberty(int x, int y, bool black) const {
	if (isEmpty(x, y)) {
		return true;
	} else if (isBlack(x, y) != black)
	  return false;

	if (x != 0) // Checks if not on left edge
		if (hasLiberty(x - 1, y, black))
			return true;

	if (x != dimensions() - 1) // Checks if not on right edge
		if (hasLiberty(x + 1, y, black))
			return true;

	if (y != 0)  // Checks if not on top edge
		if (hasLiberty(x, y - 1, black))
			return true;

	if (y != dimensions() - 1) // Checks if not on bottom edge
		if (hasLiberty(x, y + 1, black))
			return true;

	return false;
}

__device__ int pointWillBeCaptured(unsigned char x, unsigned char y,
		bool isBlack) const {
	if (isEmpty(x, y))
		throw Point::pointoccupiedexception();
	else {
		return not hasLiberty(x, y, isBlack);
	}
}


//__global__ void captureStones(Point** d_points, unsigned char dimension)
__global__ void captureStones(Point** d_points)
{
			if (not isEmpty(x, y) and isBlack(x, y) == black
					and pointWillBeCaptured(x, y, black))
				capture(threadIdx.x, threadIdx.y);
}

bool willBeTakenCuda(unsigned char DIMENSION, Point** h_points,
		const unsigned char x, const unsigned char y, const bool black)
{
	const size_t size = N(DIMENSION) * sizeof(Point*);
	Point **d_points;// = new Point[N(DIMENSION)];

	cudaError_t error;

	error = cudaMalloc((void**)&d_points, size);

	if (error != cudaSuccess) {
		printf("cudaMalloc d_points returned error code %d, line(%d)\n", error,
				__LINE__);
		exit (EXIT_FAILURE);
	}

	//	//debug to check if host board's empty
	//	for (short row = 0; row < DIMENSION; ++row)
	//		for (short col = 0; col < DIMENSION; ++col)
	//			printf("%d mIs_empty %d\n", row * DIMENSION + col,
	//					h_points[row * DIMENSION + col]->mIs_empty);

	//host to device
	error = cudaMemcpy(d_points, *h_points, size, cudaMemcpyHostToDevice);
	//	error = cudaMemcpy(d_points, h_points, sizeof(Point*), cudaMemcpyHostToDevice);

	if (error != cudaSuccess) {
		printf(
				"cudaMemcpy (d_points, &h_points) returned error code %d, line(%d)\n",
				error, __LINE__);
		exit (EXIT_FAILURE);
	}

	captureStones<<<1, N(DIMENSION)>>>(d_points);

	//	for (short row = 0; row < DIMENSION; ++row)	{
	//		for (short col = 0; col < DIMENSION; ++col)
	//		{
	//			h_points[row * DIMENSION + col]->mIs_black = true;
	//			h_points[row * DIMENSION + col]->mIs_empty = false;
	//		}
	//	}

	//device to host
	error = cudaMemcpy(*h_points, d_points, size, cudaMemcpyDeviceToHost);
	//	error = cudaMemcpy(&h_points[0], d_points, size, cudaMemcpyDeviceToHost);

	if (error != cudaSuccess) {
		printf("cudaMemcpy (h_points, d_points) returned error code %d, line(%d)\n",
				error, __LINE__);
		exit (EXIT_FAILURE);
	}

	//	error = cudaFree(d_points);
	//
	//	if (error != cudaSuccess) {
	//		printf("cudaFree(d_points) returned error code %d, line(%d)\n",
	//				error, __LINE__);
	//		exit (EXIT_FAILURE);
	//	}

	return false;
}
