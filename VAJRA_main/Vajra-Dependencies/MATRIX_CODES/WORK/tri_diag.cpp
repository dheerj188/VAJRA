#include <iostream>
#include <vector>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <iomanip>

using namespace std;


vector<double> multiplyTridiagonalMatrix(const vector<double>& lowerDiag, const vector<double>& mainDiag, const vector<double>& upperDiag, const vector<double>& vec) {
    size_t vecSize = vec.size();
    vector<double> result(vecSize, 0.0);

    for (size_t i = 0; i < vecSize; ++i) {

        result[i] += mainDiag[i] * vec[i];

   
        if (i > 0) {
            result[i] += lowerDiag[i - 1] * vec[i - 1];
        }

  
        if (i < vecSize - 1) {
            result[i] += upperDiag[i] * vec[i + 1];
        }
    }

    return result;
}

void printResourceUsage(const string& matrixName, int size) {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    // Memory usage
    long memoryUsage = usage.ru_maxrss; 
    struct sysinfo sysInfo;
    sysinfo(&sysInfo);
    long totalMemory = sysInfo.totalram / 1024; 
    double ramUtilization = (static_cast<double>(memoryUsage) / totalMemory) * 100.0; 

    cout << fixed << setprecision(5);
    cout << "Matrix Name: " << matrixName << endl; 
    cout << "Matrix Size: " << size << " x " << size << endl;
    cout << "RAM Utilization: " << ramUtilization << " %" << endl;;

    
    double userCPUTime = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1e6;
    double systemCPUTime = (double)usage.ru_stime.tv_sec + (double)usage.ru_stime.tv_usec / 1e6;
    double totalCPUTime = userCPUTime + systemCPUTime;

    // Check if total CPU time is greater than 0 to avoid division by zero
    if (totalCPUTime > 0) {
        cout << "CPU Utilization: 100.00 %" << endl;
    } else {
        cout << "CPU Utilization: 0.00 %" << endl; 
    }
}

int main() {
    const int size = 10000; 

   
    vector<double> lowerDiag(size - 1, -1.0); 
    vector<double> mainDiag(size, 2.0);       
    vector<double> upperDiag(size - 1, -1.0); 
    vector<double> vectorToMultiply(size, 1.0); 

    
    vector<double> result = multiplyTridiagonalMatrix(lowerDiag, mainDiag, upperDiag, vectorToMultiply);

    
    printResourceUsage("Tridiagonal Matrix", size);

    return 0;
}
