#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>

using namespace std;
#define n 4
#define SIZE (1<<n)

int find_solution = 0;
int SBOX[SIZE] = {}, s[SIZE][SIZE], ddt[SIZE][SIZE], L[SIZE][SIZE] = {0};

bool fixed_pos[SIZE] = {false};

void load_sbox_from_file(int sbox[SIZE], int index) {
    char filename[128];
    snprintf(filename, sizeof(filename), "sbox_%03d.txt", index + 1);

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        cerr << "The S-box file cannot be opened: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < SIZE; i++) {
        if (fscanf(fp, "%d", &sbox[i]) != 1) {
            cerr << "The file format of the S-box is incorrect: " << filename << endl;
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }
    fclose(fp);
}

int BuildDDT(int sbox[SIZE], int ddt[SIZE][SIZE]) {
    for (int x1 = 0; x1 < SIZE; x1++)
        for (int x2 = 0; x2 < SIZE; x2++)
            ddt[x1][x2] = 0;

    for (int x1 = 0; x1 < SIZE; x1++)
        for (int dx = 0; dx < SIZE; dx++) {
            int x2 = x1 ^ dx;
            if ((sbox[x1] == -1) || (sbox[x2] == -1))
                continue;
            int dy = sbox[x1] ^ sbox[x2];
            ddt[dx][dy]++;
        }
    return 0;
}

bool CheckConstraints(int ddt[SIZE][SIZE]) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int d = (1 << i) ^ (1 << j);
            if (ddt[d][d] < 2)
                return false;
        }
    }
    return true;
}

bool CheckConditionA(int ddt[SIZE][SIZE], int s0) {
    for (int i = 0; i < n; i++) {
        int d = 1 << i;
        if (ddt[d][s0 ^ d] < 2)
            return false;
    }
    return true;
}

void RecursifSearch(int S[SIZE], int i) {
    while (i < SIZE && fixed_pos[i])
        i++;

    if (i < SIZE) {
        L[i][0] = 0;

        for (int possAssign = 1; possAssign <= s[i][0]; possAssign++) {
            if (i == 0 && !CheckConditionA(ddt, s[i][possAssign]))
                goto L1;

            for (int j = 0; j < i; j++)
                if ((S[j] == s[i][possAssign]) || (ddt[i^j][s[i][possAssign] ^ S[j]] == 0))
                    goto L1;

            L[i][0]++;
            L[i][L[i][0]] = s[i][possAssign];
            L1:;
        }
    } else {
        find_solution = 1;
        return;
    }

    if (L[i][0]) {
        for (int x = 1; x <= L[i][0]; x++) {
            S[i] = L[i][x];

            for (int j = 0; j < i; j++)
                ddt[i^j][S[i]^S[j]] -= 2;

            RecursifSearch(S, i + 1);

            if (find_solution == 1)
                return;

            if (x == L[i][0])
                for (int j = 0; j < (i - 1); j++)
                    ddt[(i-1)^j][S[i-1]^S[j]] += 2;
        }
    } else {
        for (int j = 0; j < i - 1; j++)
            ddt[(i-1)^j][S[i-1]^S[j]] += 2;
        return;
    }
}

int main() {
    int ar[SIZE];
    int ctr = 0;
    int test = 1;
    double total_time = 0.0;

    for (int j = 0; j < 1000; j++)
    for (int ii = 0; ii < test; ii++) {
        load_sbox_from_file(SBOX, ii);
        BuildDDT(SBOX, ddt);
        find_solution = 0;

        for (int i = 0; i < SIZE; i++)
            fixed_pos[i] = false;

        if (!CheckConstraints(ddt)) {
            if (j == 0)
                cout << "No solution found (DDT constraints A/B not satisfied)." << endl;
            continue;
        }

        for (int i = 0; i < n; i++) {
            int idx = 1 << i;
            ar[idx] = idx;
            fixed_pos[idx] = true;
        }

        ctr = 0;
        for (int i = 0; i < SIZE; i++) {
            if (fixed_pos[i]) continue;
            for (int j1 = 0; j1 < SIZE; j1++)
                if (ddt[i][j1])
                    s[i][++ctr] = j1;
            s[i][0] = ctr;
            ctr = 0;
        }

        BuildDDT(SBOX, ddt);
        for (int fi = 0; fi < n; fi++)
            for (int fj = fi + 1; fj < n; fj++) {
                int idx_i = 1 << fi;
                int idx_j = 1 << fj;
                ddt[idx_i ^ idx_j][ar[idx_i] ^ ar[idx_j]] -= 2;
            }

        auto start = chrono::high_resolution_clock::now();

        RecursifSearch(ar, 0);

        auto end = chrono::high_resolution_clock::now();
        total_time += chrono::duration<double>(end - start).count();

        if (j == 0) {
            if (find_solution) {
                cout << "Solution S-box: [";
                for (int k = 0; k < SIZE; k++)
                    cout << ar[k] << (k < SIZE - 1 ? ", " : "");
                cout << "]" << endl;
            } else {
                cout << "No solution found." << endl;
            }
        }
    }

    double average_time = total_time / test;
    cout << "Average time: " << average_time / 1000 << " seconds" << endl;
    return 0;
}

