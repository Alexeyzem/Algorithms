    //4_2. Порядковые статистики. Дано число N и N строк.
    // Каждая строка содержит команду добавления или удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики.
    // Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
    // Запрос на получение k-ой порядковой статистики задается числом k.
    //Требования: скорость выполнения запроса - O(log n).

    #include "iostream"
    #include "stack"
#include "vector"
    #include "sstream"
    #include "cassert"

    template<class T, class Compare, class Equal>
    class AVLTree {

    private:
        struct Node {
            T Key;
            unsigned int Height;
            Node *Left;
            Node *Right;
            unsigned  int Count;
            Node(const T key) : Key(key), Left(nullptr), Right(nullptr), Height(1), Count(1) {}
        };

        Node *root;
        Compare cmp;
        Equal equal;


        unsigned int height(Node *p) {
            return p ? p->Height : 0;
        }
        unsigned int count(Node* p){
            return p ? p->Count : 0;
        }
        int BFactor(Node *p) {
            return height(p->Right) - height(p->Left);
        }

        void FixHeight(Node *p) {
            p->Height = std::max(height(p->Left), height(p->Right))+ 1;
            p->Count = count(p->Left) + count(p->Right) + 1;
        }

        Node *rotateRight(Node *p) {
            Node *left = p->Left;
            p->Left = left->Right;
            left->Right = p;
            FixHeight(p);
            FixHeight(left);
            return left;
        }

        Node *rotateLeft(Node *p) {
            Node *right = p->Right;
            p->Right = right->Left;
            right->Left = p;
            FixHeight(p);
            FixHeight(right);
            return right;
        }

        Node* Balance(Node* p) {
            FixHeight(p);
            if (BFactor(p) == 2) {
                if (BFactor(p->Right) < 0){
                    p->Right = rotateRight(p->Right);
                }
                return rotateLeft(p);
            }
            if (BFactor(p) == -2) {
                if (BFactor(p->Left) > 0) {
                    p->Left = rotateLeft(p->Left);
                }
                return rotateRight(p);
            }
            return p;
        }

        Node* insert(Node* node, T key) {
            if (!node){
                return new Node(key);
            }
            if (equal(node->Key, key)){
                return node;
            }
            if (cmp(key, node->Key)){
                node->Left = insert(node->Left, key);
            } else{
               node->Right = insert(node->Right, key);
            }
            return Balance(node);
        }

        Node* findAndRemoveMin(Node* p){
            if (!p->Left){
                return p;
            }
            Node* min = p;
            Node* last = p;
            while (min->Left){
                last = min;
                min = min->Left;
            }
            last->Left = min->Right;
            min->Right = p;
            std::vector<Node*> vec;
            Node* node = p;
            while (node) {
                vec.push_back(node);
                node = node->Left;
            }
            for (int i = vec.size()-1; i>0; --i){
                vec[i-1]->Left = Balance(vec[i]);
            }
            min->Right = Balance(vec[0]);
            return min;
        }

        Node* remove(Node* p, T k) {
            if (!p) {
                return p;
            }
            if (cmp(k, p->Key)) {
                p->Left = remove(p->Left, k);
            } else {
                if (!equal(k, p->Key)) {
                    p->Right = remove(p->Right, k);
                } else {
                    Node *left = p->Left;
                    Node *right = p->Right;
                    delete p;
                    if (!right) {
                        return left;
                    }
                    Node *min = findAndRemoveMin(right);
                    min->Left = left;
                    return Balance(min);
                }
            }
            return Balance(p);
        }

        T findStat(Node* node, int kStat){
            if (kStat == count(node->Left)){
                return node->Key;
            }
            if (kStat < count(node->Left)){
                return findStat(node->Left, kStat);
            }
            return findStat(node->Right, kStat - count(node->Left) - 1);
        }
        void deleteNode(Node* node){
            if (!node){
                return;
            }
            deleteNode(node->Left);
            deleteNode(node->Right);
            delete node;
        }
    public:
        AVLTree(Compare _cmp, Equal _equal):root(nullptr), cmp(_cmp), equal(_equal){};
        ~AVLTree(){
            deleteNode(root);
        }



        void Insert(T key) {
            root = insert(root, key);
        }

        void Remove(T key){
            root = remove(root, key);
        }
      T FindKStat(int kStat){
          return findStat(root, kStat);
        }
    };

    struct IntCmp{
        bool operator()(int l, int r){return l<r;}
    };
    struct IntEqual{
        bool operator()(int l, int r){return l == r;}
    };


    int run(std::istream& input, std::ostream & out){
        IntCmp cmp;
        IntEqual equal;
        AVLTree<int, IntCmp, IntEqual> tree(cmp, equal);
        int count ;
        input >> count;
        for(int i = 0 ; i < count; ++i){
            int number;
            int kStat;
            input >> number >> kStat;
            if (number > 0){
                tree.Insert(number);
            } else {
                tree.Remove((-1)*number);
            }
            out << tree.FindKStat(kStat) << std::endl;
        }
        return 0;
    }

    void TestAvlTree(){
        {
            std::stringstream input;
            std::stringstream output;
            input << "3 1 0 2 0 -1 0";
            run(input, output);
            std::string str = "1\n1\n2\n";
            if (output.str() != str){
                std::cout << "TEST FAILED";
                std::cout << "Output:" << std::endl << output.str() << std::endl << "Expected:" << std::endl << str;
            }
            assert(output.str() == str);
        }
        std::cout << "Test 1 OK" << std::endl;
        {
            std::stringstream input;
            std::stringstream output;
            input << "5 40 0 10 1 4 1 -10 0 50 2";
            run(input, output);
            std::string str = "40\n40\n10\n4\n50\n";
            if (output.str() != str){
                std::cout << "TEST FAILED";
                std::cout << "Output:" << std::endl << output.str() << std::endl << "Expected:" << std::endl << str;
            }
            assert(output.str() == str);
        }
        std::cout << "Test 2 OK"<< std::endl;
        {
            std::stringstream input;
            std::stringstream output;
            input << "1 100 0";
            run(input, output);
            std::string str = "100\n";
            if (output.str() != str){
                std::cout << "TEST FAILED";
                std::cout << "Output:" << std::endl << output.str() << std::endl << "Expected:" << std::endl << str;
            }
            assert(output.str() == str);
        }
        std::cout << "Test 3 OK"<< std::endl;
        {
            std::string bigData = "50 1 0 2 1 3 2 4 3 5 4 6 5 7 6 8 7 9 8 10 9 11 10 12 11 13 12 14 13 15 14 16 15 17 16 18 17 19 18 20 19 21 20 22 21 23 22 24 23 25 24 26 25 27 26 28 27 29 28 30 29 31 30 32 31 33 32 34 33 35 34 36 35 37 36 38 37 39 38 40 39 41 40 42 41 43 42 44 43 45 44 46 45 47 46 48 47 49 48 50 49";
            std::stringstream input;
            std::stringstream output;
            input << bigData;
            run(input, output);
            std::string str = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n";
            if (output.str() != str){
                std::cout << "TEST FAILED";
                std::cout << "Output:" << std::endl << output.str() << std::endl << "Expected:" << std::endl << str;
            }
            assert(output.str() == str);
        }
        std::cout << "Test 4 OK"<< std::endl;
        {
            std::string bigData = "50 1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 0 10 0 11 0 12 0 13 0 14 0 15 0 16 0 17 0 18 0 19 0 20 0 21 0 22 0 23 0 24 0 25 0 26 0 27 0 28 0 29 0 30 0 31 0 32 0 33 0 34 0 35 0 36 0 37 0 38 0 39 0 40 0 41 0 42 0 43 0 44 0 45 0 46 0 47 0 48 0 49 0 50 0";
            std::stringstream input;
            std::stringstream output;
            input << bigData;
            run(input, output);
            std::string str = "1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n";
            if (output.str() != str){
                std::cout << "TEST FAILED";
                std::cout << "Output:" << std::endl << output.str() << std::endl << "Expected:" << std::endl << str;
            }
            assert(output.str() == str);
        }
        std::cout << "Test 5 OK"<< std::endl;
        {
            std::string bigData = "50 1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 0 10 0 11 0 12 0 13 0 14 0 15 0 16 0 17 0 18 0 19 0 20 0 21 0 22 0 23 0 24 0 25 0 26 0 27 0 28 0 29 0 30 0 31 0 32 0 33 0 34 0 35 0 36 0 37 0 38 0 39 0 40 0 41 0 42 0 43 0 44 0 45 0 46 0 47 0 48 0 49 0 -30 0";
            std::stringstream input;
            std::stringstream output;
            input << bigData;
            run(input, output);
            std::string str = "1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n";
            if (output.str() != str){
                std::cout << "TEST FAILED";
                std::cout << "Output:" << std::endl << output.str() << std::endl << "Expected:" << std::endl << str;
            }
            assert(output.str() == str);
        }
        std::cout << "Test 6 OK"<< std::endl;
        {
            std::string bigData = "50 1 0 2 1 3 2 4 3 5 4 6 5 7 6 8 7 9 8 10 9 11 10 12 11 13 12 14 13 15 14 16 15 17 16 18 17 19 18 20 19 21 20 22 21 23 22 24 23 25 24 26 25 27 26 28 27 29 28 30 29 31 30 32 31 33 32 34 33 35 34 36 35 37 36 38 37 39 38 40 39 41 40 42 41 43 42 44 43 45 44 46 45 47 46 48 47 49 48 -20 47";
            std::stringstream input;
            std::stringstream output;
            input << bigData;
            run(input, output);
            std::string str = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n49\n";
            if (output.str() != str){
                std::cout << "TEST FAILED";
                std::cout << "Output:" << std::endl << output.str() << std::endl << "Expected:" << std::endl << str;
            }
            assert(output.str() == str);
        }
        std::cout << "Test 7 OK"<< std::endl;
        {
            std::string bigData = "50 1 0 2 1 3 2 4 3 5 4 6 5 7 6 8 7 9 8 10 9 11 10 12 11 13 12 14 13 15 14 16 15 17 16 18 17 19 18 20 19 21 20 22 21 23 22 24 23 25 24 26 25 27 26 28 27 29 28 30 29 31 30 32 31 33 32 34 33 35 34 36 35 37 36 38 37 39 38 40 39 41 40 42 41 43 42 44 43 45 44 46 45 47 46 48 47 -48 46 -20 45";
            std::stringstream input;
            std::stringstream output;
            input << bigData;
            run(input, output);
            std::string str = "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n47\n47\n";
            if (output.str() != str){
                std::cout << "TEST FAILED";
                std::cout << "Output:" << std::endl << output.str() << std::endl << "Expected:" << std::endl << str;
            }
            assert(output.str() == str);
        }
        std::cout << "Test 8 OK"<< std::endl;
        {
            std::string bigData = "50 1 0 2 0 3 0 4 0 5 0 6 0 7 0 8 0 9 0 10 0 11 0 12 0 13 0 14 0 15 0 16 0 17 0 18 0 19 0 20 0 21 0 22 0 23 0 24 0 25 0 26 0 27 0 28 0 29 0 30 0 -30 0 -16 0 -20 0 -1 0 -5 0 -8 0 -13 0 5 0 -9 0 -22 0 -3 0 -28 0 -17 0 -11 0 -26 0 -7 0 -10 0 -4 0 1 0 30 15";
            std::stringstream input;
            std::stringstream output;
            input << bigData;
            run(input, output);
            std::string str = "1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n1\n2\n2\n2\n2\n2\n2\n2\n2\n2\n2\n2\n2\n2\n2\n2\n1\n30\n";
            if (output.str() != str){
                std::cout << "TEST FAILED";
                std::cout << "Output:" << std::endl << output.str() << std::endl << "Expected:" << std::endl << str;
            }
            assert(output.str() == str);
        }
        std::cout << "TEST OK" << std::endl;
    }

    int main(){
       TestAvlTree();
        run(std::cin, std::cout);
        return 0;
    }
