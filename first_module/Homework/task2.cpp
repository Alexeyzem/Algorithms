    #include <iostream>

    int binarySearch(int* arr, int size){
        if (size == 1){
            return 0;
        }

        int left = 0;
        int right = 1;
        while (right < size-1){
            if (arr[right] < arr[right+1]){
                left = right;
                right = right << 1;
                if (right >= size){
                    right = size-1;
                }
            } else {
                break;
            }
        }
        int mid = 0;
        while (left < right){
            mid = (left + right)/2;
            if (arr[mid] > arr[mid+1]){
                right = mid;
            } else {
                left = mid+1;
            }
        }
        return left;
    }


    int main(){
        int N = 0;
        std::cin >> N;
        int arr[N]; // так как N<10000, то выделяем не в дин. памяти.
        for (int i = 0; i < N; ++i){
            int tmp = 0;
            std::cin >> tmp;
            arr[i] = tmp;
        }
        std::cout << binarySearch(arr, N);
        return 0;
    }