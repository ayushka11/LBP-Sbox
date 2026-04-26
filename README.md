# LBP-Sbox

## Things done

- Did basic optimisations that cut down the time slightly in Minor_Optimizations.cpp. As a part of cleaning and refactoring the code, I realized that redundant branches were present in the `RecursifSearch` function, which were promptly discarded. Time improvement (measured for `n = 8`): ~1.5 seconds vs ~1.3 seconds.

- Thoroughly analyzed the improvement in the algorithm in the DTOS paper, and added a writeup in the Code Review folder.

- Implemented the Hamming Weight-1 approach, in which we constrain the S-box search by pre-fixing all weight-1 inputs to map to themselves (i.e., `S[2^i] = 2^i`), and only searching over the remaining positions. Added a small writeup for the same.

- Applied transformation (given in `/Patterns/Readme.md`) to 6-bit and 5-bit APN S-boxes and ran a script (`find_pattern.py`) to find patterns in resulting transformed matrix.Observations are mentioned in `/Patterns/Readme.md`.

- Applied the same transformation (that was previously done to check patterns) to all APN 6-bit S-boxes and calculated the rank of the transformed matrix. Found that except one S-box, each matrix had rank 7 or 12. `[0 63 35 46 47 43 37 25 24 18 21 57 6 13 22 17 1 39 10 19 42 61 31 23 8 62 51 14 44 49 29 11 2 45 20 48 12 36 4 38 5 26 60 28 52 54 7 27 3 16 59 15 33 53 58 34 32 50 9 40 55 56 41 30]` has its rank as 32, which is the maximum possible, i.e. all its rows are linearly independent.

- Used SageMath to calculate Differential Uniformity, Algebraic Degree, Linearity, Non-Linearity and Boomerang Uniformity of 100 APN 6-bit S-boxes in `/SageMath/apn6_properties.txt`

- Used matrix_plot from SageMath to get Pollock's Representation of standard S-Boxes: `Skipjack_F, Zorro, SAFER+, AES, CLEFIA_S0, Whirlpool, Kuznyechik, and Camellia` to observe non random patterns in DDTs. (All above S-boxes were taken from SageMath.)
