import numpy as np
import os

script_dir = os.path.dirname(os.path.abspath(__file__))

rows = {}
with open(os.path.join(script_dir, "5bit_apn_sbox.pattern")) as f:
    for line in f:
        line = line.strip()
        if ':' not in line:
            continue
        idx, rest = line.split(':', 1)
        idx = int(idx.strip())
        vals = [1 if c == '1' else 0 for c in rest.split()]
        rows[idx] = vals

mat = np.array([rows[i] for i in range(16)])
print("Matrix shape:", mat.shape)
print()

for i in range(16):
    for j in range(16):
        if i == j: continue
        for k in range(j + 1, 16):
            if k == i: continue
            if np.array_equal(mat[i], mat[j] ^ mat[k]):
                print(f"  Row {i} = Row {j} XOR Row {k}")