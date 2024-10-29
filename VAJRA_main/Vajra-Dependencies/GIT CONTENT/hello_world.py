
# EXECUTES HELLO WORLD 5 TIMES ON EACH NODE
import paramiko
import queue
import threading

# Configuration
NODES = [
    {'host': '10.3.31.105', 'user': 'node_1', 'password': 'cie123'},
    {'host': '10.3.31.112', 'user': 'node_3', 'password': 'cie123'},
    {'host': '10.3.31.111', 'user': 'node_2', 'password': 'cie123'}
]
TASKS = ['echo "Hello, World!"'] * 5  # Repeat the task 5 times for demonstration
TASK_QUEUE = queue.Queue()
NODE_QUEUE = queue.Queue()
LOCK = threading.Lock()

# Initialize task queue
for task in TASKS:
    TASK_QUEUE.put(task)

# Initialize node queue
for node in NODES:
    NODE_QUEUE.put(node)

def execute_task_on_node(node, task):
    try:
        ssh = paramiko.SSHClient()
        ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
        ssh.connect(node['host'], username=node['user'], password=node['password'])

        print(f"Executing {task} on {node['host']}")
        stdin, stdout, stderr = ssh.exec_command(task)
        output = stdout.read().decode().strip()
        print(f"Output from {node['host']}: {output}")
        ssh.close()
    except paramiko.ssh_exception.NoValidConnectionsError as e:
        print(f"Failed to connect to {node['host']}: {e}")
    except Exception as e:
        print(f"An error occurred while executing {task} on {node['host']}: {e}")

def worker():
    while not TASK_QUEUE.empty():
        task = TASK_QUEUE.get()

        with LOCK:
            node = NODE_QUEUE.get()

        execute_task_on_node(node, task)

        with LOCK:
            NODE_QUEUE.put(node)

        TASK_QUEUE.task_done()

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

print("All tasks completed.")
