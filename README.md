# LBP-Sbox

## Things done

- Did basic optimisations that cut down the time slightly in Minor_Optimizations.cpp. As a part of cleaning and refactoring the code, I realized that redundant branches were present in the `RecursifSearch` function, which were promptly discarded. Time improvement (measured for `n = 8`): ~1.5 seconds vs ~1.3 seconds.

- Thoroughly analyzed the improvement in the algorithm in the DTOS paper, and added a writeup in the Code Review folder.

- Implemented the Hamming Weight-1 approach, in which we constrain the S-box search by pre-fixing all weight-1 inputs to map to themselves (i.e., `S[2^i] = 2^i`), and only searching over the remaining positions. Before searching, we verify Condition B upfront — that `DDT[2^i ⊕ 2^j][2^i ⊕ 2^j] ≥ 2` for all pairs i ≠ j — since this depends only on the fixed assignments. Condition A — that `DDT[2^i][S(0) ⊕ 2^i] ≥ 2` for each HW-1 difference — is checked dynamically during the search as each candidate value for S[0] is tried, pruning branches early where the constraint fails.
