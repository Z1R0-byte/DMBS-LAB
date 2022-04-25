#include <bits/stdc++.h>
using namespace std;

void merge(vector<vector<int>> &runs, vector<int> &temp)
{
    vector<int> pointer;
    int num = 0;
    for (int i = 0; i < runs.size(); i++)
    {
        pointer.push_back(0);
        num += runs[i].size();
    }

    for (int i = 0; i < num; i++)
    {
        int min = INT_MAX;
        int k = 0;
        for (int j = 0; j < pointer.size(); j++)
        {
            if (pointer[j] < runs[j].size() && runs[j][pointer[j]] < min)
            {
                min = runs[j][pointer[j]];
                k = j;
            }
        }
        temp.push_back(min);
        pointer[k]++;
    }
}

void external_sort(vector<vector<int>> &runs, int B)
{
    // Intially sorting each page
    for (int i = 0; i < runs.size(); i++)
    {
        sort(runs[i].begin(), runs[i].end());
    }

    while (runs.size() > 1)
    {
        vector<vector<int>> newruns;
        int k = 0;
        int i = 0;
        while (i < runs.size())
        {
            vector<vector<int>> temp;
            for (k = 0; k < B - 1 && i + k < runs.size(); k++)
            {
                temp.push_back(runs[i + k]);
            }
            vector<int> temp1;
            // Merging B-1 runs
            merge(temp, temp1);
            newruns.push_back(temp1);
            i += k;
        }
        runs = newruns;
    }
}

int main()
{
    int B, rec_page, records;
    cin >> B >> rec_page >> records;
    vector<vector<int>> runs;
    int k = 0;
    int i = 0;
    while (i < records)
    {
        vector<int> v;
        for (k = 0; k < rec_page && i + k < records; k++)
        {
            int a;
            cin >> a;
            v.push_back(a);
        }
        runs.push_back(v);
        i += k;
    }

    external_sort(runs, B);
    for (int i = 0; i < runs[0].size(); i++)
    {
        cout << runs[0][i] << endl;
    }
    return 0;
}
