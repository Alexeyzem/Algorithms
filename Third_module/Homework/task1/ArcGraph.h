#include "IGraph.h"
#include "vector"
#include "cassert"
#include "array"
#include "unordered_set"
#pragma once
class ArcGraph : public IGraph{
public:
    ArcGraph(int vertexCount);
    ArcGraph(const IGraph& graph);
    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::array<int, 2>> arc;
    int count;

};
ArcGraph::ArcGraph(const IGraph& graph){
    count = graph.VerticesCount();
    for (int i = 0; i < graph.VerticesCount(); ++i){
        std::vector<int> vec = graph.GetNextVertices(i);
        for (auto cur : vec){
         arc.push_back({i, cur});
        }
    }
};
ArcGraph::ArcGraph(int vertexCount){
    assert(vertexCount > 0);
    count = vertexCount;
};


void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < count);
    assert(to >= 0 && to < count);
    arc.push_back({from, to});
};

int ArcGraph::VerticesCount() const {
    return count;
};
std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < count);
    std::vector<int> res;
    for (auto cur : arc){
        if (cur[0] == vertex){
            res.push_back(cur[1]);
        }
    }
    return res;
};
std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < count);
    std::vector<int> res;
    for (auto cur : arc){
        if (cur[1] == vertex){
            res.push_back(cur[0]);
        }
    }
    return res;
};
