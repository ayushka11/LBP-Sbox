#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>

using namespace std;
#define n 8
#define SIZE 1 << n
int find_solution = 0;
int SBOX[SIZE] = {}, s[SIZE][SIZE], ddt[SIZE][SIZE];

void load_sbox_from_file(int sbox[SIZE], int index)
{
    char filename[128];
    snprintf(filename, sizeof(filename), "aes_sbox.txt");

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        cerr << "The S-box file cannot be opened." << filename << endl;
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < SIZE; i++)
    {
        if (fscanf(fp, "%d", &sbox[i]) != 1)
        {
            cerr << "The file format of the S-box is incorrect." << filename << endl;
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }
    fclose(fp);
}

int BuildDDT(int sbox[SIZE], int ddt[SIZE][SIZE])
{
    int x1, x2, dx, dy;
    for (x1 = 0; x1 < SIZE; x1++)
        for (x2 = 0; x2 < SIZE; x2++)
            ddt[x1][x2] = 0;

    for (x1 = 0; x1 < SIZE; x1++)
        for (dx = 0; dx < SIZE; dx++)
        {
            x2 = x1 ^ dx;
            if ((sbox[x1] == -1) || (sbox[x2] == -1))
                continue;
            dy = sbox[x1] ^ sbox[x2];
            ddt[dx][dy]++;
        }
    return 0;
}

void RecursifSearch(int S[SIZE], int i)
{
    if (i < SIZE)
    {
        bool found = false;

        for (int possAssign = 1; possAssign <= s[i][0]; possAssign++)
        {
            for (int j = 0; j < i; j++)
                if ((S[j] == s[i][possAssign]) || (ddt[i ^ j][s[i][possAssign] ^ S[j]] == 0))
                    continue;

            found = true;
            
            S[i] = s[i][possAssign];

            for (int j = 0; j < i; j++)
                ddt[i ^ j][S[i] ^ S[j]] -= 2;

            RecursifSearch(S, i + 1);

            if (find_solution == 1)
                return;
        }

        for (int j = 0; j < (i - 1); j++)
            ddt[(i - 1) ^ j][S[i - 1] ^ S[j]] += 2;

        if (!found)
            return;
    }
    else
    {
        find_solution = 1;
        return;
    }
}

int main()
{
    int ar[SIZE];
    int ctr = 0;
    int test = 10;
    double total_time = 0.0;

    for (int j = 0; j < 1000; j++)
        for (int ii = 0; ii < test; ii++)
        {
            load_sbox_from_file(SBOX, ii);
            BuildDDT(SBOX, ddt);
            find_solution = 0;

            s[0][0] = 1;
            s[0][1] = 0;
            ctr = 0;
            for (int i = 1; i < SIZE; i++)
            {

                for (int j1 = 0; j1 < SIZE; j1++)
                    if (ddt[i][j1])
                        s[i][++ctr] = j1;

                s[i][0] = ctr;
                ctr = 0;
            }

            auto start = chrono::high_resolution_clock::now();

            ar[0] = 0;
            for (int i = 0; i < SIZE; i++)
                if (ddt[1][i])
                {
                    ar[1] = i;
                    break;
                }

            RecursifSearch(ar, 2);

            auto end = chrono::high_resolution_clock::now();
            chrono::duration<double> elapsed = end - start;
            total_time += elapsed.count();
        }
    
    double average_time = total_time / (test);
    cout << "Average time: " << average_time / 1000 << " seconds" << endl;
    return 0;
}
