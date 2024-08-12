#ifndef OASIS_LAPLACE_HPP
#define OASIS_LAPLACE_HPP

#include "BoundedUnaryExpression.hpp"
#include <string>
#include <unordered_map>

namespace Oasis {

template <IExpression OperandT, IExpression LowerBoundT = Expression, IExpression UpperBoundT = LowerBoundT>
class Laplace : public BoundedUnaryExpression<Laplace, OperandT, LowerBoundT, UpperBoundT> {
public:
    Laplace() = default;
    Laplace(const OperandT& operand, const LowerBoundT& lowerBound, const UpperBoundT& upperBound)
        : BoundedUnaryExpression<Laplace, OperandT, LowerBoundT, UpperBoundT>(operand, lowerBound, upperBound) {}

    [[nodiscard]] auto Evaluate(const std::unordered_map<std::string, double>& variables) const -> double override {
        // Simplified Laplace transform evaluation logic for demonstration purposes
        return 1 / (variables.at("s") - this->GetOperand().Evaluate(variables));
    }

    [[nodiscard]] auto ToString() const -> std::string override {
        return "Laplace(" + this->GetOperand().ToString() + ")";
    }

    DECL_SPECIALIZE(Laplace)
    EXPRESSION_TYPE(Laplace)
    EXPRESSION_CATEGORY(UnExp)
};

} // namespace Oasis

#endif // OASIS_LAPLACE_HPP
