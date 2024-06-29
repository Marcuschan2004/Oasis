#ifndef OASIS_ARCTAN_HPP
#define OASIS_ARCTAN_HPP

#include "UnaryExpression.hpp"
#include "Real.hpp"

namespace Oasis {

class ArcTan : public UnaryExpression<ArcTan, Expression> {
public:
    ArcTan() = default;
    ArcTan(const ArcTan& other) = default;

    explicit ArcTan(const Expression& operand);

    [[nodiscard]] auto Simplify() const -> std::unique_ptr<Expression> final;
    auto Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> final;

    static auto Specialize(const Expression& other) -> std::unique_ptr<ArcTan>;
    static auto Specialize(const Expression& other, tf::Subflow& subflow) -> std::unique_ptr<ArcTan>;

    EXPRESSION_TYPE(ArcTan)
    EXPRESSION_CATEGORY(UnExp)
};

} // namespace Oasis

#endif // OASIS_ARCTAN_HPP
