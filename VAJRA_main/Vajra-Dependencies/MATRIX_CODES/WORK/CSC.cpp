#include <iostream>
#include <vector>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <iomanip>

using namespace std;


vector<double> multiplyCSCMatrix(const vector<double>& values, const vector<int>& row_indices, const vector<int>& col_pointers, const vector<double>& vec) {
    vector<double> result(vec.size(), 0.0);

    for (size_t col = 0; col < col_pointers.size() - 1; ++col) {
        for (int i = col_pointers[col]; i < col_pointers[col + 1]; ++i) {
            result[row_indices[i]] += values[i] * vec[col];
        }
    }

    return result;
}


void printResourceUsage(const string& matrixName, int size) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

  
    long memoryUsage = usage.ru_maxrss; 
    struct sysinfo sysInfo;
    sysinfo(&sysInfo);
    long totalMemory = sysInfo.totalram / 1024; 
    double ramUtilization = (static_cast<double>(memoryUsage) / totalMemory) * 100.0; 

    cout << fixed << setprecision(5);
    cout << "Matrix Name: " << matrixName << endl;          
    cout << "Matrix Size: " << size << " x " << size << endl;
    cout << "RAM Utilization: " << ramUtilization << " %" << endl; 

    // CPU utilization (approximation)
    double userCPUTime = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1e6;
    double systemCPUTime = (double)usage.ru_stime.tv_sec + (double)usage.ru_stime.tv_usec / 1e6;
    double totalCPUTime = userCPUTime + systemCPUTime;

    if (totalCPUTime > 0) {
        double cpuUtilization = (totalCPUTime / totalCPUTime) * 100.0; 
        cout << "CPU Utilization: " << cpuUtilization << " %" << endl;
    } else {
        cout << "CPU Utilization: 0.00 %" << endl;
    }
}

int main() {
    const int size = 90000; 
    const int numNonZero = 300000; 

  
    vector<double> values(numNonZero, 1.0);
    vector<int> row_indices(numNonZero, 0); 
    vector<int> col_pointers(size + 1, 0);

  
    for (int i = 0; i < numNonZero; ++i) {
        row_indices[i] = i % size; 
    }

    for (int i = 0; i < size; ++i) {
        col_pointers[i] = i * (numNonZero / size);
    }
    col_pointers[size] = numNonZero;

    vector<double> vectorToMultiply(size, 1.0);

   
    vector<double> result = multiplyCSCMatrix(values, row_indices, col_pointers, vectorToMultiply);


    printResourceUsage("Compressed Sparse Column Matrix", size);

    return 0;
}
