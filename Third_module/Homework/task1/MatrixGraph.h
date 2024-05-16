#include "IGraph.h"
#include "vector"
#include "cassert"
#pragma once
class MatrixGraph : public IGraph{
public:
    MatrixGraph(int vertexCount);
    MatrixGraph(const IGraph& graph);
    virtual void AddEdge(int from, int to) override;

    virtual int VerticesCount() const override;

    virtual std::vector<int> GetNextVertices(int vertex) const override;
    virtual std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> matrix;

};
MatrixGraph::MatrixGraph(const IGraph& graph){
    matrix.resize(graph.VerticesCount(), std::vector<int>(graph.VerticesCount(), 0));
    for (int i = 0; i < graph.VerticesCount(); ++i){
        std::vector<int> next = graph.GetNextVertices(i);
        for (int cur : next){
            matrix[i][cur] = 1;
        }
    }
};
MatrixGraph::MatrixGraph(int vertexCount){
    assert(vertexCount > 0);
    matrix.resize(vertexCount, std::vector<int>(vertexCount, 0));
};


void MatrixGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < matrix.size());
    assert(to >= 0 && to < matrix.size());
    matrix[from][to] = 1;
};

int MatrixGraph::VerticesCount() const {
    return matrix.size();
};
std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < matrix.size());
    std::vector<int> res;
    for (int i = 0; i < matrix.size(); ++i){
        if (matrix[vertex][i] == 1){
            res.push_back(i);
        }
    }
    return res;
};
std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < matrix.size());
    std::vector<int> res;
    for (int i = 0; i < matrix.size(); ++i){
        if (matrix[i][vertex] == 1){
            res.push_back(i);
        }
    }
    return res;
};
