//Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией.
// Хранимые строки непустые и состоят из строчных латинских букв.
//Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
//Начальный размер таблицы должен быть равным 8-ми.
// Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
//Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.
//1_2. Для разрешения коллизий используйте двойное хеширование

#include "iostream"
#include "vector"
#include "string"
#include "cassert"

template<class T, class H, class H2>
class HashTable{
public:
    HashTable(int initialSize, const H& hasher, const H2& hasher2);
    ~HashTable();
    bool Has(const T& key);

    bool Add(const T& key);

    bool Delete(const T& key);

    unsigned int Size(){
        return this->table.size();
    }

private:
    H hasher;
    H2 hasher2;
    enum CellState {Empty, KEY, Deleted};
    struct HashTableCell{
       T Key;
       unsigned int Hash;
       CellState State;
        HashTableCell(): Hash(0), State(Empty){};
        HashTableCell(T _key, unsigned int _Hash): Hash(_Hash), State(KEY), Key(_key){};
    };
    std::vector<HashTableCell> table;
    unsigned int ElCount;
    unsigned int DeletedCount;


    void growTable(unsigned int size);

};

template<class T, class H, class H2>
HashTable<T, H, H2>::HashTable(int initialSize, const H& _hasher, const H2& _hasher2):
        hasher(_hasher),
        hasher2(_hasher2),
        table(initialSize, HashTableCell()),
        ElCount(0),
        DeletedCount(0)
{

}
template<class T, class H, class H2>
HashTable<T, H, H2>::~HashTable()
{
    table.clear();
}

template<class T, class H, class H2>
bool HashTable<T, H, H2>::Has(const T& key){
    unsigned int hash = hasher(key);
    int i = 1;
    unsigned int ListIndex = hash % table.size();
    while (table[ListIndex].State != Empty && table[ListIndex].Key != key){
        ListIndex = (hash+i*hasher2(key) % table.size()) % table.size();
        ++i;
    }
    return table[ListIndex].Key==key && table[ListIndex].State == KEY;
}

template<class T, class H, class H2>
bool HashTable<T, H, H2>::Add(const T& key){
    if (4*(ElCount+1) >= 3*table.size()){
        growTable(2*table.size());
    }
    int firstDeleted = -1;
    int i = 1;
    unsigned int hash = hasher(key);
    unsigned int ListIndex = hash % table.size();
    while (table[ListIndex].State != Empty && table[ListIndex].Key != key){
        if (table[ListIndex].State == Deleted && firstDeleted == -1 ){
            firstDeleted = ListIndex;
        }
        ListIndex = (hash + i*hasher2(key) % table.size()) % table.size();
        ++i;
    }

    if (table[ListIndex].State == KEY){
        return false;
    }
    if (firstDeleted != -1){
        table[firstDeleted] = HashTableCell(key, hash);
        --DeletedCount;
    } else {
        table[ListIndex] = HashTableCell(key, hash);
    }
    ++ElCount;
    return true;
}
template<class T, class H, class H2>
bool HashTable<T, H, H2>::Delete(const T& key){
    if (2*(DeletedCount+1) >= table.size()){
        growTable(table.size());
    }
    unsigned int hash = hasher(key);
    unsigned int ListIndex = hash % table.size();
    int i = 1;
    while (table[ListIndex].State != Empty && table[ListIndex].Key != key){
        ListIndex = (hash + i*hasher2(key) % table.size()) % table.size();
        ++i;
    }
    if (table[ListIndex].State != KEY ){
        return false;
    }
    table[ListIndex].State = Deleted;
    --ElCount;
    ++DeletedCount;
    return true;

}

template<class T, class H, class H2>
void HashTable<T, H, H2>::growTable(unsigned int newSize) {
    std::vector<HashTableCell> newTable(newSize, HashTableCell());
    for (int i = 0; i < table.size(); ++i){
        if (table[i].State!=KEY){
            continue;
        }
        HashTableCell node = table[i];
        unsigned int ListIndex = node.Hash % newTable.size();
        int j = 1;
        while (newTable[ListIndex].State != Empty){
            ListIndex = (node.Hash + j*hasher2(node.Key) % newTable.size()) % newTable.size();
            ++j;
        }
            newTable[ListIndex] = node;
    }
    DeletedCount = 0;
    table = std::move(newTable);

}


struct StringHasher{
    unsigned int operator() (const std::string& key)const{
        unsigned int hash = 0;
        for (unsigned int i = 0; i < key.size(); ++i){
            hash = hash*137 + key[i];
    }
    return hash;
    }
};

struct HelperHasher{
    unsigned int operator() (const std::string& key)const{
        if ((157 - key.size()%101) % 2 == 0){
            return (157 - key.size()%101)+1;
        }
        return (157 - key.size()%101);
    }
};

int main(){
    StringHasher hasher;
    HelperHasher hasher2;
    HashTable<std::string, StringHasher, HelperHasher> table(8, hasher, hasher2);

    char operation = 0;
    std::string word;
    while (std::cin >> operation >> word){
        switch (operation) {
            case '+':
                std::cout << (table.Add(word) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (table.Has(word) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
                break;
        }
    }

    return 0;
}