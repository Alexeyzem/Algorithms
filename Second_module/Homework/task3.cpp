//Постройте B-дерево минимального порядка t и выведите его по слоям.
//В качестве ключа используются числа, лежащие в диапазоне 0..2^32 -1

#include <iostream>
#include <vector>
#include "queue"
template <class T, class Comparator>
class BTree
{
public:
    struct Node
    {
        Node(bool leaf): leaf(leaf){};

        ~Node(){
            for (Node* child: children){
                delete child;
            }
        }

        bool leaf;
        std::vector<T> keys;
        std::vector<Node*> children;
    };

    BTree(size_t min_degree, Comparator _cmp): t(min_degree), root(nullptr), cmp(_cmp){
    }

    ~BTree(){
        if (root)
            delete root;
    }

    void Insert(const T &key){
        if (!root)
            root = new Node(true);

        //сначала разбиваем полный узел
        if (isNodeFull(root))
        {
            Node *newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            splitChild(root, 0);
        }
        insertNonFull(root, key);
    }

    //функция для тестирования
    void debugPrint(){
        debugPrintInternal(root, 0);
    }
   void Print(){
       std::queue<Node*> queue;
       queue.push(root);
       queue.push(nullptr);
       while (!queue.empty()){
           if (!queue.front() && queue.size()>1){
               std::cout<<std::endl;
               queue.push(nullptr);
           } else if (queue.front()){
               Node* node = queue.front();
               PrintNode(node);
               for (auto child:node->children){
                   queue.push(child);
               }
           }
           queue.pop();
       }
    }
private:
    void PrintNode(Node* node){
      for (auto key:node->keys){
          std::cout << key << " ";
      }
    }
    //функция для тестирования
    void debugPrintInternal(Node *node, int indent){
        std::cout << std::string(indent, ' ');
        std::cout << "keys: [";
        for (auto it = node->keys.begin(); it != node->keys.end(); it++){
            std::cout << (*it);
            if (it + 1 != node->keys.end())
                std::cout << ", ";
        }
        std::cout << "]" << std::endl;

        for (auto child: node->children){
            debugPrintInternal(child, indent + 4);
        }
    }

    bool isNodeFull(Node *node){
        return node->keys.size() == 2*t - 1;
    }

    void splitChild(Node *node, size_t index){
        Node* newChild = new Node(node->children[index]->leaf);
        for (int i=0; i < t-1; ++i){
            newChild->keys.push_back(node->children[index]->keys[i+t]);
        }
        node->children[index]->keys.resize(t);
        if (!node->children[index]->leaf){
            for(int i = 0; i < t; ++i){
                newChild->children.push_back(node->children[index]->children[i+t]);
            }
            node->children[index]->children.resize(t);
        }
        node->children.push_back(node->children[node->children.size()-1]);

        for (int j = node->children.size()-2; j > index+1; --j){
            node->children[j] = node->children[j-1];
        }
        node->children[index+1] = newChild;
        T key = node->children[index]->keys[node->children[index]->keys.size()-1];
        node->children[index]->keys.resize(node->children[index]->keys.size()-1);
        if (node->keys.size() == 0){
            node->keys.push_back(key);
        } else {
            node->keys.push_back(node->keys[node->keys.size()-1]);
            for (int j = node->keys.size()-2; j > index; --j){
                node->keys[j] = node->keys[j-1];
            }
            node->keys[index] = key;
        }

    };

    void insertNonFull(Node *node, const T &key){
        int pos = node->keys.size() - 1;
        if (node->leaf){
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && cmp(key, node->keys[pos])){
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        }
        else{
            while (pos >= 0 && cmp(key, node->keys[pos])){
                pos--;
            }
            if (isNodeFull(node->children[pos + 1])){
                splitChild(node, pos + 1);
                if (!cmp(key, node->keys[pos + 1]))
                    pos++;
            }
            insertNonFull(node->children[pos + 1], key);
        }
    }
    Comparator cmp;
    size_t t;
    Node *root;

    //для тестов
    friend void test1();
};
struct CharCmp{
    bool operator()(char l, char r){return l<r;}
};
// случаи вставки с иллюстраций в лекции
void test1()
{
    CharCmp Cmp;
    BTree<char, CharCmp> tree(3, Cmp);

    tree.root = new BTree<char, CharCmp>::Node(false);
    tree.root->keys = {'G', 'M', 'P', 'X'};

    {
        auto child = new BTree<char, CharCmp>::Node(true);
        child->keys = {'A', 'C', 'D', 'E'};
        tree.root->children.push_back(child);
    }

    {
        auto child = new BTree<char, CharCmp>::Node(true);
        child->keys = {'J', 'K'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char, CharCmp>::Node(true);
        child->keys = {'N', 'O'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char, CharCmp>::Node(true);
        child->keys = {'R', 'S', 'T', 'U', 'V'};
        tree.root->children.push_back(child);
    }
    {
        auto child = new BTree<char, CharCmp>::Node(true);
        child->keys = {'Y', 'Z'};
        tree.root->children.push_back(child);
    }

    std::cout << "Initial tree:" << std::endl;
    tree.debugPrint();
    std::cout << std::endl;

    std::string insertKeys = "BQLF";
    // посимвольно добавляем в дерево ключи
    for (auto c: insertKeys)
    {
        tree.Insert(c);
        std::cout << "After inserting " << c << ":" << std::endl;
        tree.debugPrint();
        std::cout << std::endl;
    }
}
struct IntCmp{
    bool operator()(int l, int r){return l<r;}
};
int main() {
    //test1();
    IntCmp Cmp;
    int t;
    std::cin >> t;
    BTree<int, IntCmp> tree(t, Cmp);
    int value;
    while ( std::cin >> value){

    tree.Insert(value);
    }
   tree.Print();
    return 0;
}