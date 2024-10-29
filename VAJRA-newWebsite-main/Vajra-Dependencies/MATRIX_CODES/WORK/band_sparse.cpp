#include <iostream>
#include <vector>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <iomanip>

using namespace std;

vector<double> multiplyBandSparseMatrix(const vector<vector<double>>& matrix, const vector<double>& vec, int lowerBandwidth, int upperBandwidth) {
    size_t size = vec.size();
    vector<double> result(size, 0.0);

    
    for (size_t i = 0; i < size; ++i) {
  
        for (int j = max(0, (int)i - lowerBandwidth); j <= min((int)size - 1, (int)i + upperBandwidth); ++j) {
            result[i] += matrix[i][j - i + upperBandwidth] * vec[j];
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

int main() {
    const int size = 30000; 
    const int lowerBandwidth = 2; 
    const int upperBandwidth = 2; 
    // Initialize a band sparse matrix
    vector<vector<double>> matrix(size, vector<double>(lowerBandwidth + upperBandwidth + 1, 0.0));
    vector<double> vectorToMultiply(size, 1.0); 
 
    for (int i = 0; i < size; ++i) {
        for (int j = max(0, i - lowerBandwidth); j <= min(size - 1, i + upperBandwidth); ++j) {
            matrix[i][j - i + upperBandwidth] = static_cast<double>(i + j + 1); 
        }
    }

    
    vector<double> result = multiplyBandSparseMatrix(matrix, vectorToMultiply, lowerBandwidth, upperBandwidth);

    printResourceUsage("Band Sparse Matrix", size);
    
    return 0;
}
