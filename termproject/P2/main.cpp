#include <iostream>

using namespace std;

class Process{
public:
    Process* next; // Pointer
    int number;
    int arrival_t; // 도착 시간
    int burst_t; // cpu 실행 시간
    Process(int _arrival_t = 0, int _burst_t = 0):arrival_t(_arrival_t), burst_t(_burst_t) {
        number = 0;
        next = NULL;
    }
    ~Process(){}

};

class ReadyQueue{
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
        if(!this->IsEmpty()){
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
        tmp->number = p.number;
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
        p.number = tmp->number;
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
        p.number = cur->number;s
        cur = cur->next;
    }



};

void FCFS();
void SJF();
void RR();


int main(){







    return 0;
}
