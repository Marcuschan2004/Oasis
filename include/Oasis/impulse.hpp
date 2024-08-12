#ifndef OASIS_IMPULSE_HPP
#define OASIS_IMPULSE_HPP

#include "Expression.hpp"
#include <string>
#include <unordered_map>
#include <limits>

namespace Oasis {

class Impulse : public Expression {
public:
    Impulse() = default;

    [[nodiscard]] auto Evaluate(const std::unordered_map<std::string, double>& variables) const -> double override {
        auto t = variables.at("t");
        return t == 0 ? std::numeric_limits<double>::infinity() : 0.0;
    }

    [[nodiscard]] auto Simplify() const -> std::unique_ptr<Expression> override {
        return std::make_unique<Impulse>();
    }

    [[nodiscard]] auto Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> override {
        throw std::logic_error("Differentiation of Impulse function is not supported.");
    }

    [[nodiscard]] auto Copy() const -> std::unique_ptr<Expression> override {
        return std::make_unique<Impulse>();
    }

    [[nodiscard]] auto Equals(const Expression& other) const -> bool override {
        return other.Is<Impulse>();
    }

    [[nodiscard]] auto ToString() const -> std::string override {
        return "δ(t)";
    }
};

} // namespace Oasis

#endif // OASIS_IMPULSE_HPP

