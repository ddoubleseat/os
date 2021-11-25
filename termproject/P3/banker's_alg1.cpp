// Banker's Algorithm
#include <iostream>
#include <vector>
using namespace std;

int main(){
    // 5개의 프로세스 P0, P1, P2, P3, P4
    int n = 5; // 프로세스의 갯수
    int m = 4; // 리소스 타입의 갯수
    int f[n], ans[n], ind = 0;
    vector<vector <int>>max = {{6, 0, 1, 2}, // P0의 Max vector
                       {1, 7, 5, 0}, // P1
                       {2, 3, 5, 6}, // P2
                       {1, 6, 5, 3}, // P3
                       {1, 6, 5, 6}}; // P4
    vector<vector <int>>alloc = {{4, 0, 0, 1}, // P0의 Allocation vector
                                 {1, 1, 0, 0}, // P1
                                 {1, 2, 5, 4}, // P2
                                 {0, 6, 3, 3}, // P3
                                 {0, 2, 1, 3}}; // P4
    vector<int> avail = {3, 2, 1, 1}; // Available
    vector<vector<int>> need(5, vector<int> (4, 0)); // Need vector
    for (int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
    for(int i = 0; i < n; i++){
        f[i] = 0;
        ans[i] = 0;
    }
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < n; j++){
            if(f[j] == 0){
                bool flag = true;
                for(int k = 0; k < m; k++){
                    if(need[j][k] > avail[k]){
                        flag = false;
                        break;
                    }
                }
                if(flag = true){
                    ans[ind] = j;
                    ind++;
                    for(int p = 0; p < m; p++){
                        avail[p] += alloc[j][p];
                    }
                    f[j] = 1;
                }
            }
        }
    }

    cout << "--------------------Safe Sequence--------------------" << endl;
    for(int i = 0; i < n-1; i++){
        cout << "P" << ans[i] << " ->";
    }
    cout << "P" << ans[n-1] << endl;

    return 0;

}





/*
Safety Algorithm
n 은 process의 갯수
m 은 resource type의 갯수
Available:
1차원 배열
Available[j] = k <-> j번째 리소스 타입의 instance 갯수
Max:
'n*m'사이즈의 2차원 배열
Max[i,j] = k <-> i번째 프로세스의 j번째 리소스 타입의 최대 instance 갯수는 k
Allocation:
'n*m' 사이즈의 2차원 배열
Allocation[i,j] = k <-> i번째 프로세스의 j번째 리소스 타입의 현재 할당된
instance 갯수
Need:
'n*m'사이즈의 2차원 배열
Need[i,j] = k <-> i번째 프로세스의 j번째 리소스 타입의 현재 필요한 instance갯수
Need[i,j] = Max[i,j] - Allocation[i,j]


Request Algorithm
Request array 생성 후 Requesti[j] = k <-> i번째 프로세스는 j번째 리소스 타입의
 k개의 인스턴스를 필요로 한다.
*/