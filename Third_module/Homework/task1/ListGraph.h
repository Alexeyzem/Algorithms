#include "IGraph.h"
#include "vector"
#include "cassert"
#pragma once
class ListGraph : public IGraph{
public:
    ListGraph(int vertexCount);
    ListGraph(const IGraph& graph);
    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> adjLists;
    std::vector<std::vector<int>> prevAdjLists;

};
ListGraph::ListGraph(const IGraph& graph){
    adjLists.resize(graph.VerticesCount());
    prevAdjLists.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i){
        adjLists[i] = graph.GetNextVertices(i);
        prevAdjLists[i] = graph.GetPrevVertices(i);
    }
};
ListGraph::ListGraph(int vertexCount){
    assert(vertexCount > 0);
    adjLists.resize(vertexCount);
    prevAdjLists.resize(vertexCount);
};


void ListGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());
    adjLists[from].push_back(to);
    prevAdjLists[to].push_back(from);
};

int ListGraph::VerticesCount() const {
    return adjLists.size();
};
std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
};
std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return prevAdjLists[vertex];
};
