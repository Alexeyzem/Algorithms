//Необходимо написать несколько реализаций интерфейса:
//ListGraph, хранящий граф в виде массива списков смежности,
//MatrixGraph, хранящий граф в виде матрицы смежности,
//SetGraph, хранящий граф в виде массива хэш-таблиц/сбалансированных деревьев поиска,
//ArcGraph, хранящий граф в виде одного массива пар {from, to}.
//Также необходимо реализовать конструктор, принимающий const IGraph&. Такой конструктор должен скопировать переданный граф в создаваемый объект.
//Для каждого класса создавайте отдельные h и cpp файлы.
//Число вершин графа задается в конструкторе каждой реализации.


#include "queue"
#include "iostream"
#include "SetGraph.h"
#include "MatrixGraph.h"
#include "ListGraph.h"
#include "ArcGraph.h"

void bfs(const IGraph& graph, int vertex, void(*visit)(int), std::vector<bool> *visited) {
    std::queue<int> bfsQ;
    bfsQ.push(vertex);
    (*visited)[vertex] = true;
    while (!bfsQ.empty()){
        int current = bfsQ.front();
        bfsQ.pop();
        visit(current);
        std::vector<int> adjList = graph.GetNextVertices(current);
        for (int i : adjList){
            if (!((*visited)[i])){
                bfsQ.push(i);
                (*visited)[i] = true;
            }
        }
    }
}

void BFS(const IGraph& graph, void(*visit)(int)){
    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int i = 0; i < graph.VerticesCount(); ++i){
        if (!visited[i]){
            bfs(graph, i, visit, &visited);
            std::cout << std::endl;
        }
    }
}

int main(){


    {
        std::cout << "FIRST TEST" << std::endl;
        //Граф по слоям(начиная с 0):
        //первый компонент связности:
        // 0
        // 1 4
        // 2 3 6
        // 5 7
        //второй компонент связности:
        // 8
        // 9
        ListGraph listGraph(10);
        listGraph.AddEdge(0, 1);
        listGraph.AddEdge(0, 4);
        listGraph.AddEdge(1, 2);
        listGraph.AddEdge(1, 3);
        listGraph.AddEdge(2, 5);
        listGraph.AddEdge(3, 0);
        listGraph.AddEdge(3, 4);
        listGraph.AddEdge(3, 2);
        listGraph.AddEdge(4, 6);
        listGraph.AddEdge(5, 3);
        listGraph.AddEdge(6, 3);
        listGraph.AddEdge(6, 5);
        listGraph.AddEdge(6, 7);
        listGraph.AddEdge(7, 5);
        listGraph.AddEdge(8, 9);
        std::cout << "BFS for listGraph:" << std::endl;
        BFS(listGraph, [](int vertex) { std::cout << vertex << " "; });
        std::cout << std::endl;
        std::cout << "BFS for matrixGraph:" << std::endl;
        MatrixGraph matrixGraph(listGraph);
        BFS(matrixGraph, [](int vertex) { std::cout << vertex << " "; });
        std::cout << std::endl;
        std::cout << "BFS for setGraph:" << std::endl;
        SetGraph setGraph(matrixGraph);
        BFS(setGraph, [](int vertex) { std::cout << vertex << " "; });
        std::cout << std::endl;
        std::cout << "BFS for arcGraph:" << std::endl;
        ArcGraph arcGraph(matrixGraph);
        BFS(arcGraph, [](int vertex) { std::cout << vertex << " "; });
        std::cout << std::endl;
        std::cout << std::endl;
    }
    {
        std::cout << "SECOND TEST" << std::endl;
        //Граф по слоям(начиная с 0):
        // 0
        // 2 7 5
        // 1 6 4 3
        ListGraph listGraph(8);
        listGraph.AddEdge(0, 2);
        listGraph.AddEdge(0, 7);
        listGraph.AddEdge(0, 5);
        listGraph.AddEdge(2, 0);
        listGraph.AddEdge(2, 6);
        listGraph.AddEdge(3, 5);
        listGraph.AddEdge(3, 4);
        listGraph.AddEdge(4, 3);
        listGraph.AddEdge(4, 7);
        listGraph.AddEdge(4, 6);
        listGraph.AddEdge(4, 5);
        listGraph.AddEdge(5, 0);
        listGraph.AddEdge(5, 3);
        listGraph.AddEdge(5, 4);
        listGraph.AddEdge(6, 2);
        listGraph.AddEdge(6, 4);
        listGraph.AddEdge(7, 0);
        listGraph.AddEdge(7, 1);
        listGraph.AddEdge(7, 4);
        std::cout << "BFS for listGraph:" << std::endl;
        BFS(listGraph, [](int vertex) { std::cout << vertex << " "; });
        std::cout << std::endl;
        std::cout << "BFS for matrixGraph:" << std::endl;
        MatrixGraph matrixGraph(listGraph);
        BFS(matrixGraph, [](int vertex) { std::cout << vertex << " "; });
        std::cout << std::endl;
        std::cout << "BFS for setGraph:" << std::endl;
        SetGraph setGraph(listGraph);
        BFS(setGraph, [](int vertex) { std::cout << vertex << " "; });
        std::cout << std::endl;
        std::cout << "BFS for arcGraph:" << std::endl;
        ArcGraph arcGraph(listGraph);
        BFS(arcGraph, [](int vertex) { std::cout << vertex << " "; });
        std::cout << std::endl;
        std::cout << std::endl;
    }
    return 0;
}