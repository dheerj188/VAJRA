#include <stdio.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> // For socket programming




double calculate_cpu_usage(struct rusage *start, struct rusage *end) {
    double start_user = (double)start->ru_utime.tv_sec + (double)start->ru_utime.tv_usec / 1000000.0;
    double start_sys = (double)start->ru_stime.tv_sec + (double)start->ru_stime.tv_usec / 1000000.0;
    double end_user = (double)end->ru_utime.tv_sec + (double)end->ru_utime.tv_usec / 1000000.0;
    double end_sys = (double)end->ru_stime.tv_sec + (double)end->ru_stime.tv_usec / 1000000.0;

    double total_time = end_user - start_user + end_sys - start_sys;
    return total_time;
}

double *LoadImage(int M, int N, const char *FilePath) {
    FILE *imagefile = fopen(FilePath, "r");
    if (!imagefile) {
        fprintf(stderr, "Error opening file: %s\n", FilePath);
        return NULL;
    }
    double *Image = (double *)malloc(M * N * sizeof(double));
    if (!Image) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(imagefile);
        return NULL;
    }
    for (int i = 0; i < M * N; i++) {
        fscanf(imagefile, "%lf", &Image[i]);
    }
    fclose(imagefile);
    return Image;
}

double *LoadWeights(int M, int N, const char *FilePath) {
    FILE *weightfile = fopen(FilePath, "r");
    if (!weightfile) {
        fprintf(stderr, "Error opening file: %s\n", FilePath);
        return NULL;
    }
    double *weights = (double *)malloc(M * N * sizeof(double));
    if (!weights) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(weightfile);
        return NULL;
    }
    for (int i = 0; i < M * N; i++) {
        fscanf(weightfile, "%lf", &weights[i]);
    }
    fclose(weightfile);
    return weights;
}

double *LoadBiases(int N, const char *FilePath) {
    FILE *biasfile = fopen(FilePath, "r");
    if (!biasfile) {
        fprintf(stderr, "Error opening file: %s\n", FilePath);
        return NULL;
    }
    double *biases = (double *)malloc(N * sizeof(double));
    if (!biases) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(biasfile);
        return NULL;
    }
    for (int i = 0; i < N; i++) {
        fscanf(biasfile, "%lf", &biases[i]);
    }
    fclose(biasfile);
    return biases;
}

double *LinearPass(int M, int N, double *inputs, double *weights, double *biases) {
    double *activations = (double *)calloc(N, sizeof(double));
    if (!activations) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            activations[i] += weights[M * i + j] * inputs[j];
        }
        activations[i] += biases[i];
    }
    free(inputs);
    free(weights);
    free(biases);
    return activations;
}
void sendActivations(double *activations, int N, const char *server_ip) {
    int sock;
    struct sockaddr_in serv_addr;
    int iResult;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        fprintf(stderr, "Error creating socket\n");
        return;
    }

    // Prepare sockaddr_in structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address or address not supported\n");
        close(sock);
        return;
    }

    // Connect to server
    iResult = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (iResult < 0) {
        fprintf(stderr, "Connection failed\n");
        close(sock);
        return;
    }
 iResult = send(sock, activations, N * sizeof(double), 0);
    if (iResult < 0) {
        fprintf(stderr, "Error sending data\n");
    }

    // Cleanup
    close(sock);
}

int main() {

struct rusage start_usage, end_usage;
struct timespec start_time, end_time;
    // Get initial CPU usage
    getrusage(RUSAGE_SELF, &start_usage);
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    int ImgDimx = 28;
    int ImgDimy = 28;
    int InLayerM = 784;
    int InLayerN = 256;
    int Hl1M = 256;
    int Hl1N =128;
    clock_t st,en,str,end;
    double runt,comt;
    st=clock();

    double *InputImage = LoadImage(ImgDimx, ImgDimy, "testimage.txt");
    if (!InputImage) return -1;

    double *InLayerWeights = LoadWeights(InLayerM, InLayerN, "in_layer_weights.txt");
    if (!InLayerWeights) return -1;

    double *InLayerBiases = LoadBiases(InLayerN, "in_layer_biases.txt");
    if (!InLayerBiases) return -1;

    double *activationsL1 = LinearPass(InLayerM, InLayerN, InputImage, InLayerWeights, InLayerBiases);
    double *activationsL1R = ReLU(InLayerN, activationsL1);

    double *Hl1Weights = LoadWeights(Hl1M, Hl1N, "hl1_layer_weights.txt");
    if (!Hl1Weights) return -1;

    double *Hl1Biases = LoadBiases(Hl1N, "hl1_layer_biases.txt");
    if (!Hl1Biases) return -1;

    double *activationsL2 = LinearPass(Hl1M, Hl1N, activationsL1R, Hl1Weights, Hl1Biases);
    double *activationsL2R = ReLU(Hl1N, activationsL2);
    en=clock();
    runt= ((double) (en - st)) / CLOCKS_PER_SEC;
    printf("run time: %f seconds\n", runt);


    sendActivations(activationsL2R, Hl1N, "10.3.31.150"); // Replace with server IP address

getrusage(RUSAGE_SELF, &end_usage);
 clock_gettime(CLOCK_MONOTONIC, &end_time);


    // Calculate CPU usage
    double cpu_time_used = calculate_cpu_usage(&start_usage, &end_usage);

double start_wall = (double)start_time.tv_sec + (double)start_time.tv_nsec / 1000000000.0;
    double end_wall = (double)end_time.tv_sec + (double)end_time.tv_nsec / 1000000000.0;
    double wall_time_elapsed = end_wall - start_wall;

    // Calculate CPU usage percentage
    double cpu_usage_percent = (cpu_time_used / wall_time_elapsed) * 100.0;



printf("CPU usage percentage: %f%%\n", cpu_usage_percent);


    free(activationsL2R);

    return 0;
