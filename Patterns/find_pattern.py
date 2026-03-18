import numpy as np
import os

script_dir = os.path.dirname(os.path.abspath(__file__))

rows = {}
pattern_file_path = os.path.join(script_dir, "6bit_apn_sbox1.pattern")
with open(pattern_file_path) as f:
    for line in f:
        line = line.strip()
        if ':' not in line:
            continue
        idx, rest = line.split(':', 1)
        idx = int(idx.strip())
        vals = [1 if c == '1' else 0 for c in rest.split()]
        rows[idx] = vals

mat = np.array([rows[i] for i in range(32)])
print("Matrix shape:", mat.shape)
print()

print("Identical Rows: ")
for i in range(32):
    for j in range(i+1, 32):
        if np.array_equal(mat[i], mat[j]):
            print(f"  Row {i} == Row {j}")

print("\nComplementary Rows: ")
for i in range(32):
    for j in range(i+1, 32):
        if np.array_equal(mat[i], 1 - mat[j]):
            print(f"  Row {i} is complement of Row {j}")

print("\nCyclic Shift Relations between Rows: ")
for i in range(32):
    for j in range(i+1, 32):
        for shift in range(1, 32):
            if np.array_equal(np.roll(mat[i], shift), mat[j]):
                print(f"  Row {j} = Row {i} cyclically shifted by {shift}")
                break

print("\nColumn Blocks (each row split into 4 blocks of 8): ")
for i in range(32):
    blocks = [tuple(mat[i][k*8:(k+1)*8]) for k in range(4)]
    if blocks[0] == blocks[1] == blocks[2] == blocks[3]:
        print(f"  Row {i}: all 4 blocks identical")
    elif blocks[0] == blocks[2] and blocks[1] == blocks[3]:
        print(f"  Row {i}: blocks repeat with period 2 (AB AB): {blocks[0]} | {blocks[1]}")
    elif len(set(blocks)) < 4:
        print(f"  Row {i}: some blocks repeat: {blocks}")

print("\nRow XOR Relations: ")
for i in range(32):
    for j in range(32):
        if i == j: continue
        for k in range(j+1, 32):
            if k == i: continue
            if np.array_equal(mat[i], mat[j] ^ mat[k]):
                print(f"  Row {i} = Row {j} XOR Row {k}")