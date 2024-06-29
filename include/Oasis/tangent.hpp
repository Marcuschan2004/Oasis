#ifndef OASIS_TAN_HPP
#define OASIS_TAN_HPP

#include "UnaryExpression.hpp"
#include "Real.hpp"

namespace Oasis {

class Tan : public UnaryExpression<Tan, Expression> {
public:
    Tan() = default;
    Tan(const Tan& other) = default;

    explicit Tan(const Expression& operand);

    [[nodiscard]] auto Simplify() const -> std::unique_ptr<Expression> final;
    auto Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> final;

    static auto Specialize(const Expression& other) -> std::unique_ptr<Tan>;
    static auto Specialize(const Expression& other, tf::Subflow& subflow) -> std::unique_ptr<Tan>;

    EXPRESSION_TYPE(Tan)
    EXPRESSION_CATEGORY(UnExp)
};

} // namespace Oasis

#endif // OASIS_TAN_HPP
