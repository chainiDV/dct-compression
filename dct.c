#include <stdio.h>
#include <math.h>

#define N 8

float c_func(int num);

/* Orden que el Run Length Encoder debe recorrer el array de quantized*/
int zigzagOrder[N * N] = {
    0, 1, 8, 16, 9, 2, 3, 10, 17, 24, 32, 25, 18, 11, 4, 5, 12, 19, 26, 33, 40, 48, 41, 34, 27, 20, 13, 6, 7, 14, 21, 28, 35, 42, 49, 56, 57, 50, 43, 36, 29, 22, 15, 23, 30, 37, 44, 51, 58, 59, 52, 45, 38, 31, 39, 46, 53, 60, 61, 54, 47, 55, 62, 63
};

int calculateRunLengthEncoder(int** result, int quantized[N][N]) {
    //Generate array of arrays. Each element of main array will have an array of two elements
    //First element will be the value, second element will be the number of zeros that precede it
    //Example: { {25, 0}, {3, 3}, {5,8}}

    
    int result_index = 0;
    int zero_count = 0;

    for (int i = 0; i < N * N; i++) {
        int row = zigzagOrder[i] / N;
        int col = zigzagOrder[i] % N;
        int value = quantized[row][col];

        if(value != 0){
            result[result_index][0] = value;
            result[result_index][1] = zero_count;
            zero_count = 0;
            result_index++;
        } else {
            zero_count++;
        }
    }

    return result_index;
}

/* Calculate quantized matrix */
void calculateQuantized(int dct[N][N], int quant[N][N], int quantized[N][N]) {

    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            quantized[i][j] = round(dct[i][j] / quant[i][j]);
        }
    }
}


void calculateDCT(int matrix[N][N], int dct[N][N]) {
    double ci, cj, sum;
    double pi = 3.14159265358979323846;

    for(int k = 0; k < N; k++){
        for(int l = 0; l < N; l++){
            double result = 0.25*c_func(k) * c_func(l);
            double sum = 0;
            
            for(int x = 0; x < N; x++){
                for(int y = 0; y < N; y++){
                    sum += matrix[x][y] * cos((2*x + 1) * k * pi / 2*N) * cos((2*y + 1) * l * pi / 2*N);
                }
            }

            dct[k][l] = result * sum;
        }
    }
}

float c_func(int num) {
    if(num == 0){
        return 1.0/sqrt(2);
    } else {
        return 1.0;
    }
}

//Debe leer de un nombre que se pasa como argumento
void readMatrix(char* nombre, int matrix[N][N]) {
    FILE *file = fopen(nombre, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if( j < N - 1){
                fscanf(file, "%d, ", &matrix[i][j]);
            } else {
                fscanf(file, "%d", &matrix[i][j]);
            }
            
        }
    }

    fclose(file);

}

void printMatrix(char* nombre, int matrix[N][N]) {
    //Debo escribir la matriz en un archivo txt
    FILE *file = fopen(nombre, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void printMatrixConsole(int matrix[N][N]) {
    //Debo escribir la matriz en un archivo txt
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

void printRLEResult(char* nombre, int** result, int length) {
    FILE *file = fopen(nombre, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < length; i++) {
        fprintf(file, "(%d, %d) ", result[i][0], result[i][1]);
    }

    fclose(file);
}

void allocateResultArray(int ***result) {
    *result = (int **)malloc(N * N * sizeof(int *));
    for (int i = 0; i < N * N; i++) {
        (*result)[i] = (int *)malloc(2 * sizeof(int));
    }
}

int main(int argc, char *argv[]) {
    int matrix[N][N];
    int dct[N][N];
    int quant[N][N];
    int quantized[N][N];

    if(argc != 3){
        printf("Usage: %s <matrix_file> <quant_file>\n", argv[0]);
        return 1;
    }

    readMatrix(argv[1], matrix);
    readMatrix(argv[2], quant);
    
  //  printMatrixConsole(quant);

    calculateDCT(matrix, dct);
    printMatrix("matrix_dct_output.txt", dct);

    calculateQuantized(dct, quant, quantized);

    printMatrix("quantized_output.txt", quantized);

    int** runLengthEncoder;
    allocateResultArray(&runLengthEncoder);

    int runArrayLength = calculateRunLengthEncoder(runLengthEncoder,quantized);

    printRLEResult("rle_output.txt", runLengthEncoder, runArrayLength);

    //Free memory
    for (int i = 0; i < runArrayLength; i++) {
        free(runLengthEncoder[i]);
    }

    free(runLengthEncoder);

    return 0;
}
