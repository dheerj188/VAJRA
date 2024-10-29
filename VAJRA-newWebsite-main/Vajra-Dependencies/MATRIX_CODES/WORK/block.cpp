#include <iostream>
#include <vector>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <iomanip>

using namespace std;


vector<double> multiplyBlockDiagonalMatrix(const vector<vector<double> >& blocks, const vector<double>& vec) {
    size_t vecSize = vec.size();
    vector<double> result(vecSize, 0.0);

    
    for (const vector<double>& block : blocks) {
        size_t blockSize = block.size();
        for (size_t i = 0; i < blockSize; ++i) {
            
            if (i < vecSize) {
                result[i] += block[i] * vec[i];
            }
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

    
    double userCPUTime = (double)usage.ru_utime.tv_sec + (double)usage.ru_utime.tv_usec / 1e6;
    double systemCPUTime = (double)usage.ru_stime.tv_sec + (double)usage.ru_stime.tv_usec / 1e6;
    double totalCPUTime = userCPUTime + systemCPUTime;

  
    if (totalCPUTime > 0) {
        double cpuUtilization = (totalCPUTime / totalCPUTime) * 100.0; 
        cout << "CPU Utilization: " << cpuUtilization << " %" << endl; 
    } else {
        cout << "CPU Utilization: 0.00 %" << endl; // If total CPU time is zero
    }
}

int main() {
    const int blockSize = 130; 
    const int numBlocks =100; 
    const int size = blockSize * numBlocks; 

   
    vector<vector<double> > blocks(numBlocks, vector<double>(blockSize, 0.0));
    vector<double> vectorToMultiply(size, 1.0); 
    
    for (int b = 0; b < numBlocks; ++b) {
        for (int i = 0; i < blockSize; ++i) {
            blocks[b][i] = static_cast<double>(i + 1 + b * blockSize); 
        }
    }

    
    vector<double> result = multiplyBlockDiagonalMatrix(blocks, vectorToMultiply);

    
    printResourceUsage("Block Diagonal Matrix", size);

    return 0;
}

