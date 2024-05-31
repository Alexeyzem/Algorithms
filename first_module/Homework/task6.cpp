#include "iostream"
#include "assert.h"

//6_4. Реализуйте стратегию выбора опорного элемента “случайный элемент”.
// Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.


template<class T, class IsLessClass>
int Partition( T* arr, int l, int r, IsLessClass isLess) {
    if( r-l <= 1 ) {
        return l;
    }
    int pivotPos = l + std::rand()%(r-l);
    std::swap(arr[pivotPos], arr[l]);
    const T& pivot = arr[l];
    int i = r;
    for (int j = r-1; j > 0; --j){
        if (isLess(pivot, arr[j])){
            std::swap(arr[--i], arr[j]);
        }
    }
    std::swap(arr[l], arr[--i]);
    return i;
}



template<class T, class IsLessClass>
int FindKStat(T *arr, int n, int k, IsLessClass isLess) {
    assert(k >= 0 && k < n);
    int l = 0;
    int r = n;
    while(l < r) {
        int pivotPos = Partition(arr, l, r, isLess);
        if (pivotPos == k){
            return arr[k];
        }
        if (k < pivotPos){
           r =  pivotPos;
        } else {
            l = pivotPos+1 ;
        }
    }
}

int main(){
    int count;
    std::cin >> count;
    int arr [count];
    for (int i = 0; i < count; ++i){
        int data;
        std::cin >> data;
        arr[i] = data;
    };
    std::cout << FindKStat(arr, count, count/10, [](const int& l, const int& r){return l < r;});
    std::cout << "\n";
    std::cout << FindKStat(arr, count, count/2, [](const int& l, const int& r){return l < r;});
    std::cout << "\n";
    std::cout << FindKStat(arr, count, 9*count/10, [](const int& l, const int& r){return l < r;});
    return 0;
}