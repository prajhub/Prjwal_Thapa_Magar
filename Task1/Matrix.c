//Prajwal_Thapa_Magar_L6CG5

//This code is a program that multiplies two matrices together using multiple threads. It includes the use of pthread library to create and manage threads.


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int *find_number_of_rows_and_columns(const char *file_name); // This function takes in file name and returns pointer to an array of integers.
void *multiply_matrices(void *args); //This function takes in void pointer and returns void pointer, likely used for matrix multiplication.



// Defining variables to restrict the number of input threads to the size of matrices.
int max_threads_allowed;

int threadCount;

// structure for storing individual elements of a matrix.
typedef struct
{
    double x;
} unit;


// A structure for storing rows, columns and reference to elements of a matrix.
typedef struct
{
    int rows;
    int cols;
    unit *x;
} matrix;

matrix A, B, C, _C, target;
pthread_mutex_t mutex;

int thread_counter = 0;



/* 
This function creates matrix of given rows and columns with all elements initialized to 0.0.
It takes in integer inputs for rows and columns, allocates memory for the matrix and returns the initialized matrix.
*/
matrix create_matrix(int rows, int cols)
{
    // matrix target;
    int i, j;
    double temp_data;

    target.rows = rows;
    target.cols = cols;
    target.x = (unit *)malloc(rows * cols * sizeof(unit));
    for (i = 0; i < rows; i++)
        for (j = 0; j < cols; j++)
        {
            temp_data = 0.0F;
            (target.x + i * target.cols + j)->x = temp_data;
        }
    return target;
}


/*
   the function writeinFile  writes the elements of the given matrix to a file named "Output.txt" in a formatted way. 
    It opens the file in write mode, writes the matrix elements to the file and closes the file at the end.
*/
void writeinFile(matrix write_Matrix){
     int rows = write_Matrix.rows;
    int cols = write_Matrix.cols;
     FILE *fptr;
     fptr=fopen("Output.txt","w");
    int i, j;
     for (i = 0; i < rows; i++)
    {
        fprintf(fptr,"[  ");
        
        for (j = 0; j < cols; j++){
          fprintf(fptr,"%lf  ", (write_Matrix.x + i * cols + j)->x);
        }

        
        fprintf(fptr,"]\n");
        
    }
    fprintf(fptr,"\n\n");
}


/*
    the function display_matrix takes a matrix as input and prints its elements in a organized format to the console.
*/
void display_matrix(matrix displayable_matrix)
{
    int rows = displayable_matrix.rows;
    int cols = displayable_matrix.cols;
    int i, j;
   

    for (i = 0; i < rows; i++)
    {
        printf("[  ");
        
        for (j = 0; j < cols; j++){
          printf("%lf  ", (displayable_matrix.x + i * cols + j)->x);
        }
        printf("]\n");
        
    }
    printf("\n\n");
}


/*
    This function calculates the product of two elements from matrices A and B and 
    it takes two integers as input and returns a double value.
*/

double calculate_one_matrix_unit(int first, int second)
{
    int i;
    double res = 0.0F;
    for (i = 0; i < A.cols; i++)
    {
        res += (A.x + first * A.cols + i)->x * (B.x + i * B.cols + second)->x;
    }

    return res;
}

/*
This function is used to handle the number of rows assigned to each thread.
*/
void *multiply_matrices(void *param)
{
    while (1)
    {
        int firstNum;
        int secondNum;
        int i, j, flag = 0, close = 0;
        double res;

        pthread_mutex_lock(&mutex);
        for (i = 0; i < _C.rows; i++)
        {
            for (j = 0; j < _C.cols; j++)
            {
                if ((_C.x + i * _C.cols + j)->x == 0.0F)
                {
                    firstNum = i;
                    secondNum = j;
                    (_C.x + i * _C.cols + j)->x = 1.0F;
                    close = 1;
                    break;
                }
            }
            if (close == 1)
                break;
            else if (i == _C.rows - 1)
                flag = 1;
        }
        pthread_mutex_unlock(&mutex);

        if (flag == 1)
            pthread_exit(NULL);
        res = calculate_one_matrix_unit(firstNum, secondNum);
        (C.x + firstNum * C.cols + secondNum)->x = res;
    }
    pthread_exit(NULL);
}

// this is the main function which gets invoked at runtime.
void main(int argc, char *argv[])
{
     threadCount = strtol(argv[3], NULL, 10);

    if (threadCount <= 0)
    {
        printf("plese enter number of threads after file name");
       exit(0);
    }

    FILE *fp1, *fp2 = NULL;
    int row, col;
    double matval = 0.0;
    int c;

    int MatrixA_Row, MatrixA_Col, MatrixB_Row, MatrixB_Col, MatrixC_Row, MatrixC_Col;

    char *File1 = argv[1];
    char *File2 = argv[2];

    fp1 = fopen(File1, "r");
    fp2 = fopen(File2, "r");

    if (fp1 != NULL && fp2 != NULL)
    {
        int *p;
        int *q;

        p = find_number_of_rows_and_columns(File1);

        MatrixA_Row = *(p + 0);
        MatrixA_Col = *(p + 1);

    

        q = find_number_of_rows_and_columns(File2);

        MatrixB_Row = *(q + 0);
        MatrixB_Col = *(q + 1);

        // Matrix C is the result of combining rows from matrix A and columns from matrix B.
        MatrixC_Row = MatrixA_Row;
        MatrixC_Col = MatrixB_Col;

    

        printf("\nMatrix A \tRows: %d, Columns: %d\n", MatrixA_Row, MatrixA_Col);
        printf("Matrix B \t Rows: %d, Columns: %d\n", MatrixB_Row, MatrixB_Col);
        printf("Output Matrix\t Rows: %d, Columns: %d\n\n", MatrixA_Row, MatrixB_Col);

// Checking the matrix for performing multiplication.

        if (MatrixA_Col == MatrixB_Row)
        {
            // Elements / values present in each matrix
            int matA_elements = MatrixA_Row * MatrixA_Col;
            int matB_elements = MatrixB_Row * MatrixB_Col;
            int matC_elements = MatrixC_Row * MatrixC_Col;

            // Dynamic Memory Allocation
            double *Matrix_A = (double *)malloc(matA_elements * sizeof(double));
            double *Matrix_B = (double *)malloc(matB_elements * sizeof(double));

            matrix target_matA;
            target_matA.rows = MatrixA_Row;
            target_matA.cols = MatrixA_Col;
            target_matA.x = (unit *)malloc(MatrixA_Row * MatrixA_Col * sizeof(unit));

            if (Matrix_A == NULL || Matrix_B == NULL)
            {
                printf("\nError! memory not allocated.\n");
                exit(0);
            }

            // Reading the file and saving the data of matrix A in allocated memory.
            int counter = 0;
            for (row = 0; row < MatrixA_Row; row++)
            {
                for (col = 0; col < MatrixA_Col; col++)
                {
                    fscanf(fp1, "%lf,", Matrix_A + counter);
                    (target_matA.x + row * target_matA.cols + col)->x = *(Matrix_A + counter);
                    counter++;
                }
            }

            printf("\nMatrix A elements >>> \n");
            A = target_matA;
            display_matrix(A);

            matrix target_matB;
            target_matB.rows = MatrixB_Row;
            target_matB.cols = MatrixB_Col;
            target_matB.x = (unit *)malloc(MatrixB_Row * MatrixB_Col * sizeof(unit));

            // Reading the file and saving the data of matrix B in allocated memory.
             counter = 0;
            for (row = 0; row < MatrixB_Row; row++)
            {
                for (col = 0; col < MatrixB_Col; col++)
                {
                    fscanf(fp2, "%lf,", Matrix_B + counter);
                    (target_matB.x + row * target_matB.cols + col)->x = *(Matrix_B + counter);
                    counter;
                }
            }

            printf("Matrix B elements >>> \n");
            B = target_matB;
            display_matrix(B);

            int i;
            C = create_matrix(A.rows, B.cols);
            for (i = 0; i < C.cols * C.rows; i++)
            {
                (C.x + i)->x = 0.0F;
            }

            _C = create_matrix(A.rows, B.cols);
            for (i = 0; i < _C.cols * _C.rows; i++)
            {
                (_C.x + i)->x = 0.0F;
            }

            pthread_t thread_id[threadCount];

            printf("Creating threads and computing the matrix multiplication...\n\n");

            pthread_mutex_init(&mutex, NULL);

            for (int m = 0; m < threadCount; m++)
            {
                pthread_create(&thread_id[m], NULL, multiply_matrices, NULL);
            }

            for (int n = 0; n < threadCount; n++)
            {
                pthread_join(thread_id[n], NULL);
            }

            printf("\n\nOutput matrix C is printed on file output.txt \n");
            writeinFile(C);

            // Deallocating the memory
            free(Matrix_A);
            free(Matrix_B);
            free(target_matA.x);
            free(target_matB.x);
            free(target.x);
        }
        else
        {
            printf("\nOops! the column of matrix A is not equal to the row of matrix B, thus matrices cannot be multiplied.\n");
        }

        fclose(fp1);
        fclose(fp2);
    }
    else
    {
        printf("\nNo such file found!\n");
    }
}


/*
This function is used to find out the dimensions or rows & columns of each matrix from the files.
*/
int *find_number_of_rows_and_columns(const char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    int newRows = 1;
    int newCols = 1;
    char ch;

    static int rows_cols[10];

    while (!feof(fp))
    {
        ch = fgetc(fp);

        if (ch == '\n')
        {
            newRows++;
            // rows_cols[0] = newCols;
            newCols = 1;
        }
        else if (ch == ',')
        {
            newCols++;
        }
    }
    rows_cols[0] = newRows;
    rows_cols[1] = newCols;

    // printf("\nRows: %d, Cols: %d\n", rows_cols[0], rows_cols[1]);

    return rows_cols;
}
