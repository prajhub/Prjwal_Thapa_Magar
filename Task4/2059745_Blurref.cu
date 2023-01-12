//Prajwal_Thapa_Magar_2059745



/*

This code applies a box blur to an image using a CUDA kernel function on the GPU. I
it takes in the memory locations of the input and output images, as well as their dimensions. 

*/


// Compile with:  nvcc 2059745_Blurref.cu lodepng.cpp -o cudablur
// Execute with: ./cudablur



#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include "lodepng.h"


__global__ void box_blur(unsigned char * device_image_output, unsigned char * device_image_input, unsigned int width, unsigned int height)
{
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;
	int x, y;
	int count = 0;

	int idx = blockDim.x * blockIdx.x + threadIdx.x;
	int pixel = idx * 4;

	for (x = (pixel - 4); x <= (pixel + 4); x += 4) 
	{
		if ((x > 0) && x < (height * width * 4) && ((x - 4) / (4 * width) == pixel / (4 * width))) 
		{
			for (y = (x - (4 * width)); y <=  (x + (4 * width)); y += (4 * width)) 
			{
				if (y > 0 && y < (height * width * 4)) 
				{
					r += device_image_input[y];
					g += device_image_input[1 + y];
					b += device_image_input[2 + y]; 
					count++;
				}
			}
		}
	}
	
	a = device_image_input[3 + pixel];

	device_image_output[pixel] = r / count;
	device_image_output[1 + pixel] = g / count;
	device_image_output[2 + pixel] = b / count;
	device_image_output[3 + pixel] = a;
}

int main(int argc, char **argv)
{
	unsigned int error;
	unsigned char *image;
	unsigned int width;
	unsigned int height;
	const char *input_filename = "2059745_image.png";
	const char *output_filename = "2059745_blur.png";

	error = lodepng_decode32_file(&image, &width, &height, input_filename);
	if (error) {
		printf("Error %u: %s\n", error, lodepng_error_text(error));
	}

	int array_size = width * height * 4;
	int array_bytes = array_size * sizeof(unsigned char);

	unsigned char host_image_input[array_size * 4];
	unsigned char host_image_output[array_size * 4];

	for (int i = 0; i < array_size; i++) {
		host_image_input[i] = image[i];
	}

	// Declaring the device's memory pointers.
	unsigned char * d_in;
	unsigned char * d_out;

	// Allocating device's  memory.
	cudaMalloc((void**) &d_in, array_bytes);
	cudaMalloc((void**) &d_out, array_bytes);

	// Transferring the host image input data to device memory..
	cudaMemcpy(d_in, host_image_input, array_bytes, cudaMemcpyHostToDevice);

	// Invoking the kernel function.
	box_blur<<<height, width>>>(d_out, d_in, width, height);

	// Transferring the processed data from the device memory back to the host memory.
	cudaMemcpy(host_image_output, d_out, array_bytes, cudaMemcpyDeviceToHost);
	
	error = lodepng_encode32_file(output_filename, host_image_output, width, height);
	if (error) {
		printf("error %u: %s\n", error, lodepng_error_text(error));
	}

	printf("\n Blurred image is saved on the same directory with name 2059745_blur\n\n");

	// deallocating the device memory
	cudaFree(d_in);
	cudaFree(d_out);

	return 0;
}