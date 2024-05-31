// Инвертируйте значение бита в числе N по его номеру K

#include "iostream"

unsigned int inverseBit(int data, int pos){
    int tmp = 1 << pos;
    return data xor tmp;
}

int main(){
    unsigned int N = 0;
    std::cin >> N;
    int k = 0;
    std::cin >> k;
    unsigned int out = inverseBit(N, k);
    std::cout << out;
    return 0;
}