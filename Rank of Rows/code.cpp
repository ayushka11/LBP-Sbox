#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#define n 6
#define SIZE (1 << n)
#define LIMIT 100

using Sbox = array<int, SIZE>;
using Matrix = array<array<int, SIZE>, SIZE>;

void BuildDDT(const Sbox &sbox, Matrix &ddt)
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
            int dy = sbox[x1] ^ sbox[x2];
            ddt[dx][dy]++;
        }
    }
}

vector<uint64_t> BuildTransformedRows(const Matrix &ddt)
{
    array<array<int, SIZE>, SIZE> t{};
    array<array<int, SIZE>, SIZE> tr{};

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            t[i][j] = (ddt[i][j] >= 2) ? 1 : 0;
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            tr[i][j] = t[j][i];
        }
    }

    vector<uint64_t> rows;
    rows.reserve(SIZE / 2);
    for (int i = 0; i < SIZE / 2; i++)
    {
        uint64_t rowMask = 0;
        for (int j = 0; j < SIZE; j++)
        {
            int bit = tr[i][j] ^ tr[i + SIZE / 2][j];
            if (bit)
            {
                rowMask |= (uint64_t(1) << j);
            }
        }
        rows.push_back(rowMask);
    }

    return rows;
}

int RankGF2(const vector<uint64_t> &rows)
{
    vector<uint64_t> basis(64, 0);
    int rank = 0;

    for (uint64_t row : rows)
    {
        uint64_t x = row;
        for (int bit = 63; bit >= 0 && x != 0; --bit)
        {
            if (((x >> bit) & 1ULL) == 0)
            {
                continue;
            }

            if (basis[bit] == 0)
            {
                basis[bit] = x;
                rank++;
                break;
            }

            x ^= basis[bit];
        }
    }

    return rank;
}

vector<int> FindPairXorRedundantRows(const vector<uint64_t> &rows)
{
    int m = static_cast<int>(rows.size());
    vector<bool> redundant(m, false);

    for (int k = 0; k < m; ++k)
    {
        bool found = false;
        for (int i = 0; i < m && !found; ++i)
        {
            if (i == k)
                continue;
            for (int j = i + 1; j < m; ++j)
            {
                if (j == k)
                    continue;
                if ((rows[i] ^ rows[j]) == rows[k])
                {
                    redundant[k] = true;
                    found = true;
                    break;
                }
            }
        }
    }

    vector<int> indices;
    for (int i = 0; i < m; ++i)
    {
        if (redundant[i])
        {
            indices.push_back(i);
        }
    }
    return indices;
}

bool ParseSboxLine(const string &line, Sbox &sbox)
{
    istringstream iss(line);
    for (int i = 0; i < SIZE; ++i)
    {
        if (!(iss >> sbox[i]))
        {
            return false;
        }
    }
    return true;
}

int main()
{
    ifstream in("apn6.log");
    if (!in)
    {
        cerr << "Cannot open apn6.log" << endl;
        return 1;
    }

    ofstream out("apn6_row_rank.txt");
    if (!out)
    {
        cerr << "Cannot open apn6_row_rank.txt for writing" << endl;
        return 1;
    }

    string line;
    int processed = 0;
    while (processed < LIMIT && getline(in, line))
    {
        if (line.empty())
        {
            continue;
        }

        Sbox sbox{};
        if (!ParseSboxLine(line, sbox))
        {
            cerr << "Skipping malformed line " << (processed + 1) << endl;
            continue;
        }

        Matrix ddt{};
        BuildDDT(sbox, ddt);

        vector<uint64_t> rows = BuildTransformedRows(ddt);
        int rank = RankGF2(rows);
        vector<int> redundant = FindPairXorRedundantRows(rows);

        out << "S-box " << (processed + 1) << ": rank=" << rank
            << ", redundant_rows=" << redundant.size();
        if (!redundant.empty())
        {
            out << " [";
            for (size_t i = 0; i < redundant.size(); ++i)
            {
                if (i)
                    out << ",";
                out << redundant[i];
            }
            out << "]";
        }
        out << '\n';

        processed++;
    }

    cout << "Processed " << processed << " S-boxes from apn6.log" << endl;
    cout << "Row-rank summary written to apn6_row_rank.txt" << endl;

    return 0;
}