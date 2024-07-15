
#ifndef OASIS_MODULO_HPP
#define OASIS_MODULO_HPP

#include "BinaryExpression.hpp"

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

    [[nodiscard]] auto Simplify() const -> std::unique_ptr<Expression> final {
        // Implement your simplification logic here
        return nullptr; // Placeholder
    }
    auto Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> final {
        // Implement your parallel simplification logic here
        return nullptr; // Placeholder
    }

    [[nodiscard]] auto Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> final {
        // Implement differentiation logic for modulo operation
        return nullptr; // Placeholder
    }

    [[nodiscard]] auto Integrate(const Expression& integrationVariable) -> std::unique_ptr<Expression> final {
        // Implement integration logic for modulo operation
        return nullptr; // Placeholder
    }

    static auto Specialize(const Expression& other) -> std::unique_ptr<Modulo> {
        if (!other.Is<Oasis::Modulo>()) {
            return nullptr;
        }
        auto specialized = std::make_unique<Modulo>();

        std::unique_ptr<Expression> otherGeneralized = other.Generalize();
        const auto& otherBinaryExpression = static_cast<const Modulo<Expression>&>(*otherGeneralized);

        if (otherBinaryExpression.HasMostSigOp()) {
            specialized->SetMostSigOp(*Expression::Specialize(otherBinaryExpression.GetMostSigOp()));
        }

        if (otherBinaryExpression.HasLeastSigOp()) {
            specialized->SetLeastSigOp(*Expression::Specialize(otherBinaryExpression.GetLeastSigOp()));
        }

        return specialized;
    }
    static auto Specialize(const Expression& other, tf::Subflow& subflow) -> std::unique_ptr<Modulo> {
        if (!other.Is<Oasis::Modulo>()) {
            return nullptr;
        }
        Modulo specialized;

        std::unique_ptr<Expression> otherGeneralized;
        tf::Task generalizeTask = subflow.emplace([&other, &otherGeneralized](tf::Subflow& sbf) {
            otherGeneralized = other.Generalize(sbf);
        });

        tf::Task mostSigOpTask = subflow.emplace([&specialized, &otherGeneralized](tf::Subflow& sbf) {
            const auto& otherBinaryExpression = dynamic_cast<const Modulo<Expression>&>(*otherGeneralized);
            if (otherBinaryExpression.HasMostSigOp()) {
                specialized.SetMostSigOp(*Expression::Specialize(otherBinaryExpression.GetMostSigOp(), sbf));
            }
        });

        mostSigOpTask.succeed(generalizeTask);

        tf::Task leastSigOpTask = subflow.emplace([&specialized, &otherGeneralized](tf::Subflow& sbf) {
            const auto& otherBinaryExpression = dynamic_cast<const Modulo<Expression>&>(*otherGeneralized);
            if (otherBinaryExpression.HasLeastSigOp()) {
                specialized.SetLeastSigOp(*Expression::Specialize(otherBinaryExpression.GetLeastSigOp(), sbf));
            }
        });

        leastSigOpTask.succeed(generalizeTask);

        subflow.join();

        return std::make_unique<Modulo>(specialized);
    }

    EXPRESSION_TYPE(Modulo)
    EXPRESSION_CATEGORY(BinExp)
};
/// @endcond

/**
 * The Modulo expression calculates the remainder of the division of two expressions.
 *
 * @tparam DividendT The expression to be divided.
 * @tparam DivisorT The expression to divide the dividend by.
 */
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
