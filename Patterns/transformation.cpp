#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define n 5
#define SIZE (1 << n)

int SBOX[SIZE];
int ddt[SIZE][SIZE];

void load_sbox_from_file(int sbox[SIZE], int index)
{
    char filename[128];
    snprintf(filename, sizeof(filename), "sbox_%03d.txt", index + 1);

    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        cerr << "The S-box file cannot be opened: " << filename << endl;
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < SIZE; i++)
    {
        if (fscanf(fp, "%d", &sbox[i]) != 1)
        {
            cerr << "The file format of the S-box is incorrect: " << filename << endl;
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }
    fclose(fp);
}

int BuildDDT(int sbox[SIZE], int ddt[SIZE][SIZE])
{
    for (int x1 = 0; x1 < SIZE; x1++)
    {
        for (int x2 = 0; x2 < SIZE; x2++)
        {
            ddt[x1][x2] = 0;
        }
    }

    for (int x1 = 0; x1 < SIZE; x1++)
    {
        for (int dx = 0; dx < SIZE; dx++)
        {
            int x2 = x1 ^ dx;
            if ((sbox[x1] == -1) || (sbox[x2] == -1))
                continue;
            int dy = sbox[x1] ^ sbox[x2];
            ddt[dx][dy]++;
        }
    }
    return 0;
}

int main()
{
    FILE *fp = fopen("5bit_apn_sbox.txt", "r");
    if (!fp)
    {
        cerr << "Cannot open 5bit_apn_sbox.txt" << endl;
        return 1;
    }
    for (int i = 0; i < SIZE; i++)
    {
        if (fscanf(fp, "%d", &SBOX[i]) != 1)
        {
            cerr << "Error reading sbox value at index " << i << endl;
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);

    BuildDDT(SBOX, ddt);

    FILE *out = fopen("5bit_apn_sbox.ddt", "w");
    if (!out)
    {
        cerr << "Cannot open 5bit_apn_sbox.ddt for writing" << endl;
        return 1;
    }

    fprintf(out, "     ");
    for (int dy = 0; dy < SIZE; dy++)
    {
        fprintf(out, "%3d", dy);
    }
    fprintf(out, "\n");

    for (int dx = 0; dx < SIZE; dx++)
    {
        fprintf(out, "%3d: ", dx);
        for (int dy = 0; dy < SIZE; dy++)
        {
            fprintf(out, "%3d", ddt[dx][dy]);
        }
        fprintf(out, "\n");
    }

    fclose(out);
    cout << "DDT written to 5bit_apn_sbox.ddt" << endl;

    int t[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            t[i][j] = (ddt[i][j] >= 2) ? 1 : 0;
        }
    }
    int tr[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            tr[i][j] = t[j][i];
        }
    }

    int pat[SIZE / 2][SIZE];
    for (int i = 0; i < SIZE / 2; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            pat[i][j] = tr[i][j] ^ tr[i + SIZE / 2][j];
        }
    }

    FILE *pout = fopen("5bit_apn_sbox.pattern", "w");
    if (!pout)
    {
        cerr << "Cannot open 5bit_apn_sbox.pattern for writing" << endl;
        return 1;
    }

    fprintf(pout, "      ");
    for (int j = 0; j < SIZE; j++)
    {
        fprintf(pout, "%3d", j);
    }
    fprintf(pout, "\n");

    for (int i = 0; i < SIZE / 2; i++)
    {
        fprintf(pout, "%3d: ", i);
        for (int j = 0; j < SIZE; j++)
        {
            fprintf(pout, "  %c", pat[i][j] ? '1' : '.');
        }
        fprintf(pout, "\n");
    }
    fclose(pout);
    cout << "Pattern written to 5bit_apn_sbox.pattern" << endl;

    return 0;
}