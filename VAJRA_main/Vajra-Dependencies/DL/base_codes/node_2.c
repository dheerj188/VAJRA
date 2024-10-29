include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <time.h>
#include <sys/resource.h>



double calculate_cpu_usage(struct rusage *start, struct rusage *end) {
    double start_user = (double)start->ru_utime.tv_sec + (double)start->ru_utime.tv_usec / 1000000.0;
    double start_sys = (double)start->ru_stime.tv_sec + (double)start->ru_stime.tv_usec / 1000000.0;
    double end_user = (double)end->ru_utime.tv_sec + (double)end->ru_utime.tv_usec / 1000000.0;
    double end_sys = (double)end->ru_stime.tv_sec + (double)end->ru_stime.tv_usec / 1000000.0;

    double total_time = (end_user - start_user) + (end_sys - start_sys);
    return total_time;
}



double *LoadWeights(int M, int N, const char *FilePath) {
    FILE *weightfile = fopen(FilePath, "r");
    if (!weightfile) {
        fprintf(stderr, "Error opening file: %s\n", FilePath);
        return NULL;
    }
    double *weights = (double *)malloc(M * N * sizeof(double));
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
    for (int i = 0; i < N; i++) {
        fscanf(biasfile, "%lf", &biases[i]);
    }
    fclose(biasfile);
    return biases;
}

double *ReLU(int N, double *activations) {
    for (int i = 0; i < N; i++) {
        if (activations[i] < 0.0) {
            activations[i] = 0.0;
        }
    }
    return activations;
}

double *LinearPass(int M, int N, double *inputs, double *weights, double *biases) {
    double *activations = (double *)calloc(N, sizeof(double));
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

double *receiveActivations(int N) {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    double *activations = (double *)calloc(N, sizeof(double));

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        fprintf(stderr, "Error creating socket\n");
        return activations;
    }

    // Prepare sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    // Bind
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "Bind failed\n");
        close(server_sock);
        return activations;
    }
// Listen
    if (listen(server_sock, 3) < 0) {
        fprintf(stderr, "Listen failed\n");
        close(server_sock);
        return activations;
    }

    // Accept incoming connections
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_sock < 0) {
        fprintf(stderr, "Accept failed\n");
        close(server_sock);
        return activations;
    }

    // Receive data
    int received_bytes = recv(client_sock, activations, N * sizeof(double), 0);
    if (received_bytes > 0) {
        printf("Received %d bytes from client.\n", received_bytes);
        // Process received data as needed
    } else if (received_bytes == 0) {
        printf("Connection closed.\n");
    } else {
        fprintf(stderr, "Receive failed\n");
    }

    // Cleanup
    close(client_sock);
    close(server_sock);

    return activations;
}
int main() {
    struct rusage start_usage, end_usage;
    struct timespec start_time, end_time;

    // Get initial CPU usage and wall clock time
    getrusage(RUSAGE_SELF, &start_usage);
    clock_gettime(CLOCK_MONOTONIC, &start_time);

    clock_t start, end;
    double cputime;

    start = clock();

    int Hl2M = 128;
    int Hl2N = 64;
    int OutLayerM = 64;
    int OutLayerN = 10;

    double *activationsL2R = receiveActivations(Hl2M);
    if (activationsL2R) {
        // Process received activations
        for (int i = 0; i < Hl2M; ++i) {
          //  printf("Received activation[%d]: %lf\n", i, activationsL2R[i]);
        }
    }

    end = clock();
    cputime = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("communication time: %f seconds\n", cputime);

    clock_t st,en;
    double runtime;
    st=clock();

    double *Hl2Weights = LoadWeights(Hl2M, Hl2N, "hl2_layer_weights.txt");
    double *Hl2Biases = LoadBiases(Hl2N, "hl2_layer_biases.txt");
 double *activationsL3 = LinearPass(Hl2M, Hl2N, activationsL2R, Hl2Weights, Hl2Biases);
    double *activationsL3R = ReLU(Hl2N, activationsL3);

    double *OutLayerWeights = LoadWeights(OutLayerM, OutLayerN, "out_layer_weights.txt");
    double *OutLayerBiases = LoadBiases(OutLayerN, "out_layer_biases.txt");

    double *activationsOut = LinearPass(OutLayerM, OutLayerN, activationsL3R, OutLayerWeights, OutLayerBiases);
    en = clock();
    runtime = ((double) (en - st)) / CLOCKS_PER_SEC;
    printf("execution time: %f seconds\n", runtime);

  getrusage(RUSAGE_SELF, &end_usage);
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    // Calculate CPU time used
    double cpu_time_used = calculate_cpu_usage(&start_usage, &end_usage);

    // Calculate wall clock time elapsed
    double start_wall = (double)start_time.tv_sec + (double)start_time.tv_nsec / 1000000000.0;
    double end_wall = (double)end_time.tv_sec + (double)end_time.tv_nsec / 1000000000.0;
    double wall_time_elapsed = end_wall - start_wall;

    // Calculate CPU usage percentage
    double cpu_usage_percent = (cpu_time_used / wall_time_elapsed) * 100.0;
    printf("CPU usage percentage: %f%%\n", cpu_usage_percent);
    for (int i = 0; i < 10; i++) {
        printf("%lf\n", activationsOut[i]);
    }



    free(activationsOut);



    return 0;
                    