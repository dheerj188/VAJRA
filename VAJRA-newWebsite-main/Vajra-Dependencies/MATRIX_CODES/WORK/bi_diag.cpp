#include <iostream>
#include <vector>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <iomanip>

using namespace std;


vector<double> multiplyBidiagonalMatrix(const vector<double>& mainDiagonal, const vectoe>& lowerDiagonal, const vector<double>& vec) {
    size_t size = vec.size();
    vector<double> result(size, 0.0);

  
    for (size_t i = 0; i < size; ++i) {
        result[i] = mainDiagonal[i] * vec[i];
    }

    
    for (size_t i = 1; i < size; ++i) { 
        result[i] += lowerDiagonal[i - 1] * vec[i - 1];
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
    double ramUtilization = (static_cast<double>(memoryUsage) / totalMemory) * 100.0; /tilization percentage

    cout << fixed << setprecision(5);
    cout << "Matrix Name: " << matrixName << endl;        
    cout << "Matrix Size: " << size << " x " << size << endl;
    cout << "RAM Utilization: " << ramUtilization << " %" << endl; 

   
    double userCPUTime = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec
    double systemCPUTime = (double)usage.ru_stime.tv_sec + (double)usage.ru_stime.tv_us6;
    double totalCPUTime = userCPUTime + systemCPUTime;

    
    if (totalCPUTime > 0) {
        double cpuUtilization = (totalCPUTime / totalCPUTime) * 100.0;
        cout << "CPU Utilization: " << cpuUtilization << " %" << endl;
    } else {
        cout << "CPU Utilization: 0.00 %" << endl; 
    }
}

int main() {
    const int size = 10000; 

    
    vector<double> mainDiagonal(size, 0.0);
    vector<double> lowerDiagonal(size - 1, 0.0);
    vector<double> vectorToMultiply(size, 1.0);

    // Fill the diagonals with example values
    for (int i = 0; i < size; ++i) {
        mainDiagonal[i] = static_cast<double>(i + 1); 
    }
    for (int i = 0; i < size - 1; ++i) {
        lowerDiagonal[i] = static_cast<double>(i + 1); 
    }

    
    vector<double> result = multiplyBidiagonalMatrix(mainDiagonal, lowerDiagonal, vectoiply);

    
    printResourceUsage("Bidiagonal Matrix", size);

    return 0;
}
