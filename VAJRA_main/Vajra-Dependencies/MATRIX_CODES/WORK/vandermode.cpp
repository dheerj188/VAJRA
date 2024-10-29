#include <iostream>
#include <vector>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <iomanip>
#include <cmath>

using namespace std;


vector<double> multiplyVandermondeSparseMatrix(const vector<double>& base, const vector<double>& vec, const vector<int>& col_pointers) {
    int n = vec.size();
    vector<double> result(n, 0.0);

    
    for (size_t col = 0; col < col_pointers.size() - 1; ++col) {
        for (int i = 0; i < n; ++i) {
            result[i] += pow(base[i], col) * vec[col]; 
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
    const int numPowers = 5; 

    
    vector<double> base(size);
    for (int i = 0; i < size; ++i) {
        base[i] = static_cast<double>(i + 1) / size; 
    }

    
    vector<int> col_pointers(numPowers + 1, 0); 
    for (int i = 0; i <= numPowers; ++i) {
        col_pointers[i] = i; 
    }

   
    vector<double> vectorToMultiply(numPowers, 1.0); 
   
    vector<double> result = multiplyVandermondeSparseMatrix(base, vectorToMultiply, col_pointers);

   
    printResourceUsage("Sparse Vandermonde Matrix", size);

    return 0;
}
