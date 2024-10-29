# VAJRA: Versatile AI Junction for Resource-constrained Edge Applications

for our open-source software please visit the "VAJRA-main" directory

We have built the "VAJRA" an edge server for computing deep learning workloads which are usually heavy to deploy at the extreme edge devices of the network. VAJRA mimics an Edge-Cloud scenario, operating between edge devices and the cloud, co-optimizing latency and throughput. 

Hardware: VAJRA hosts a Raspberry Pi-5 as a master node, that acts like a gateway router to the 3 Intel DE-10 SoC FPGAs. The aim is to scale the number of parameters for resource-constrained edge devices, by employing HPC techniques at the Edge. 

![Screenshot 2024-10-29 at 1 58 23 PM](https://github.com/user-attachments/assets/d491803c-6292-4b17-a21c-5dd9a6ad3f74)
                         Figure-1: VAJRA System

![Screenshot 2024-10-29 at 2 03 08 PM](https://github.com/user-attachments/assets/7e495db6-249e-4bec-ae9c-4ff1c14ae527)
                        Figure-2: System Arhcitecture

# Platform Specific Project: System Scheduler 
Team VAJRA is building a lightweight scheduler on the master node. This will aim to co-optimize latency and throughput. 

![Screenshot 2024-10-29 at 2 05 26 PM](https://github.com/user-attachments/assets/ccad0526-3cbf-4fc8-a3b3-96b0c2e4cbfd)

### Embedded Deep Learning Research 
Team VAJRA focuses on delivering reconfigurable compute facilities to their users to elevate performance at the edge. We aim to deploy openCL-based kernels on the device to ensure Power-Time-Memory are optimized throughout the inference cycle. 

### Software for Optimization: VAJRA model Analyzer (VMA)
VMA enables model parallelism for deep learning on our multi-node system. It optimally analyzes the memory requirement for each model, and optimally partitions it to fit on the system. We scale our parameters upto 400 million parameters using the VMA for VAJRA. 

![Screenshot 2024-10-29 at 2 05 32 PM](https://github.com/user-attachments/assets/84173fab-7314-4647-87cd-5c0793558597)

### Applications on VAJRA (Future Work)



