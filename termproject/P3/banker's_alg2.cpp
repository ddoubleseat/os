// Banker's Algorithm
#include <iostream>
#include <vector>
using namespace std;

const int P = 5; // Process 개수

const int R = 4; // Resource type 개수

// Need 계산 함수
void calculateNeed(vector<vector<int>>& need, const vector<vector<int>>& max,
                   const vector<vector<int>>& alloc)
{
    for (int i = 0 ; i < P ; i++) {
        for (int j = 0; j < R; j++)
            need[i][j] = max[i][j] - alloc[i][j];
    }
}

// Request 계산 함수
bool request(int process, vector<int> req,  vector<int>& avail,
             vector<vector<int>>& alloc, vector<vector<int>>& need){
    for(int i = 0; i < R; i++){
        avail[i] -= req[i];
        if(avail[i] < 0)
            return false;
        alloc[process][i] += req[i];
        need[process][i] -= req[i];
    }
    return true;
}

// Safe? or not safe?
bool isSafe(vector<int> processes, vector<int> avail, vector<vector<int>> max,
            vector<vector<int>> alloc, vector<vector<int>>& need)
{
    calculateNeed(need, max, alloc);

    bool finish[P] = {0};

    // safe sequence 저장
    int safeSeq[P];

    int work[R];
    for (int i = 0; i < R ; i++)
        work[i] = avail[i];

    int count = 0;
    while (count < P){
        bool found = false;
        for (int p = 0; p < P; p++){
            if (finish[p] == 0){
                int j;
                for (j = 0; j < R; j++)
                    if (need[p][j] > work[j])
                        break;
                if (j == R){
                    for (int k = 0 ; k < R ; k++)
                        work[k] += alloc[p][k];
                    safeSeq[count++] = p;
                    finish[p] = 1;
                    found = true;
                }
            }
        }

        if (found == false){
            cout << "System is not in safe state";
            return false;
        }
    }

    cout << "System is in safe state." << endl;
    cout << "Safe sequence is: ";
    for (int i = 0; i < P ; i++)
        cout << safeSeq[i] << " ";

    return true;
}


int main()
{
    vector<int> processes = {0, 1, 2, 3, 4};

    vector<int> avail = {3, 2, 1, 1};

    vector<vector<int>> max = {{6, 0, 1, 2},
                               {1, 7, 5, 0},
                               {2, 3, 5, 6},
                               {1, 6, 5, 3},
                               {1, 6, 5, 6}};

    vector<vector<int>> alloc = {{4, 0, 0, 1},
                                 {1, 1, 0, 0},
                                 {1, 2, 5, 4},
                                 {0, 6, 3, 3},
                                 {0, 2, 1, 3}};

    vector<vector<int>> need(P, vector<int> (R, 0));

    cout << "1. Is the System is safe state?" << endl;
    isSafe(processes, avail, max, alloc, need);
    cout << endl << endl;

    cout << "2. Can request for (1, 2, 0, 0) by P4 be granted?" << endl;
    int process = 4; // request process
    vector<int> req = {1, 2, 0, 0}; // request
    bool result = request(process-1, req, avail, alloc, need);
    if(result == true){
        cout << "Request <= Available" << endl;
        isSafe(processes, avail, max, alloc, need);
    }
    else{
        cout << "Request > Available" << endl;
    }

    return 0;
}


