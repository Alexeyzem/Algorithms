#include "ListGraph.h"
#include "iostream"
#include "queue"


int findCountMinPaths(IGraph& graph, int from, int to){
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::vector<bool> curVisited(graph.VerticesCount(), false);
    std::vector<int> countPaths(graph.VerticesCount(), 0);
    std::queue<int> bfsQ;
    bfsQ.push(from);
    bfsQ.push(-1);
    visited[from] = true;
    countPaths[from] = 1;
    while (bfsQ.size() > 1){
        int current = bfsQ.front();
        if (current == -1){
            bfsQ.pop();
            bfsQ.push(-1);
            curVisited = std::vector<bool>(graph.VerticesCount(), false);
            continue;
        }
        bfsQ.pop();
        std::vector<int> adjList = graph.GetNextVertices(current);
        for (int i : adjList){
           if (!visited[i]){
                visited[i] = true;
                curVisited[i] = true;
                bfsQ.push(i);
                std::vector<int> prev = graph.GetPrevVertices(i);
                for (int j : prev){
                    if (visited[j] && !curVisited[j]){
                        countPaths[i] += countPaths[j];
                    }
                }
           }
        }
    }
    return countPaths[to];
}

int main(){
    int countVertex;
    int countEdge;
    std::cin >> countVertex >> countEdge;
    ListGraph graph(countVertex);
    for (int i = 0; i < countEdge; ++i){
        int from, to;
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }
    int first, second;
    std::cin >> first >> second;
    std::cout << findCountMinPaths(graph, first, second);
}