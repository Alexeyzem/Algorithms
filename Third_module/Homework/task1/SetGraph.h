#include "IGraph.h"
#include "vector"
#include "cassert"
#include "unordered_set"
#pragma once
class SetGraph : public IGraph{
public:
    SetGraph(int vertexCount);
    SetGraph(const IGraph& graph);
    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::unordered_set<int>> setsNext;
    std::vector<std::unordered_set<int>> setsPrev;
};
SetGraph::SetGraph(const IGraph& graph){
    setsNext.resize(graph.VerticesCount());
    setsPrev.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i){
        std::vector<int> vec = graph.GetNextVertices(i);
        for ( auto cur : vec) {
            setsNext[i].insert(cur);
        }
        vec = graph.GetPrevVertices(i);
        for (auto  cur : vec){
            setsPrev[i].insert(cur);
        }
    }
};
SetGraph::SetGraph(int vertexCount){
    assert(vertexCount > 0);
    setsNext.resize(vertexCount);
    setsPrev.resize(vertexCount);
};


void SetGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < setsNext.size());
    assert(to >= 0 && to < setsNext.size());
    setsNext[from].insert(to);
    setsPrev[to].insert(from);
};

int SetGraph::VerticesCount() const {
    return setsNext.size();
};
std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < setsNext.size());
    auto set = setsNext[vertex];
    std::vector<int> res;
    for ( auto cur : set){
        res.push_back(cur);
    }
    return res;
};
std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < setsNext.size());
    auto set = setsPrev[vertex];
    std::vector<int> res;
    for ( auto cur : set){
        res.push_back(cur);
    }
    return res;
};
