#include <vector>
#include <iostream>
#include <cassert>
#include <ctime>

using namespace std;

void Swap(pair<uint64_t, string> &first, pair<uint64_t, string> &second)
{
    pair<uint64_t, string> tmp = first;
    first = second;
    second = tmp;
}

void InsertionSort(vector<pair<uint64_t, string>> &array)
{
    for (int i = 1; i < array.size(); i++)
        for (int j = i; j > 0; --j)
            if (array[j - 1].first > array[j].first)
            {
                Swap(array[j - 1], array[j]);
            }
}

void BucketSort(vector<pair<uint64_t, string>> &array)
{
    vector<vector<pair<uint64_t, string>>> buckets(array.size());
    uint64_t max_elem = array[0].first;
    uint64_t min_elem = array[0].first;
    for (const auto &p : array)
    {
        if (max_elem < p.first)
            max_elem = p.first;
        if (min_elem > p.first)
            min_elem = p.first;
    }
    // cout << min_elem << " " << max_elem << "\n";
    const long double interval = (long double)(max_elem - min_elem + 1) / array.size();
    for (int i = 0; i < array.size(); ++i)
    {
        auto a = (uint64_t)((array[i].first - min_elem) / interval);
        if (a == array.size())
            a--;
        buckets[a].push_back(array[i]);
    }
    for (int i = 0; i < array.size(); ++i)
    {
        InsertionSort(buckets[i]);
    }
    int k = 0;
    for (int i = 0; i < buckets.size(); ++i)
    {
        for (int j = 0; j < buckets[i].size(); ++j)
        {
            array[k++] = buckets[i][j];
        }
    }
    assert(k == array.size());
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    vector<pair<uint64_t, string>> array;
    //    int N;
    //    cin >> N;
    uint64_t key;
    string value;
    uint64_t count = 0;
    while (cin >> key >> value)
    {
        ++count;
        array.emplace_back(make_pair(key, value));
    }
    unsigned int start_time = clock();
    if (count > 0)
    {
        BucketSort(array);
    }
    for (const auto &p : array)
    {
        std::cout << p.first << "\t" << p.second << endl;
    }
    unsigned int end_time = clock();
    unsigned int search_time = end_time - start_time;
    cout << search_time;

    return 0;
}