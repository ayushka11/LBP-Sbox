# LBP-Sbox

## Things done

- Did basic optimisations that cut down the time slightly in Minor_Optimizations.cpp. As a part of cleaning and refactoring the code, I realized that redundant branches were present in the `RecursifSearch` function, which were promptly discarded. Time improvement (measured for `n = 8`): ~1.5 seconds vs ~1.3 seconds.

- Thoroughly analyzed the improvement in the algorithm in the DTOS paper, and added a writeup in the Code Review folder.