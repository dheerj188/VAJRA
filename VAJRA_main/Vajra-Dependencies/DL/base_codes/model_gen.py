"""import os
import torch
import json
import numpy as np
import subprocess

# Load the JSON file
with open('/home/cie/Desktop/SCHEDULER/parameters.json','r+') as f:
    config = json.load(f)

# Extract data
num_layers = config['layers']
job_id = config['id']

# Load the state dictionary
state_dict = torch.load(f'/home/cie/Desktop/SCHEDULER/{job_id}.pth')

# Create directory if it doesn't exist
directory = f"/home/cie/Desktop/SCHEDULE/{job_id}__"
if not os.path.exists(directory):
    os.makedirs(directory)

# Save keys
keys = list(state_dict.keys())
np.savetxt(f"/home/cie/Desktop/SCHEDULE/{job_id}__/{job_id}_keys.txt", keys, fmt="%s")

# Calculate file size and save values
file_size_bytes = 0
for key in state_dict.keys():
    values = state_dict[key].numpy()
    np.savetxt(f"/home/cie/Desktop/SCHEDULE/{job_id}__/{job_id}_{key}.txt", values)
    file_size_bytes += os.path.getsize(f"/home/cie/Desktop/SCHEDULE/{job_id}__/{job_id}_{key}.txt")

# Update JSON with model size
config['Model_size'] = file_size_bytes
with open('/home/cie/Desktop/SCHEDULE/parameters.json', 'w') as f:
    json.dump(config, f, indent=4)
    """

# Run Analyzer.py

"""analyzer_path = '/home/cie/Desktop/SCHEDULE/Analyzer.py'
subprocess.run(['python3', analyzer_path]) """


"""
import os
import torch
import json
import numpy as np
import subprocess
import pickle
# Load the JSON file
with open('/home/cie/Desktop/SCHEDULER/parameters.json', 'r+') as f:
    config = json.load(f)

# Extract data
num_layers = config['layers']
job_id = config['id']

# Load the state dictionary
state_dict = torch.load(f'/home/cie/Desktop/SCHEDULER/{job_id}.pth', weights_only=True)

# Create directory if it doesn't exist
directory = f"/home/cie/Desktop/SCHEDULE/{job_id}__"
if not os.path.exists(directory):
    os.makedirs(directory)

# Save keys
keys = list(state_dict.keys())
np.savetxt(f"/home/cie/Desktop/SCHEDULE/{job_id}__/{job_id}_keys.txt", np.array(keys), fmt="%s")

# Calculate file size and save values
file_size_bytes = 0
for key in state_dict.keys():
    if state_dict[key].numel() > 0:  # Check if the tensor has elements
        values = state_dict[key].cpu().numpy()  # Move tensor to CPU and convert to numpy
        print(values)
        np.savetxt(f"/home/cie/Desktop/SCHEDULE/{job_id}__/{job_id}_{key}.txt", values)
        file_size_bytes += os.path.getsize(f"/home/cie/Desktop/SCHEDULE/{job_id}__/{job_id}_{key}.txt")
    else:
        print(f"Warning: {key} has no values to save.")

# Update JSON with model size
config['Model_size'] = file_size_bytes
with open('/home/cie/Desktop/SCHEDULE/parameters.json', 'w') as f:
    json.dump(config, f, indent=4)

# Run Analyzer.py (if needed)
# analyzer_path = '/home/cie/Desktop/SCHEDULE/Analyzer.py'
# subprocess.run(['python3', analyzer_path])"""


import os
import torch
import json
import numpy as np

# Load the JSON file
with open('/home/cie/Desktop/SCHEDULER/parameters.json', 'r+') as f:
    config = json.load(f)

# Extract data
num_layers = config['layers']
job_id = config['id']

# Load the state dictionary
state_dict = torch.load(f'/home/cie/Desktop/SCHEDULER/{job_id}.pth', weights_only=True)

# Create directory if it doesn't exist
directory = f"/home/cie/Desktop/SCHEDULE/{job_id}__"
if not os.path.exists(directory):
    os.makedirs(directory)

# Save keys
keys = list(state_dict.keys())
np.savetxt(f"/home/cie/Desktop/SCHEDULE/{job_id}__/{job_id}_keys.txt", np.array(keys), fmt="%s")

# Calculate file size and save values
file_size_bytes = 0
for key in state_dict.keys():
    # Check if the entry is a tensor
    if isinstance(state_dict[key], torch.Tensor):
        if state_dict[key].numel() > 0:  # Check if the tensor has elements
            values = state_dict[key].cpu().numpy()  # Move tensor to CPU and convert to numpy
            print(values)

            # Handle scalar values by converting to 1D array
            if values.ndim == 0:  # Scalar
                values = np.array([values])  # Convert to 1D array

            np.savetxt(f"/home/cie/Desktop/SCHEDULE/{job_id}__/{job_id}_{key}.txt", values)

            # Update the file size
            file_size_bytes += os.path.getsize(f"/home/cie/Desktop/SCHEDULE/{job_id}__/{job_id}_{key}.txt")
        else:
            print(f"Warning: {key} has no values to save.")
    else:
        print(f"Warning: {key} is not a tensor, skipping.")

# Update JSON with model size
config['Model_size'] = file_size_bytes
with open('/home/cie/Desktop/SCHEDULE/parameters.json', 'w') as f:
    json.dump(config, f, indent=4)
