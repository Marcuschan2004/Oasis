#ifndef OASIS_CONSTANT_HPP
#define OASIS_CONSTANT_HPP

#include "Expression.hpp"
#include "Multiply.hpp"
#include "Variable.hpp"
#include <string>
#include <unordered_map>

namespace Oasis {

class Constant : public Expression {
public:
    Constant(double value, std::string name)
        : value_(value), name_(std::move(name)) {}

    [[nodiscard]] auto Evaluate(const std::unordered_map<std::string, double>& variables) const -> double final {
        return value_;
    }

    [[nodiscard]] auto Simplify() const -> std::unique_ptr<Expression> final {
        return std::make_unique<Constant>(*this);
    }

    auto Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> final {
        return std::make_unique<Constant>(*this);
    }

    [[nodiscard]] auto Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> final {
        return std::make_unique<Constant>(0.0, "0");
    }

    [[nodiscard]] auto Integrate(const Expression& integrationVariable) -> std::unique_ptr<Expression> final {
        return std::make_unique<Multiply>(integrationVariable.Clone(), std::make_unique<Constant>(*this));
    }

    [[nodiscard]] auto Clone() const -> std::unique_ptr<Expression> final {
        return std::make_unique<Constant>(*this);
    }

    [[nodiscard]] auto Compare(const Expression& other) const -> bool final {
        if (!other.Is<Constant>()) {
            return false;
        }
        const auto& otherConstant = static_cast<const Constant&>(other);
        return this->value_ == otherConstant.value_;
    }

    [[nodiscard]] auto ToString() const -> std::string final {
        return name_;
    }

    static auto Pi() -> std::unique_ptr<Constant> {
        return std::make_unique<Constant>(3.141592653589793, "π");
    }

    static auto E() -> std::unique_ptr<Constant> {
        return std::make_unique<Constant>(2.718281828459045, "e");
    }

private:
    double value_;
    std::string name_;
};

} 

#endif // OASIS_CONSTANT_HPP
