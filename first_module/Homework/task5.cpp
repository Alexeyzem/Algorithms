//На числовой прямой окрасили N отрезков. Известны координаты левого и правого концов
// каждого отрезка (Li и Ri). Найти длину окрашенной части числовой прямой.

#include "iostream"

struct Point{
    int X; //координата, где происходит изменение закраски
    int Delta; //какое изменение + или -
};

template<class T>
class IsLessDefault{
public:
    bool operator() (const T& l, const T& r) {return l < r ;};
};

template<class T, class IsLessClass = IsLessDefault<T>>
void MergeSort(T* arr, int size, IsLessClass isLess = IsLessClass()){
if(size <= 1){
    return;
}
int FirstLen = size/2;
int SecondLen = size - FirstLen;
    MergeSort(arr, FirstLen, isLess);
    MergeSort(arr+FirstLen, SecondLen, isLess);
    T* c = new T[size];
    Merge(arr, FirstLen, arr+FirstLen, SecondLen, c, isLess);
    for (int i = 0; i < FirstLen+SecondLen; ++i){
        arr[i] = c[i];
    }
    delete []c;
}
template<class T, class IsLessClass = IsLessDefault<T>>
void Merge(T* arr1, int size1, T* arr2, int size2, T* outArr, IsLessClass isLess = IsLessClass()){
 int first  = 0;
 int second = 0;
 int i = 0;
    while (first<size1){
        if (second == size2){
            while (i < size2+size1){
                outArr[i] = arr1[first];
                ++i;
                ++first;
            }
            return;
        }
        if (isLess(arr1[first], arr2[second])){
            outArr[i] = arr1[first];
            first++;
        } else {
            outArr[i] = arr2[second];
            second++;
        }
        ++i;
    }
    if (second < size2){
        while (i < size2+size1){
            outArr[i] = arr2[second];
            ++i;
            ++second;
        }
    }
}

int main() {
    int count;
    std::cin >> count;
    Point arr[2*count];
    for (int i = 0; i < count; ++i){
        int left;
        int right;
        std::cin >> left;
        std::cin >> right;;
        Point l{left, 1};
        Point r{right, -1};
        arr[2*i] = l;
        arr[2*i+1] = r;
    }

    MergeSort(arr, 2*count, [](const Point& l, const Point& r)
    {
        if (l.X == r.X){
            return l.Delta < r.Delta;
        }
            return l.X < r.X;
    });
    int painted = 0;
    int res = 0;
    int lastX;
    for (int i = 0; i < count*2; ++i){
        if (painted > 0 && i != 0){
            res += arr[i].X - lastX;
        }
        painted+= arr[i].Delta;
        lastX = arr[i].X;
    }

    std::cout << res;

    return 0;
}


// все отрезки конвертируем в такую штуку, где лево +1 право -1
// сортируем по координате Х и учитываем дельта (по увеличению)
// Перебираем все эти точки и двигаясь от координаты к координате понимаем толщину покраски.