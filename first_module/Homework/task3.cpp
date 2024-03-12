//Реализовать очередь с помощью двух стеков

#include <iostream>
#include <assert.h>

class Stack{
public:
    Stack();
    ~Stack();
    Stack(Stack&) = delete;
    Stack& operator=(const Stack) = delete;
    void Push(int data);
    int Pop();
    int Size() const;
private:
    int size;
    int bufferSize;
    int* buffer;
    void reallocate();
};

Stack::Stack():size(0), bufferSize(0), buffer(nullptr)
{

}

Stack::~Stack() {
    size = 0;
    bufferSize = 0;
    delete[] buffer;
}

int Stack::Size() const {
    return size;
}

int Stack::Pop() {
    assert(size > 0);
    size--;
    return buffer[size];

}

void Stack::reallocate() {
    if (bufferSize == 0) {
        bufferSize = 1;
    } else {
        bufferSize = bufferSize * 2;
    }
    int* newBuffer = new int[bufferSize];
    for (int i = 0; i < size; ++i){
        newBuffer[i] = buffer[i];
    }
    delete []buffer;
    buffer = newBuffer;
}

void Stack::Push(int data) {
    if (size == bufferSize) {
      reallocate();
    }
    buffer[size] = data;
    ++size;
}

class Queue{
public:
    Queue();
    ~Queue();
    Queue(Queue&) = delete;
    Queue& operator=(const Queue) = delete;
    void EnQueue(int data);
    int DeQueue();
    int Size() const;
private:
    Stack stIn;
    Stack stOut;
    void reallocate();
};

Queue::Queue(): stIn(), stOut()
{

}

Queue::~Queue()
{
}

int Queue::Size() const {
    return stIn.Size()+stOut.Size();
}

void Queue::EnQueue(int data) {
    stIn.Push(data);
}

int Queue::DeQueue() {
    assert (Size()>0);
    if (stOut.Size() == 0){
        reallocate();
    }
   return stOut.Pop();
}

void Queue::reallocate() {
    while (stIn.Size() > 0){
        stOut.Push(stIn.Pop());
    }
}

int main(){
    int count = 0;
    std::cin >> count;
    Queue intQueue;
    bool result = true;
    for (int i = 0; i < count; ++i){
        int command = 0;
        int data = 0;
        std::cin >> command >> data;
        switch (command) {
            case 2:
                if (intQueue.Size() > 0){
                    result = result && intQueue.DeQueue() == data;
              }else{
                  result = result && data == -1;
                }
                break;
            case 3:
                intQueue.EnQueue(data);
                break;
            default:
                assert(false);
        }
    }
    std::cout << (result ? "YES" : "NO");

    return 0;
}