#ifndef OASIS_MODULO_HPP
#define OASIS_MODULO_HPP

#include "BinaryExpression.hpp"
#include "Constant.hpp"

namespace Oasis {

template <IExpression DividendT, IExpression DivisorT>
class Modulo;

/// @cond
template <>
class Modulo<Expression, Expression> : public BinaryExpression<Modulo> {
public:
    Modulo() = default;
    Modulo(const Modulo<Expression, Expression>& other) = default;

    Modulo(const Expression& dividend, const Expression& divisor)
        : BinaryExpression(dividend, divisor) {}

    [[nodiscard]] auto Simplify() const -> std::unique_ptr<Expression> final;
    auto Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> final;
    [[nodiscard]] auto Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> final;
    [[nodiscard]] auto Integrate(const Expression& integrationVariable) -> std::unique_ptr<Expression> final;

    [[nodiscard]] auto Evaluate(const std::unordered_map<std::string, double>& variables) const -> double final;
    [[nodiscard]] auto Compare(const Expression& other) const -> bool final;
    [[nodiscard]] auto Clone() const -> std::unique_ptr<Expression> final;

    static auto Specialize(const Expression& other) -> std::unique_ptr<Modulo>;
    static auto Specialize(const Expression& other, tf::Subflow& subflow) -> std::unique_ptr<Modulo>;

    EXPRESSION_TYPE(Modulo)
    EXPRESSION_CATEGORY(BinExp)
};
/// @endcond

template <IExpression DividendT = Expression, IExpression DivisorT = DividendT>
class Modulo : public BinaryExpression<Modulo, DividendT, DivisorT> {
public:
    Modulo() = default;
    Modulo(const Modulo<DividendT, DivisorT>& other)
        : BinaryExpression<Modulo, DividendT, DivisorT>(other) {}

    Modulo(const DividendT& dividend, const DivisorT& divisor)
        : BinaryExpression<Modulo, DividendT, DivisorT>(dividend, divisor) {}

    IMPL_SPECIALIZE(Modulo, DividendT, DivisorT)

    auto operator=(const Modulo& other) -> Modulo& = default;

    EXPRESSION_TYPE(Modulo)
    EXPRESSION_CATEGORY(BinExp)
};

} // Oasis

#endif // OASIS_MODULO_HPP

