//В операционной системе Technux есть планировщик процессов.
//Каждый процесс характеризуется:
//приоритетом P
//временем, которое он уже отработал t
//временем, которое необходимо для завершения работы процесса T
//Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет его время P и кладет обратно в очередь процессов.
//Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
//Требуется посчитать кол-во переключений процессора

#include "iostream"
#include <cassert>

struct Element{
    int P;
    int t;
    int T;
};

template<class T>
class IsLessDefault{
public:
    bool operator() (const T& l, const T & r) {return l < r ;};
};

template<class T, class IsLess = IsLessDefault<T>>
class Heap{
public:
    explicit Heap(IsLess _isLess = IsLess()): buffer(nullptr), isLess(_isLess), bufferSize(0), size(0){};

    explicit Heap(const T* _arr, int arrSize, IsLess _isLess = IsLess()):isLess(_isLess), size(arrSize), bufferSize(arrSize){
        buffer = new T[arrSize];
        for (int i = 0; i < arrSize; i++){
            buffer[i] = _arr[i];
        }
        buildHeap();
    };

    ~Heap(){
        delete []buffer;
        buffer = nullptr;
        size = 0;
        bufferSize = 0;
    };

    Heap(Heap&) = delete;
    Heap& operator=(const Heap) = delete;

    void Insert(const T& el){
        if (size == bufferSize) {
            grow();
        }
        buffer[size] = el;
        ++size;
        siftUp(size-1);
    };

    T ExtractMax(){
        assert(size>0);
        T out = buffer[0];
        buffer[0] = buffer[size-1];
        size--;
        siftDown(0);
        return out;
    };

    T& PeekMax()const{
        assert(size > 0);
        return buffer[0];
    };

    int Size() const{
        return size;
    };

private:
    IsLess isLess;
    T* buffer;
    int bufferSize;
    int size;

    void buildHeap(){
        for (int i = size/2-1; i>=0; --i){
            siftDown(i);
        }
    };

    void siftDown(int i){
        while (i < size){
            int left = 2*i + 1;
            int right = 2*i + 2;
            int largest = i;
            if (left < size && isLess(buffer[largest], buffer[left])){
                largest = left;
            }
            if (right < size && isLess(buffer[largest], buffer[right])){
                largest = right;
            }
            if (largest != i){
                std::swap(buffer[largest], buffer[i]);
                i = largest;
            } else {
                return;
            }
        }
    };

    void siftUp(int i){
        while (i > 0){
            int parent = (i-1)/2;
            if (isLess(buffer[i], buffer[parent])){
                return;
            }
            std::swap(buffer[i], buffer[parent]);
            i = parent;
        }
    };

    void grow(){
        if (bufferSize == 0) {
            bufferSize = 1;
        } else {
            bufferSize = bufferSize * 2;
        }
        T* newBuffer = new T[bufferSize];
        for (int i = 0; i < size; ++i){
            newBuffer[i] = buffer[i];
        }
        delete []buffer;
        buffer = newBuffer;
    };
};

int main(){
    int count;
    std::cin >> count;
    Element el{};
    Element arrEl[count];
    for (int i = 0; i < count; i++){
        int P, T;
        std::cin >> P;
        std::cin >> T;
        el.P = P;
        el.T = T;
        el.t = 0;
        arrEl[i] = el;
    }
    int out = 0;
    Heap h(arrEl, count, [](const Element& l, const Element& r){return l.P*(l.t+1) > r.P*(r.t+1);});
    while (h.Size() > 0){
        Element tmp = h.ExtractMax();
        tmp.t += tmp.P;
        if (tmp.t < tmp.T){
            h.Insert(tmp);
        }
        out++;
    }
    std::cout << out;
    return 0;
}
