//7_2. LSD для long long.
//Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше 10^6.
// Отсортировать массив методом поразрядной сортировки LSD по байтам.


#include "iostream"
#include "memory.h"


template<class T, class TGetKey>
void CountingSort( T* arr, int n, TGetKey getKey) {
    int max = getKey(arr[0]);

    for (int i = 1; i < n; ++i){
        if (max < getKey(arr[i])){
            max = getKey(arr[i]);
        }
    }

    int* c = new int[max+1];
    memset(c, 0, (max+1)*sizeof(int));

    for( int i = 0; i < n; ++i )
        ++c[getKey(arr[i])];

    for (int i = 1; i < max+1; ++i){
        c[i] += c[i-1];
    }


    T* b = new T[n];
    for( int i = n-1; i >= 0; --i ) {
        b[--c[getKey(arr[i])]] = arr[i];
    }
    delete[] c;

    for (int i = 0; i < n ; ++i){
        arr[i] = b[i];
    }
    delete[] b;
}
int main(){
    int count;
    std::cin >> count;
    unsigned long long arr[count];
    for (int i = 0; i < count; ++i){
        unsigned long long data;
        std::cin >> data;
        arr[i] = data;
    }
    for (int i = 0; i < 8; ++i) {
        CountingSort(arr, count, [i](unsigned long long val) { return (val >> i * 8) & 255; });
    }
    for (int i = 0; i < count; ++i){
        std::cout << arr[i] << " " ;
    }

    return 0;
}