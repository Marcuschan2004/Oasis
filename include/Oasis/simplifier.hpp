#ifndef OASIS_SIMPLIFIER_HPP
#define OASIS_SIMPLIFIER_HPP

#include "SimplificationGraph.hpp"
#include "Expression.hpp"

namespace Oasis {

class Simplifier {
public:
    Simplifier() : simplificationGraph_(std::make_unique<SimplificationGraph>()) {
        // Initialize the graph with an initial state if necessary
    }

    auto Simplify(std::unique_ptr<Expression> expression) -> std::unique_ptr<Expression> {
        // Start by adding the initial expression to the graph
        auto currentVertex = simplificationGraph_->AddExpressionNode(expression->Copy());

        while (true) {
            auto simplifiedExpression = expression->Simplify();
            if (expression->Equals(*simplifiedExpression)) {
                break;
            }

            // Add the new simplified state to the graph
            auto newVertex = simplificationGraph_->AddExpressionNode(simplifiedExpression->Copy());
            simplificationGraph_->AddSimplificationStep(currentVertex, newVertex);

            // Move to the new state
            expression = std::move(simplifiedExpression);
            currentVertex = newVertex;
        }

        return expression;
    }

    [[nodiscard]] auto GetGraph() const -> const SimplificationGraph& {
        return *simplificationGraph_;
    }

private:
    std::unique_ptr<SimplificationGraph> simplificationGraph_;
};

} // namespace Oasis

#endif // OASIS_SIMPLIFIER_HPP
