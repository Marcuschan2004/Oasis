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
        if (this->GetLeastSigOp().IsZero()) {
            throw std::runtime_error("Division by zero in modulo operation.");
        }

        if (this->GetMostSigOp().IsZero()) {
            return std::make_unique<Constant>(0);
        }

        if (this->GetLeastSigOp().IsOne()) {
            return std::make_unique<Constant>(0);
        }

        return nullptr; // No further simplification possible
    }

    auto Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> final {
        auto result = std::make_unique<Modulo>(*this);

        subflow.emplace([&]() {
            auto simplifiedDividend = this->GetMostSigOp().Simplify(subflow);
            result->SetMostSigOp(*simplifiedDividend);
        });

        subflow.emplace([&]() {
            auto simplifiedDivisor = this->GetLeastSigOp().Simplify(subflow);
            result->SetLeastSigOp(*simplifiedDivisor);
        });

        subflow.join();

        return result->Simplify(); // Simplify again after parallel tasks
    }

    [[nodiscard]] auto Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> final {
        auto u = this->GetMostSigOp();
        auto v = this->GetLeastSigOp();
        auto du = u.Differentiate(differentiationVariable);
        auto dv = v.Differentiate(differentiationVariable);

        auto quotient = std::make_unique<Divide>(u, v);
        auto quotientDerivative = quotient->Differentiate(differentiationVariable);

        auto result = std::make_unique<Subtract>(du, std::make_unique<Multiply>(dv, quotientDerivative));

        return result;
    }

    [[nodiscard]] auto Integrate(const Expression& integrationVariable) -> std::unique_ptr<Expression> final {
        throw std::logic_error("Integration of modulo operation is not generally defined.");
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
