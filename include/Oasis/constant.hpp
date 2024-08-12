#ifndef OASIS_CONSTANT_HPP
#define OASIS_CONSTANT_HPP

#include "Expression.hpp"
#include "Multiply.hpp"
#include "Variable.hpp"
#include <string>
#include <unordered_map>
#include <cmath>

namespace Oasis {

class Constant : public Expression {
public:
    // Constructor to initialize the constant with a value and a symbolic name
    Constant(double value, std::string name)
        : value_(value), name_(std::move(name)) {}

    // Evaluate the constant (returns the value)
    [[nodiscard]] auto Evaluate(const std::unordered_map<std::string, double>& variables) const -> double final {
        return value_;
    }

    // Simplification returns a copy of the constant itself
    [[nodiscard]] auto Simplify() const -> std::unique_ptr<Expression> final {
        return std::make_unique<Constant>(*this);
    }

    // Parallel simplification also returns a copy of the constant itself
    auto Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> final {
        return std::make_unique<Constant>(*this);
    }

    // Differentiation of a constant is always zero
    [[nodiscard]] auto Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> final {
        return std::make_unique<Constant>(0.0, "0");
    }

    // Integration of a constant with respect to a variable
    [[nodiscard]] auto Integrate(const Expression& integrationVariable) -> std::unique_ptr<Expression> final {
        return std::make_unique<Multiply>(integrationVariable.Clone(), std::make_unique<Constant>(*this));
    }

    // Cloning the constant (returns a copy of the constant)
    [[nodiscard]] auto Clone() const -> std::unique_ptr<Expression> final {
        return std::make_unique<Constant>(*this);
    }

    // Comparing this constant with another expression
    [[nodiscard]] auto Compare(const Expression& other) const -> bool final {
        if (!other.Is<Constant>()) {
            return false;
        }
        const auto& otherConstant = static_cast<const Constant&>(other);
        return this->value_ == otherConstant.value_;
    }

    // Converts the constant to its symbolic name (e.g., "π" or "e")
    [[nodiscard]] auto ToString() const -> std::string final {
        return name_;
    }

    // Static method to create a constant for π (Pi)
    static auto Pi() -> std::unique_ptr<Constant> {
        return std::make_unique<Constant>(3.141592653589793, "π");
    }

    // Static method to create a constant for e (Euler's number)
    static auto E() -> std::unique_ptr<Constant> {
        return std::make_unique<Constant>(2.718281828459045, "e");
    }

    // Static method to create a constant for the Golden Ratio (φ)
    static auto GoldenRatio() -> std::unique_ptr<Constant> {
        return std::make_unique<Constant>(1.618033988749895, "φ");
    }

    // Static method to create a constant for the square root of 2
    static auto Sqrt2() -> std::unique_ptr<Constant> {
        return std::make_unique<Constant>(1.4142135623730951, "√2");
    }

    // Static method to create a constant for the natural logarithm of 2 (ln(2))
    static auto Ln2() -> std::unique_ptr<Constant> {
        return std::make_unique<Constant>(0.6931471805599453, "ln(2)");
    }

    // Static method to create a constant for the natural logarithm of 10 (ln(10))
    static auto Ln10() -> std::unique_ptr<Constant> {
        return std::make_unique<Constant>(2.302585092994046, "ln(10)");
    }

private:
    double value_;   // The numerical value of the constant
    std::string name_; // The symbolic name of the constant (e.g., "π" for pi)
};

} // namespace Oasis

#endif // OASIS_CONSTANT_HPP
