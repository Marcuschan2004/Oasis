#ifndef OASIS_SIMPLIFICATIONGRAPH_HPP
#define OASIS_SIMPLIFICATIONGRAPH_HPP

#include "ExpressionNode.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <memory>

namespace Oasis {

class SimplificationGraph {
public:
    using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, std::shared_ptr<ExpressionNode>>;
    using Vertex = boost::graph_traits<Graph>::vertex_descriptor;

    SimplificationGraph() {
        graph_ = std::make_unique<Graph>();
    }

    Vertex AddExpressionNode(std::unique_ptr<Expression> expression) {
        auto node = std::make_shared<ExpressionNode>(std::move(expression));
        auto vertex = boost::add_vertex(node, *graph_);
        return vertex;
    }

    void AddSimplificationStep(Vertex from, Vertex to) {
        boost::add_edge(from, to, *graph_);
    }

    [[nodiscard]] auto GetGraph() const -> const Graph& {
        return *graph_;
    }

    void SaveGraphToFile(const std::string& filename) const {
        std::ofstream file(filename);
        boost::write_graphviz(file, *graph_, boost::make_label_writer(boost::get(boost::vertex_bundle, *graph_)));
    }

private:
    std::unique_ptr<Graph> graph_;
};

} // namespace Oasis

#endif // OASIS_SIMPLIFICATIONGRAPH_HPP
