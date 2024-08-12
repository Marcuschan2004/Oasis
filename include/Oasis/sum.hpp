#ifndef OASIS_SUM_HPP
#define OASIS_SUM_HPP

#include "Expression.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace Oasis {

class Sum : public Expression {
public:
    Sum(std::unique_ptr<Expression> lowerBound, std::unique_ptr<Expression> upperBound, std::unique_ptr<Expression> function)
        : lowerBound_(std::move(lowerBound)), upperBound_(std::move(upperBound)), function_(std::move(function)) {}

    // Evaluate the summation by iterating from lowerBound to upperBound
    auto Evaluate(const std::unordered_map<std::string, double>& variables) const -> double override;

    // Simplify the summation, applying summation identities if possible
    auto Simplify() const -> std::unique_ptr<Expression> override;

    // Differentiate the summation with respect to a variable
    auto Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> override;

    // Clone the summation node
    auto Copy() const -> std::unique_ptr<Expression> override;

    auto Copy(tf::Subflow& subflow) const -> std::unique_ptr<Expression> override;

    // Compare two summation nodes for equivalence
    auto Equals(const Expression& other) const -> bool override;

    // Convert the summation to a string representation
    auto ToString() const -> std::string override;

    // Apply summation identities
    auto ApplySummationIdentities() const -> std::unique_ptr<Expression>;

    // Additional methods specific to Sum
    auto StructurallyEquivalent(const Expression& other) const -> bool override;
    auto StructurallyEquivalent(const Expression& other, tf::Subflow& subflow) const -> bool override;

private:
    std::unique_ptr<Expression> lowerBound_;
    std::unique_ptr<Expression> upperBound_;
    std::unique_ptr<Expression> function_;
};

} // namespace Oasis

#endif // OASIS_SUM_HPP



