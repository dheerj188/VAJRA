import paramiko
import queue
import threading
import time
import numpy as np
import psutil
import matplotlib.pyplot as plt

NODES = [
    {'host': '10.3.31.152', 'user': 'node_1', 'password': 'cie123', 'task_count': 0},
    {'host': '10.3.31.150', 'user': 'node_3', 'password': 'cie123', 'task_count': 0},
    {'host': '10.3.31.151', 'user': 'node_2', 'password': 'cie123', 'task_count': 0}
]


TASKS = [
    {'task': 'vector_addition', 'weight': 1},
    {'task': 'matrix_multiplication', 'weight': 3},
    {'task': 'array_sorting', 'weight': 2}
]

TASK_QUEUE = queue.Queue()
NODE_QUEUE = queue.Queue()
LOCK = threading.Lock()

# Vector addition (light task)
def vector_addition():
    vector1 = np.random.randint(1, 10, size=1000)
    vector2 = np.random.randint(1, 10, size=1000)
    result = vector1 + vector2
    print(f"Vector addition result (first 5): {result[:5]}")

# Matrix multiplication (heavy task)
def matrix_multiplication():
    matrix1 = np.random.randint(1, 10, size=(100, 100))
    matrix2 = np.random.randint(1, 10, size=(100, 100))
    result = np.dot(matrix1, matrix2)
    print(f"Matrix multiplication result (first 2x2):\n{result[:2, :2]}")

# Array sorting (medium task)
def array_sorting():
    array = np.random.randint(1, 10000, size=5000)
    result = sorted(array)
    print(f"Array sorting result (first 5): {result[:5]}")


def initialize_weighted_task_queue():
    for task_info in TASKS:
        task = task_info['task']
        weight = task_info['weight']
        
        for _ in range(weight):
            TASK_QUEUE.put(task)

initialize_weighted_task_queue()


for node in NODES:
    NODE_QUEUE.put(node)

def execute_task_on_node(node, task):
    start_time = time.time()
    try:
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        ssh.connect(node['host'], username=node['user'], password=node['password'])

        print(f"Executing {task} on {node['host']}")

        if task == 'vector_addition':
            vector_addition()
        elif task == 'matrix_multiplication':
            matrix_multiplication()
        elif task == 'array_sorting':
            array_sorting()
        else:
            print(f"Unknown task: {task}")

        ssh.close()

        
        with LOCK:
            node['task_count'] += 1

    except paramiko.ssh_exception.NoValidConnectionsError as e:
        print(f"Failed to connect to {node['host']}: {e}")
    except Exception as e:
        print(f"An error occurred while executing {task} on {node['host']}: {e}")
    end_time = time.time()
    print(f"Task on {node['host']} took {end_time - start_time:.2f} seconds")

def worker():
    while not TASK_QUEUE.empty():
        task = TASK_QUEUE.get()

        with LOCK:
            node = NODE_QUEUE.get()

        execute_task_on_node(node, task)

        with LOCK:
            NODE_QUEUE.put(node)

        TASK_QUEUE.task_done()


cpu_usage_before = psutil.cpu_percent(interval=1)
memory_usage_before = psutil.virtual_memory().percent


start_time = time.time()


threads = []
for _ in range(len(NODES)):
    t = threading.Thread(target=worker)
    t.start()
    threads.append(t)


TASK_QUEUE.join()


for t in threads:
    t.join()

end_time = time.time()


cpu_usage_after = psutil.cpu_percent(interval=1)
memory_usage_after = psutil.virtual_memory().percent

print("All tasks completed.")
print(f"Total execution time: {end_time - start_time:.2f} seconds")
print(f"CPU usage before: {cpu_usage_before}%")
print(f"CPU usage after: {cpu_usage_after}%")
print(f"Memory usage before: {memory_usage_before}%")
print(f"Memory usage after: {memory_usage_after}%")


node_hosts = [node['host'] for node in NODES]
task_counts = [node['task_count'] for node in NODES]


plt.bar(node_hosts, task_counts)
plt.xlabel('Node')
plt.ylabel('Number of Tasks Executed')
plt.title('Workload Distribution Across Nodes')
plt.show()
