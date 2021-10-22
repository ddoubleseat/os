#define JOBQUEUE 0
#define READY 1
#define RUNNING 2
#define WAIT 3

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class PCB{ // process control block
public:
    static int numcreatedPCB;
    PCB* next; // Pointer
    string pid; // process ID = PID
    int ps; // process state
    // (ps==0)->Jobqueue, 1->Ready, 2->Running, 3->wait

    PCB() {
        ps = JOBQUEUE;
        pid = "PID";
        next = NULL;
        // to_string: integer를 string타입으로 변환
        // stoi, atoi: string이나 char타입을 integer로 변환

    }
    ~PCB() {
        //cout << pid << " is destructed.." << endl;
    }

//    string getPID() {return pid;} // getter
//    int getPS() {return ps;} // getter
//    void setPS(int updatePS) {ps = updatePS;} //setter
    friend class Queue;
//    friend class JobQueue;
//    friend class ReadyQueue;
//    friend class DeviceQueue;
};

int PCB::numcreatedPCB = 1;

class JobQueue{
protected:
    int size;
    PCB* cur; // 현재 위치(Jobqueue에만 해당)
    PCB* head;
    PCB* tail;
public:
    JobQueue(){
        this->size = 0;
        head = tail = cur = NULL;
    }
    ~JobQueue(){
        if(IsEmpty()==true){
            PCB* tmpPtr;
            while(head != NULL){
                tmpPtr = head;
                head = head->next;
                delete tmpPtr;
            }
        }
    }

    bool IsEmpty() const {
        return (tail == NULL);
    }

    virtual void Enqueue(const string& pid){
        PCB* temp = new PCB; // 새로운 PCB Node 만들기
        temp->ps = JOBQUEUE; // ps == 0
        temp->pid += to_string(PCB::numcreatedPCB); // PID 설정

        if(tail == NULL)
            head = cur = temp;
        else
            tail->next = temp;
        tail = temp;

        this->size += 1;
        PCB::numcreatedPCB++;
    }

    void Dequeue(string& pid){ //PCB 회수하기
        PCB* tempPtr = head;
        pid = head->pid;
        head = head->next;
        if(head == NULL){
            tail = NULL;
        }
        delete tempPtr;
        this->size -= 1;
    }

    void CurPCB(string& pid){ //현재 queue의 위치(iteration)
        pid = cur->pid;
        cur = cur->next;
    }
    virtual void PrintQ(){ // 출력
        PCB* tempPtr;
        tempPtr = head;
        cout << "Job Queue: ";
        while(tempPtr != NULL){
            if (tempPtr->next != NULL)
                cout << tempPtr->pid << ", ";
            else
                cout << tempPtr->pid;
            tempPtr = tempPtr->next;
        }
        cout << endl;
    }

};

class ReadyQueue: public JobQueue{
public:
    void Enqueue(const string& pid) override{
        PCB* temp = new PCB; // 새로운 PCB Node 만들기
        temp->ps = READY; // ps == 1
        temp->pid = pid; // PID 설정

        if(tail == NULL)
            head = temp;
        else
            tail->next = temp;
        tail = temp;

        this->size += 1;
    }

    void PrintQ() override { // ReadyQueue 출력
        PCB* tempPtr;
        tempPtr = head;
        cout << "Ready: ";
        while(tempPtr != NULL){
            if (tempPtr->next != NULL)
                cout << tempPtr->pid << ", ";
            else
                cout << tempPtr->pid;
            tempPtr = tempPtr->next;
        }
        cout << endl;
    }
};

class DeviceQueue: public JobQueue{
public:
    void Enqueue(const string& pid) override{
        PCB* temp = new PCB; // 새로운 PCB Node 만들기
        temp->ps = WAIT; // ps == 3
        temp->pid = pid; // PID 설정

        if(tail == NULL)
            head = cur = temp;
        else
            tail->next = temp;
        tail = temp;

        this->size += 1;
    }
    void PrintQ() override { // DeviceQueue 출력
        PCB* tempPtr;
        tempPtr = head;
        cout << "Wait: ";
        while(tempPtr != NULL){
            if (tempPtr->next != NULL)
                cout << tempPtr->pid << ", ";
            else {
                cout << tempPtr->pid;
                cout << " (HDD I/O Queue)";
            }
            tempPtr = tempPtr->next;
        }
        cout << endl;
    }
};

void Print(const string& pid, JobQueue& jobQ,\
            ReadyQueue& readyQ, DeviceQueue& deviceQ){//출력

    readyQ.PrintQ();
    deviceQ.PrintQ();
    jobQ.PrintQ();
    cout << endl;
}

int main(){
    JobQueue jobQ;
    ReadyQueue readyQ;
    DeviceQueue deviceQ;
    PCB pcb;
    string pid, pid2, pid3 = "";

    for(int i = 0; i < 6; i++) { //새로운 프로세스 생성, jobQ에 enqueue
        jobQ.Enqueue(pid);
    }
    //jobQ.PrintQ();

    cout << "Process Creation" << endl;
    cout << "Running: " << endl;
    Print(pid, jobQ, readyQ, deviceQ);

    for(int i = 0; i < 4; i++){ // readyQ에 enqueue
        jobQ.CurPCB(pid);
        readyQ.Enqueue(pid);
    }
    //readyQ.PrintQ();

    readyQ.Dequeue(pid);//readyQ에서 CPU dispatch, Ready->Running
    cout << pid << " Process Running" << endl;
    cout << "Running: " << pid << endl; //Running 상태일때의 클래스는 따로 구현하지 않음.
    Print(pid, jobQ, readyQ, deviceQ);

    deviceQ.Enqueue(pid); // I/O Event 발생, Running->Wait
    cout << pid << " HDD I/O Event" << endl;
    cout << "Running: " << endl;
    Print(pid, jobQ, readyQ, deviceQ);

    readyQ.Dequeue(pid);
    jobQ.CurPCB(pid2);
    readyQ.Enqueue(pid2);
    cout << pid << " Process Running" << endl;
    cout << "Running: " << pid << endl;
    Print(pid, jobQ, readyQ, deviceQ);

    cout << "HDD I/O finished" << endl;
    cout << "Running: " << pid << endl;
    Print(pid, jobQ, readyQ, deviceQ);

    cout << pid << " Process Terminated" << endl;
    cout << "Running: " << endl;
    jobQ.Dequeue(pid);
    Print(pid, jobQ, readyQ, deviceQ);


    return 0;
}