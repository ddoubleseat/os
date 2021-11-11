#include <iostream>
#include <vector>

using namespace std;

class Process{
public:
    Process* next; // Pointer
    int pid;
    int arrival_t; // 도착 시간
    int burst_t; // cpu 실행 시간
    Process(){
        pid = 0;
        arrival_t = 0;
        burst_t = 0;
        next = NULL;
    }
    ~Process(){}
};

class ReadyQueue{ // 들어온 순서대로 실행할 프로세스를 담는 readyqueue
private:
    int size;
    Process* cur; // 현재 위치
    Process* head;
    Process* tail;
public:
    ReadyQueue(){
        this->size = 0;
        cur = head = tail = NULL;
    }
    ~ReadyQueue(){
        if(this->IsEmpty()==true){
            Process* tmp;
            while(head != NULL){
                tmp = head;
                head = head->next;
                delete tmp;
            }
        }
    }
    bool IsEmpty() const {
        return (tail == NULL);
    }
    void Enqueue(const Process& p){
        Process* tmp = new Process;
        tmp->pid = p.pid;
        tmp->arrival_t = p.arrival_t;
        tmp->burst_t = p.burst_t;

        if(tail == NULL)
            head = cur = tmp;
        else
            tail->next = tmp;
        tail = tmp;

        this->size += 1;
    }
    void Dequeue(Process& p){
        Process* tmp = head;
        p.pid = tmp->pid;
        p.arrival_t = tmp->arrival_t;
        p.burst_t = tmp->burst_t;

        head = head->next;
        if(head == NULL){
            tail = NULL;
        }
        delete tmp;
        this->size -= 1;
    }
    void CurP(Process& p){
        p.pid = cur->pid;
        cur = cur->next;
    }
    void PrintQ(){ //출력
        Process* tmp = head;
        cout << "ReadyQueue: ";
        while(tmp != NULL){
            if(tmp->next != NULL)
                cout << "P" << tmp->pid << " " << tmp->arrival_t\
                << " " << tmp->burst_t << ", ";
            else
                cout << "P" << tmp->pid << " " << tmp->arrival_t\
                << " " << tmp->burst_t;
            tmp = tmp->next;
        }
        cout << endl;
    }
    friend void FCFS(ReadyQueue readyQ);
    friend void SJF(ReadyQueue readyQ);
    friend void RR(ReadyQueue readyQ);
};


// 프로세스 정보 입력
void Input(ReadyQueue& readyQ){
    int process_num;
    Process tmpP;
    cout << "number of process: "; // 프로세스 개수 입력
    cin >> process_num;
    for(int i = 0; i < process_num; i++){
        cout << i+1 << "-th process" << endl; // 1번부터 시작
        cout << "Arrival Time / Burst Time " << endl;
        tmpP.pid = i+1;
        cin >> tmpP.arrival_t >> tmpP.burst_t; // 도착시간, 수행시간 입력
        readyQ.Enqueue(tmpP);
    }
}


// FCFS 알고리즘
void FCFS(ReadyQueue readyQ){
    Process p;
    int pre_burst_t = 0;
    int service_t = 0;
    vector<int> vec = {}; // wait time 배열
    int wait_t = 0;
    float sum_wait_t = 0;
    float avg_w_t = 0;
    int size = readyQ.size;

    cout << endl << "----------------------------------FCFS----------------------------------";
    cout << endl << "<Process Time>" << endl;
    for(int i = 0; i < size; i++){
        readyQ.Dequeue(p); // 현재 처리중인 프로세스 정보
        service_t += pre_burst_t; // 프로세스가 수행되는 시점
        wait_t = service_t - p.arrival_t;
        vec.push_back(wait_t);
        sum_wait_t += wait_t;

        // 출력
        cout << "P" << p.pid << " : " << service_t << "~" <<\
        service_t + p.burst_t << " " << p.burst_t << "실행" << endl;

        pre_burst_t = p.burst_t; // burst time은 다음 프로세스 waiting time에 영향
    }
    avg_w_t = sum_wait_t/size;
    cout << "<Wait Time>" << '\t' << '\t';
    for(int i = 0; i < vec.size(); i++){
        cout << "P" << i+1 << ": " << vec.at(i) << "  ";
    }
    cout << endl << "<Average Waiting Time>" << '\t';
    cout << "(";
    for(int j = 0; j < vec.size(); j++){
        if(j != vec.size() - 1)
            cout << vec.at(j) << " + ";
        else
            cout << vec.at(j);
    }
    cout << ")/" << vec.size() << " = " << avg_w_t;
}


// Preemptive SJF 알고리즘
void SJF(ReadyQueue readyQ){
    Process p;
    int size = readyQ.size;
    int t = 0;
    int complete = 0;
    bool check = false;
    int min = INT_MAX;
    int shortest = 0;
    int finish_time;
    vector<int> burst_vec = {};
    vector<int> arrival_vec = {};
    vector<int> wait_vec(size);
    vector<int> tmp = {};
    float sum_w_t = 0;
    float avg_w_t = 0;


    cout << endl << "----------------------------------SJF----------------------------------";
    for(int i = 0; i < size; i++){
        readyQ.Dequeue(p);
        tmp.push_back(p.burst_t);
        burst_vec.push_back(p.burst_t);
        arrival_vec.push_back(p.arrival_t);
    }

    // wait time 구하기
    while(complete != size){
        for(int j = 0; j < size; j++){
            if((arrival_vec.at(j) <= t) && (tmp.at(j) < min) && tmp.at(j) > 0){
                min = tmp.at(j);
                shortest = j;
                check = true;
            }
        }
        if(check == false){
            t++;
            continue;
        }
        tmp.at(shortest)--;

        min = tmp.at(shortest);
        if (min == 0)
            min = INT_MAX;
        if(tmp.at(shortest) == 0){
            complete++;
            check = false;
            finish_time = t+1;
            wait_vec.at(shortest) = finish_time- burst_vec.at(shortest) - arrival_vec.at(shortest);
            if(wait_vec.at(shortest) < 0)
                wait_vec.at(shortest) = 0;
        }
        t++;
    }

    // 평균 wait time 구하기
    for(int i = 0; i < size; i++){
        sum_w_t += wait_vec.at(i);
    }
    avg_w_t = sum_w_t/size;
    cout << "<Wait Time>" << '\t' << '\t';
    for(int i = 0; i < wait_vec.size(); i++){
        cout << "P" << i+1 << ": " << wait_vec.at(i) << "  ";
    }
    cout << endl << "<Average Waiting Time>" << '\t';
    cout << "(";
    for(int i = 0; i < wait_vec.size(); i++){
        if(i != wait_vec.size()-1)
            cout << wait_vec.at(i) << " + ";
        else
            cout << wait_vec.at(i);
    }
    cout << ")/" << wait_vec.size() << " = " << avg_w_t;
}


// Round Robin 알고리즘
void RR(ReadyQueue readyQ){
    Process p;
    int size = readyQ.size;
    int t = 0;
    int quantum = 1;
    vector<int> burst_vec = {};
    vector<int> arrival_vec = {};
    vector<int> wait_vec(size);
    vector<int> tmp = {};
    float sum_w_t = 0;
    float avg_w_t = 0;

    cout << endl << "----------------------------------RR----------------------------------";
    for(int i = 0; i < size; i++){
        readyQ.Dequeue(p);
        tmp.push_back(p.burst_t);
        burst_vec.push_back(p.burst_t);
        arrival_vec.push_back(p.arrival_t);
    }

    // wait time 구하기
    while(1){
        bool done = true;
        for(int i = 0; i < size; i++){
            if(tmp.at(i) > 0){
                done = false;
                if(tmp.at(i) > quantum){
                    t += quantum;
                    tmp.at(i) -= quantum;
                }
                else{
                    t = t+tmp.at(i);
                    wait_vec.at(i) = t - burst_vec.at(i);
                    tmp.at(i) = 0;
                }
            }
        }
        if (done == true)
            break;
    }

    // 평균 wait time 구하기
    for(int i = 0; i < size; i++){
        sum_w_t += wait_vec.at(i);
    }
    avg_w_t = sum_w_t/size;
    cout << endl << "<Wait Time>" << '\t' << '\t';
    for(int i = 0; i < wait_vec.size(); i++){
        cout << "P" << i+1 << ": " << wait_vec.at(i) << "  ";
    }
    cout << endl << "<Average Waiting Time>" << '\t';
    cout << "(";
    for(int i = 0; i < wait_vec.size(); i++){
        if(i != wait_vec.size()-1)
            cout << wait_vec.at(i) << " + ";
        else
            cout << wait_vec.at(i);
    }
    cout << ")/" << wait_vec.size() << " = " << avg_w_t;

}


int main(){
    ReadyQueue readyQ;
    Input(readyQ);
//    FCFS(readyQ);
//    SJF(readyQ);
    RR(readyQ);
    // 한번에 모두 실행하면 오류가 나서 한번에 한 알고리즘만 수행하게끔
    // 주석처리 해 놓았습니다.
    return 0;
}
