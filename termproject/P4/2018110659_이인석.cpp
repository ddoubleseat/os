#include <iostream>>
#include <bits/stdc++.h>
#include <vector>
using namespace std;

// LRU Algorithm
int lruPage(int n, int c, int pages[])
{
    int count = 0;

    // 메모리에 있는것을 저장하기 위한 벡터
    vector<int> v;
    int i;
    for (i = 0; i <= n - 1; i++) {
        // 현재 메모리에 있는지 확인
        auto it = find(v.begin(), v.end(), pages[i]);
        if (it == v.end()) {
            if (v.size() == c) {
                // 첫번째 데이터 제거
                v.erase(v.begin());
            }
            //가장 최근 데이터 추가
            v.push_back(pages[i]);

            count++;
        }
        else {
            // 현재 데이터 제거후 가장 최근 데이터로 교체
            v.erase(it);
            v.push_back(pages[i]);
        }
    }

    return count;
}


// Optimal Algorithm
bool search(int key, vector<int>& fr)
{
    for (int i = 0; i < fr.size(); i++)
        if (fr[i] == key)
            return true;
    return false;
}

int predict(int pg[], vector<int>& fr, int pn, int index)
{

    int res = -1, farthest = index;
    for (int i = 0; i < fr.size(); i++) {
        int j;
        for (j = index; j < pn; j++) {
            if (fr[i] == pg[j]) {
                if (j > farthest) {
                    farthest = j;
                    res = i;
                }
                break;
            }
        }
        if (j == pn)
            return i;
    }
    return (res == -1) ? 0 : res;
}

int optimalPage(int pg[], int pn, int fn)
{
    vector<int> fr;

    int hit = 0;
    for (int i = 0; i < pn; i++) {
        if (search(pg[i], fr)) {
            hit++;
            continue;
        }
        if (fr.size() < fn)
            fr.push_back(pg[i]);
        else {
            int j = predict(pg, fr, pn, i + 1);
            fr[j] = pg[i];
        }
    }
    return hit;
}


int main()
{
    int pages[] = {1, 2, 1, 4, 5, 6, 3, 4, 6, 3, 7, 3, 1, 5, 3, 7, 3, 4, 2, 4, 1, 4, 5, 1};
    int n = 24, c = 4;

    cout << "LRU Page Faults = " << lruPage(n, c, pages) << endl;
    cout << "Optimal Page Faults = " << optimalPage(pages, n, c) << endl;

    return 0;
}