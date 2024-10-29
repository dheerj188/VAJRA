# PERFORMS a 100x100 matrix multiplication across each node

import paramiko
import queue
import threading
import time
import psutil
import os
import subprocess
import matplotlib.pyplot as plt

# Configuration
NODES = [
    {'host': '10.3.31.152', 'user': 'node_1', 'password': 'cie123'},
    {'host': '10.3.31.150', 'user': 'node_2', 'password': 'cie123'},
    {'host': '10.3.31.151', 'user': 'node_3', 'password': 'cie123'}
]
TASK_QUEUE = queue.Queue()
NODE_QUEUE = queue.Queue()
LOCK = threading.Lock()
NODE_TIMES = {node['host']: [] for node in NODES}

# Matrix multiplication task for 100x100 matrices
matrix_multiplication_script = """
import numpy as np
A = np.random.rand(100, 100)
B = np.random.rand(100, 100)
C = np.dot(A, B)
print(C)
"""

# Create a temporary script file
with open('matrix_multiplication.py', 'w') as f:
    f.write(matrix_multiplication_script)

# Initialize task queue
for _ in range(70):  # Repeat the task 70 times for demonstration
    TASK_QUEUE.put('matrix_multiplication.py')

# Initialize node queue
for node in NODES:
    NODE_QUEUE.put(node)

def ping_node(host):
    result = subprocess.run(['ping', '-c', '1', host], stdout=subprocess.PIPE)
    output = result.stdout.decode()
    if 'time=' in output:
        latency = output.split('time=')[1].split(' ')[0]
        return float(latency)
    return None

def execute_task_on_node(node, task_file):
    start_time = time.time()
    try:
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        ssh.connect(node['host'], username=node['user'], password=node['password'])

        sftp = ssh.open_sftp()
        sftp.put(task_file, '/tmp/matrix_multiplication.py')
        sftp.close()

        print(f"Executing {task_file} on {node['host']}")
        stdin, stdout, stderr = ssh.exec_command('python3 /tmp/matrix_multiplication.py')
        output = stdout.read().decode().strip()
        print(f"Output from {node['host']}: {output}")
        ssh.close()
    except paramiko.ssh_exception.NoValidConnectionsError as e:
        print(f"Failed to connect to {node['host']}: {e}")
    except Exception as e:
        print(f"An error occurred while executing {task_file} on {node['host']}: {e}")
    end_time = time.time()
    task_time = end_time - start_time
    print(f"Task on {node['host']} took {task_time:.2f} seconds")
    
    with LOCK:
        NODE_TIMES[node['host']].append(task_time)

def worker():
    while not TASK_QUEUE.empty():
        task_file = TASK_QUEUE.get()

        with LOCK:
            node = NODE_QUEUE.get()

        execute_task_on_node(node, task_file)

        with LOCK:
            NODE_QUEUE.put(node)

        TASK_QUEUE.task_done()

# Measure CPU and memory usage before running the tasks
cpu_usage_before = psutil.cpu_percent(interval=1)
memory_usage_before = psutil.virtual_memory().percent

# Measure network latency to each node
for node in NODES:
    latency = ping_node(node['host'])
    if latency is not None:
        print(f"Network latency to {node['host']}: {latency} ms")
    else:
        print(f"Failed to measure latency to {node['host']}")

# Start the benchmark
start_time = time.time()

# Create and start threads
threads = []
for _ in range(len(NODES)):
    t = threading.Thread(target=worker)
    t.start()
    threads.append(t)

# Wait for all tasks to complete
TASK_QUEUE.join()

# Ensure all threads have finished
for t in threads:
    t.join()

end_time = time.time()

# Measure CPU and memory usage after running the tasks
cpu_usage_after = psutil.cpu_percent(interval=1)
memory_usage_after = psutil.virtual_memory().percent

print("All tasks completed.")
print(f"Total execution time: {end_time - start_time:.2f} seconds")
print(f"CPU usage before: {cpu_usage_before}%")
print(f"CPU usage after: {cpu_usage_after}%")
print(f"Memory usage before: {memory_usage_before}%")
print(f"Memory usage after: {memory_usage_after}%")

# Cleanup temporary script file
os.remove('matrix_multiplication.py')

# Generate bar graph comparing each node's output times
node_averages = {node: sum(times) / len(times) for node, times in NODE_TIMES.items()}
nodes = list(node_averages.keys())
averages = list(node_averages.values())

plt.bar(nodes, averages)
plt.xlabel('Nodes')
plt.ylabel('Average Task Time (seconds)')
plt.title('Average Task Time per Node')
plt.show()
