#ifndef OASIS_EXPRESSIONNODE_HPP
#define OASIS_EXPRESSIONNODE_HPP

#include "Expression.hpp"
#include <memory>

namespace Oasis {

class ExpressionNode {
public:
    explicit ExpressionNode(std::unique_ptr<Expression> expression)
        : expression_(std::move(expression)) {}

    [[nodiscard]] const Expression& GetExpression() const {
        return *expression_;
    }

    [[nodiscard]] auto Copy() const -> std::unique_ptr<ExpressionNode> {
        return std::make_unique<ExpressionNode>(expression_->Copy());
    }

    [[nodiscard]] auto ToString() const -> std::string {
        return expression_->ToString();
    }

private:
    std::unique_ptr<Expression> expression_;
};

} // namespace Oasis

#endif // OASIS_EXPRESSIONNODE_HPP
