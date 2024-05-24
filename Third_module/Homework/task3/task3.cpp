#include "iostream"
#include "set"
#include "vector"
#include "cassert"


#define MAX 2147483647
struct Vertex{
    int pos;
    int distance;
};

struct IGraph{
    virtual ~IGraph() = default;
    virtual void AddEdge(int from, int to, int distance) = 0;
    virtual int VertexCount() const = 0;
    virtual int VerticesCount() const = 0;

    virtual std::vector<Vertex> GetNextVertices(int vertex) const = 0;
    virtual std::vector<Vertex> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph{
public:
    ListGraph(int vertexCount);
    ListGraph(const IGraph& graph);
    ~ListGraph() override{}
    virtual void AddEdge(int from, int to, int distance) override;
   int VertexCount() const override;
    virtual int VerticesCount() const override;
    virtual std::vector<Vertex> GetNextVertices(int vertex) const override;
    virtual std::vector<Vertex> GetPrevVertices(int vertex) const override;

private:
    int vertexCount;
    std::vector<std::vector<Vertex>> adjLists;
    std::vector<std::vector<Vertex>> prevAdjLists;

};
ListGraph::ListGraph(const IGraph& graph){
    adjLists.resize(graph.VerticesCount());
    prevAdjLists.resize(graph.VerticesCount());
    for (int i = 0; i < graph.VerticesCount(); ++i){
        adjLists[i] = graph.GetNextVertices(i);
        prevAdjLists[i] = graph.GetPrevVertices(i);
    }
    vertexCount = graph.VertexCount();
};
ListGraph::ListGraph(int _vertexCount){
    assert(_vertexCount > 0);
    adjLists.resize(_vertexCount);
    prevAdjLists.resize(_vertexCount);
    vertexCount = _vertexCount;
};

int ListGraph::VertexCount() const {
    return vertexCount;
}
void ListGraph::AddEdge(int from, int to, int distance) {
    assert(from >= 0 && from < adjLists.size());
    assert(to >= 0 && to < adjLists.size());

    adjLists[from].push_back({to, distance});
    prevAdjLists[to].push_back({from, distance});
};

int ListGraph::VerticesCount() const {
    return adjLists.size();
};
std::vector<Vertex> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return adjLists[vertex];
};
std::vector<Vertex> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < adjLists.size());
    return prevAdjLists[vertex];
};
struct Less{
    bool operator()(const Vertex l, const Vertex r) const{
        return l.distance <= r.distance;
    }
};

int Deikstra(IGraph& graph, int from, int to){
    std::vector<int> distance(graph.VertexCount(), MAX);
    distance[from] = 0;
    std::set<Vertex, Less> queue;
    queue.insert({from, 0});
    std::set<Vertex>::iterator it;
    while (!queue.empty()){
        it = queue.begin();
        Vertex u = *it;
        queue.erase(it);
        for (auto v:graph.GetNextVertices(u.pos)){
            if(distance[v.pos]==MAX){
                distance[v.pos] = distance[u.pos] + v.distance;
                queue.insert({v.pos, distance[v.pos]});
            } else if(distance[v.pos] > distance[u.pos] + v.distance){
                queue.erase({v.pos, distance[v.pos]});
                distance[v.pos] = distance[u.pos] + v.distance;
                queue.insert({v.pos, distance[v.pos]});
            }
        }
    }
    return distance[to];
}

int main(){
    int VertexCount, VerticesCount;
    std::cin >> VertexCount >> VerticesCount;
    ListGraph graph(VertexCount);
    for (int i = 0; i < VerticesCount; i++){
        int from, to, price;
        std::cin >> from >> to >> price;
        graph.AddEdge(from, to, price);
        graph.AddEdge(to, from, price);
    }
    int start, to;
    std:: cin >> start >> to;
    std::cout << Deikstra(graph, start, to);
    return 0;
}