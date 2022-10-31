// Implemented by @kitanoyoru

#ifndef __KITA_GRAPH__
#define __KITA_GRAPH__

#include <vector>
#include <stdexcept>
#include <unordered_map>

template<class TV, class EI, class TE>
class UndirectedGraph {
public:
  typedef int vertex_id_type;
  typedef TV vertex_data_type;
  typedef EI edge_id_type;
  typedef TE edge_data_type;
private:
  typedef UndirectedGraph<vertex_data_type, edge_id_type, edge_data_type> graph_type;
  typedef std::unordered_map<vertex_id_type, vertex_data_type> vertex_container;
  typedef std::unordered_map<edge_id_type, edge_data_type> edge_container;
  typedef std::vector<vertex_id_type> adj_list; // 0 1 1 0 1 ...
  typedef std::unordered_map<vertex_id_type, adj_list> adj_container;

  vertex_container vertices;
  edge_container edges;
  adj_container adj;  // adj matrix
public:
  typedef typename vertex_container::iterator graph_vertex_iterator;
  typedef typename vertex_container::const_iterator graph_vertex_const_iterator;
  typedef typename edge_container::iterator graph_edge_iterator;
  typedef typename edge_container::const_iterator graph_edge_const_iterator;
  typedef typename adj_list::iterator graph_adj_iterator;
  typedef typename adj_list::const_iterator graph_adj_const_iterator;

  edge_id_type make_edge_id(const vertex_id_type& a, const vertex_id_type& b) {
    return edge_id_type(a, b);
  } 

  bool is_empty() const {
    return vertices.empty();
  }
  
  int n_vertices() const {
    return vertices.size();
  }
  int n_edges() const {
    return edges.size();
  }
  
  vertex_data_type& at_vertex(const vertex_id_type& id) {
    return vertices.at(id);
  }
  const vertex_data_type& at_vertex(const vertex_id_type& id) const {
    return vertices.at(id);
  }
  edge_data_type& at_edge(const edge_id_type& id) {
    return edges.at(id);
  }
  const edge_data_type& at_edge(const edge_id_type& id) const {
    return edges.at(id);
  }
  
  graph_vertex_iterator find_vertex(const vertex_id_type& id) {
    return vertices.find(id);
  }
  graph_vertex_const_iterator find_vertex(const vertex_id_type& id) const {
    return vertices.find(id);    
  }
  graph_edge_iterator find_edge(const edge_id_type& id) {
    return edges.find(id);
  }
  graph_edge_const_iterator find_edge(const edge_id_type& id) const {
    return edges.find(id);   
  }
  
  graph_vertex_iterator begin_vertices() {
    return vertices.begin();
  }
  graph_vertex_const_iterator begin_vertices() const {
    return vertices.begin();
  }
  graph_edge_iterator begin_edges() {
    return edges.begin();
  }
  graph_edge_const_iterator begin_edges() const {
    return edges.begin();
  }

  graph_vertex_iterator end_vertices() {
    return vertices.end();
  }
  graph_vertex_const_iterator end_vertices() const {
    return vertices.end();
  }
  graph_edge_iterator end_edges() {
    return edges.end();
  }
  graph_edge_const_iterator end_edges() const {
    return edges.end();
  }
  graph_adj_iterator begin_adj(const vertex_id_type& vertex_id) {
    return adj.at(vertex_id).begin();
  }
  graph_adj_const_iterator begin_adj(const vertex_id_type& vertex_id) const {
    return adj.at(vertex_id).begin();    
  }
  graph_adj_iterator end_adj(const vertex_id_type& vertex_id) {
    return adj.at(vertex_id).end();
  }
  graph_adj_const_iterator end_adj(const vertex_id_type& vertex_id) const {
    return adj.at(vertex_id).end();    
  }

  void clear() {
    vertices.clear();
    edges.clear();
    adj.clear();
  };

  std::pair<graph_vertex_iterator, bool> insert_vertex(const vertex_id_type& vertex_id, const vertex_data_type& vertex_data) {
    for (int i = 0; i < this->n_vertices(); i++) {
      adj[i].push_back(0);
    }
    auto pair = vertices.emplace(vertex_id, vertex_data);
    adj.emplace(vertex_id, adj_list(this->n_vertices(), 0));
    return pair;
  }
  
  std::pair<graph_edge_iterator, bool> insert_edge(const vertex_id_type& v1, const vertex_id_type& v2, edge_data_type& data) {
    if (vertices.count(v1) == 0 || vertices.count(v2) == 0) {
      return std::make_pair(edges.end(), false);
    }

    if (v1 == v2) {
      throw std::invalid_argument("");
    }

    auto pair = edges.emplace(make_edge_id(v1, v2), data);

    if (pair.second) {
      adj[v1][v2] = 1;
      if (v1 != v2) {
        adj[v2][v1] = 1;
      }
    }

    return pair;
  }

  bool delete_vertex(const vertex_id_type& vertex_id) {
    auto count = vertices.erase(vertex_id);
    if (count == 0) {
      return false;
    }
    
    auto& vertex_id_list = adj.at(vertex_id);
    for (auto it = vertex_id_list.begin(); it != vertex_id_list.end(); it++) {
      const auto other_id = *it;

      edges.erase(make_edge_id(vertex_id, other_id));
      
      auto& other_id_list = adj.at(other_id);
      for (auto it = other_id_list.begin(); it != other_id_list.end(); it++) {
        if (*it == vertex_id) {
          other_id_list.erase(it);
          break;
        }
      }
    }

    adj.erase(vertex_id);

    return true;
  } 

  bool delete_edge(const edge_id_type& edge) {
    auto count = edges.erase(edge);
    if (count == 0) {
      return false;
    }

    adj[edge.a][edge.b] = 0;
    adj[edge.b][edge.a] = 0;

    return true;
  }
};

#endif