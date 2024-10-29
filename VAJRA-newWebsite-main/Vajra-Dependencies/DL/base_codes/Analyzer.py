"""import json
import os
import subprocess

remote_user="node_1"
remote_server="10.3.31.152"
remote_path="/home/node_1/DEEP_LEARNING/"


# Define the maximum number of nodes
MAX_NODES = 3

# Function to read disk space from the file
def read_disk_space(filename):
    node_spaces = {}
    with open(filename, 'r') as file:
        for line in file:
            node, space = line.strip().split(':')
            node_spaces[node] = int(space)
    return node_spaces

# Function to read model configuration from the JSON file
def read_model_config(filename):
    with open(filename, 'r') as file:
        model_config = json.load(file)
    return model_config

# Function to calculate the number of nodes needed based on fixed node sizes
def calculate_nodes_needed(file_size, node_spaces):
    num_nodes = 0
    remaining_size = file_size  # Use remaining_size to track leftover size

    for node, space in node_spaces.items():
        if remaining_size > space:
            num_nodes += 1
            remaining_size -= space  # Use -= to correctly subtract node space
        else:
            num_nodes += 1
            break

    return num_nodes

if __name__ == "__main__":
    # Read disk space from the file
    node_spaces = read_disk_space('disk_space.txt')

    # Read model configuration from the JSON file
    model_config = read_model_config('parameters.json')

    # Extract values from the model configuration
    num_layers = model_config['layers']
    dimensions = model_config['layer_dimensions']
    file_size = model_config['file_size_bytes']
    job_id= model_config['id']

    # Calculate the number of nodes needed
    num_nodes = calculate_nodes_needed(file_size, node_spaces)

    # Print the extracted values
    print(f"File size: {file_size}")
    print(f"Node sizes: {node_spaces}")
    print(f"Number of nodes required: {num_nodes}")

    # Print manually input dimensions
    print(f"Number of layers: {num_layers}")
    print("Dimensions: ", end="")
    for dim in dimensions:
        print(dim, end=" ")
    print()
    keys_file="/home/cie/Desktop/SCHEDULE/"+job_id+"__/"+job_id+"_keys.txt"
    with open(keys_file,"r") as K:
        filenames=[line.strip() for line in K.readlines()]
    for filename in filenames:
        local_file=filename+".txt"
        subprocess.run("scp",local_file,f"{remote_user}@{remote_server}:{remote_path}")
    subprocess.run(["scp","parameters.json",f"{remote_user}@{remote_server}:{remote_path}"])

"""



""" 888import json
import os
import subprocess

password = "cie123" 

remote_user = "node_1"
remote_server = "10.3.31.152"
remote_path = "/home/node_1/DEEP_LEARNING/"

# Define the maximum number of nodes
MAX_NODES = 3

# Function to read disk space from the file
def read_disk_space(filename):
    node_spaces = {}
    try:
        with open(filename, 'r') as file:
            for line in file:
                node, space = line.strip().split(':')
                node_spaces[node] = int(space)
    except FileNotFoundError:
        print(f"Error: {filename} not found.")
    except ValueError as e:
        print(f"Error parsing {filename}: {e}")
    return node_spaces

# Function to read model configuration from the JSON file
def read_model_config(filename):
    try:
        with open(filename, 'r') as file:
            model_config = json.load(file)
    except FileNotFoundError:
        print(f"Error: {filename} not found.")
        return {}
    except json.JSONDecodeError as e:
        print(f"Error parsing {filename}: {e}")
        return {}
    return model_config

# Function to calculate the number of nodes needed based on fixed node sizes
def calculate_nodes_needed(file_size, node_spaces):
    num_nodes = 0
    remaining_size = file_size  # Use remaining_size to track leftover size

    for node, space in node_spaces.items():
        if remaining_size > space:
            num_nodes += 1
            remaining_size -= space  # Use -= to correctly subtract node space
        else:
            num_nodes += 1
            break

    return num_nodes

if __name__ == "__main__":
    # Read disk space from the file
    node_spaces = read_disk_space('disk_space.txt')

    # Read model configuration from the JSON file
    model_config = read_model_config('parameters.json')
    
    if not model_config:
        print("Error: Model configuration could not be read. Exiting.")
        exit(1)

    # Extract values from the model configuration
    num_layers = model_config.get('layers', 0)
    dimensions = model_config.get('layer_dimensions', [])
    file_size = model_config.get('file_size_bytes', 0)
    job_id = model_config.get('id', '')

    # Calculate the number of nodes needed
    num_nodes = calculate_nodes_needed(file_size, node_spaces)

    # Print the extracted values
    print(f"File size: {file_size}")
    print(f"Node sizes: {node_spaces}")
    print(f"Number of nodes required: {num_nodes}")

    # Print manually input dimensions
    print(f"Number of layers: {num_layers}")
    print("Dimensions: ", end="")
    for dim in dimensions:
        print(dim, end=" ")
    print()

    keys_file = os.path.join("/home/cie/Desktop/SCHEDULE/", f"{job_id}__/{job_id}_keys.txt")
    
    if not os.path.isfile(keys_file):
        print(f"Error: {keys_file} not found.")
        exit(1)

    try:
        with open(keys_file, "r") as K:
            filenames = [line.strip() for line in K.readlines()]
    except IOError as e:
        print(f"Error reading {keys_file}: {e}")
        exit(1)

    for filename in filenames:
        local_file = "/home/cie/Desktop/SCHEDULE/"+f"{job_id}__/"+f"{job_id}_"+filename + ".txt"
        if os.path.isfile(local_file):
            subprocess.run(["scp", local_file, f"{remote_user}@{remote_server}:{remote_path}"], check=True)
        else:
            print(f"Warning: Local file {local_file} does not exist.")
    
    # Copy the parameters.json file
    try:
        subprocess.run(["scp", "parameters.json", f"{remote_user}@{remote_server}:{remote_path}"], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error during scp command: {e}")
    try:
        subprocess.run(["scp", keys_file, f"{remote_user}@{remote_server}:{remote_path}"], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error during scp command: {e}")
    
    c_executable = "oneNode_"  # Replace with the actual C executable filename
    try:
        ssh_command = f"ssh {remote_user}@{remote_server} 'cd {remote_path} && ./{c_executable}'"
        subprocess.run(ssh_command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
     555   print(f"Error executing remote C code: {e}")
"""

import json
import os
import subprocess
import threading
import tarfile

password = "cie123" 
remote_user = "node_1"
remote_server = "10.3.31.152"
remote_path = "/home/node_1/DEEP_LEARNING/"

# Define the maximum number of nodes
MAX_NODES = 3

# Function to read disk space from the file
def read_disk_space(filename):
    node_spaces = {}
    try:
        with open(filename, 'r') as file:
            for line in file:
                node, space = line.strip().split(':')
                node_spaces[node] = int(space)
    except FileNotFoundError:
        print(f"Error: {filename} not found.")
    except ValueError as e:
        print(f"Error parsing {filename}: {e}")
    return node_spaces

# Function to read model configuration from the JSON file
def read_model_config(filename):
    try:
        with open(filename, 'r') as file:
            model_config = json.load(file)
    except FileNotFoundError:
        print(f"Error: {filename} not found.")
        return {}
    except json.JSONDecodeError as e:
        print(f"Error parsing {filename}: {e}")
        return {}
    return model_config

# Function to calculate the number of nodes needed based on fixed node sizes
def calculate_nodes_needed(file_size, node_spaces):
    num_nodes = 0
    remaining_size = file_size  # Use remaining_size to track leftover size

    for node, space in node_spaces.items():
        if remaining_size > space:
            num_nodes += 1
            remaining_size -= space  # Use -= to correctly subtract node space
        else:
            num_nodes += 1
            break

    return num_nodes

# Function to create a tar file for faster SCP transfer
def create_tar_file(filenames, tar_filename):
    with tarfile.open(tar_filename, "w:gz") as tar:
        for filename in filenames:
            tar.add(filename, arcname=os.path.basename(filename))

# Function to run SCP in parallel
def scp_transfer(local_file, remote_file):
    try:
        subprocess.run(["scp", local_file, remote_file], check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error during scp command for file {local_file}: {e}")

if __name__ == "__main__":
    # Read disk space from the file
    node_spaces = read_disk_space('disk_space.txt')

    # Read model configuration from the JSON file
    model_config = read_model_config('parameters.json')
    
    if not model_config:
        print("Error: Model configuration could not be read. Exiting.")
        exit(1)

    # Extract values from the model configuration
    num_layers = model_config.get('layers', 0)
    dimensions = model_config.get('layer_dimensions', [])
    file_size = model_config.get('Model_size', 0)
    job_id = model_config.get('id', '')

    # Calculate the number of nodes needed
    num_nodes = calculate_nodes_needed(file_size, node_spaces)

    # Print the extracted values
    print(f"File size: {file_size}")
    print(f"Node sizes: {node_spaces}")
    print(f"Number of nodes required: {num_nodes}")

    # Print manually input dimensions
    print(f"Number of layers: {num_layers}")
    print("Dimensions: ", end="")
    for dim in dimensions:
        print(dim, end=" ")
    print()

    keys_file = os.path.join("/home/cie/Desktop/SCHEDULE/", f"{job_id}__/{job_id}_keys.txt")
    
    if not os.path.isfile(keys_file):
        print(f"Error: {keys_file} not found.")
        exit(1)

    try:
        with open(keys_file, "r") as K:
            filenames = [line.strip() for line in K.readlines()]
    except IOError as e:
        print(f"Error reading {keys_file}: {e}")
        exit(1)

    local_files = ["/home/cie/Desktop/SCHEDULE/"+f"{job_id}__/"+f"{job_id}_"+filename + ".txt" for filename in filenames]
    local_files = [file for file in local_files if os.path.isfile(file)]

    tar_filename = f"{job_id}_files.tar.gz"
    create_tar_file(local_files + ['parameters.json', keys_file], tar_filename)

    # SCP the tar file
    scp_transfer(tar_filename, f"{remote_user}@{remote_server}:{remote_path}")

    # SSH into the remote server to extract the tar file
    try:
        ssh_command = f"ssh {remote_user}@{remote_server} 'cd {remote_path} && tar -xzf {tar_filename} && rm {tar_filename}'"
        subprocess.run(ssh_command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error executing remote SSH command: {e}")

    # Execute the C executable
    c_executable = "oneNode_"  # Replace with the actual C executable filename
    try:
        ssh_command = f"ssh {remote_user}@{remote_server} 'cd {remote_path} && ./{c_executable}'"
        subprocess.run(ssh_command, shell=True, check=True)
    except subprocess.CalledProcessError as e:
        print(f"Error executing remote C code: {e}")
    





