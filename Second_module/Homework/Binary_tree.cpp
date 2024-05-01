//Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
//Требуется построить бинарное дерево, заданное наивным порядком вставки.
//Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K,
// то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
//Требования: Рекурсия запрещена. Решение должно поддерживать передачу функции сравнения снаружи.
//2_1. Выведите элементы в порядке in-order (слева направо).

#include "iostream"
#include "vector"
#include "stack"

template <class T, class Compare>
class BinaryTree{
public:
    BinaryTree(const Compare& _cmp): root(nullptr), cmp(_cmp){};
    ~BinaryTree();

    void Add( const T& key);
    bool Has( const T& key) const;
    void Dfs(void (*visit)(const T& key)) const;
private:

    struct Node{
        T Key;
        Node* Left;
        Node* Right;

        Node(const T& key): Key(key), Left(nullptr), Right(nullptr){}
    };
    Node* root;
    Compare cmp;
};

template <class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree(){
    if (!root){
        return;
    }
    std::vector<Node*> delVector;
    std::stack<Node*> st;
    Node* last = nullptr;
    while (!st.empty()){
        Node* node = st.top();
        if (!last || last->Left == node || last->Right == node){
            if (node->Left){
                st.push(node->Left);
            } else if (node->Right){
                st.push(node->Right);
            } else {
                st.pop();
                delVector.push_back(node);
            }
        }
        else if(node->Left == last){
            if (node->Right){
                st.push(node->Right);
            } else{
                st.pop();
                delVector.push_back(node);
            }
        } else if (node->Right== last){
            st.pop();
            delVector.push_back(node->Right);
        }
        last = node;
    }

    for (int i = 0; i < delVector.size(); ++i){
        delete delVector[i];
    }
}


template <class T, class Compare>
void BinaryTree<T, Compare>:: Add( const T& key){
    if (root == nullptr){
        root = new Node(key);
        return;
    }

    Node* node = root;
    Node* lastNode = node;
    while (node != nullptr){
        lastNode = node;
        if (cmp(key, node->Key) ){
            node = node->Left;
        } else{
            node = node->Right;
        }
    }
    if (cmp(key, lastNode->Key)){
        lastNode->Left = new Node(key);
    } else {
        lastNode->Right = new Node(key);
    }
};


template <class T, class Compare>
bool BinaryTree<T, Compare>:: Has( const T& key) const{
    Node* node = root;
    while (node->Key != key && node != nullptr){
        if (cmp(key, node->Key)){
            node = node->Left;
        } else{
            node = node->Right;
        }
    }
    if (node->Key == key){
        return true;
    }
    return false;
};


template <class T, class Compare>
void BinaryTree<T, Compare>::Dfs(void (*visit)(const T& key)) const{
    std::stack<Node*> st;
    Node* node = root;
    if (node == nullptr){
        return;
    }
   while (node || !st.empty()){
       while (node){
           st.push(node);
           node = node->Left;
       }
       node = st.top();
       st.pop();
       visit(node->Key);
       node = node->Right;
   }

}


struct IntCmp{
    bool operator()(int l, int r){return l<r;}
};

int main(){
    IntCmp cmp;
    BinaryTree<int, IntCmp> tree(cmp);
   int count;
   std::cin >> count;
   for(int i = 0; i < count; ++i){
       int tmp;
       std::cin >> tmp;
       tree.Add(tmp);
   }
    tree.Dfs([](const int& key){std::cout << key << " ";});
    return 0;
}