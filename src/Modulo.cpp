#include "Modulo.hpp"

namespace Oasis {

Modulo<Expression, Expression>::Modulo(const Expression& dividend, const Expression& divisor)
    : BinaryExpression(dividend, divisor) {}

auto Modulo<Expression, Expression>::Simplify() const -> std::unique_ptr<Expression> {
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

auto Modulo<Expression, Expression>::Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> {
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

auto Modulo<Expression, Expression>::Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> {
    auto u = this->GetMostSigOp();
    auto v = this->GetLeastSigOp();
    auto du = u.Differentiate(differentiationVariable);
    auto dv = v.Differentiate(differentiationVariable);

    auto quotient = std::make_unique<Divide>(u, v);
    auto quotientDerivative = quotient->Differentiate(differentiationVariable);

    auto result = std::make_unique<Subtract>(du, std::make_unique<Multiply>(dv, quotientDerivative));

    return result;
}

auto Modulo<Expression, Expression>::Integrate(const Expression& integrationVariable) -> std::unique_ptr<Expression> {
    throw std::logic_error("Integration of modulo operation is not generally defined.");
}

auto Modulo<Expression, Expression>::Specialize(const Expression& other) -> std::unique_ptr<Modulo> {
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

auto Modulo<Expression, Expression>::Specialize(const Expression& other, tf::Subflow& subflow) -> std::unique_ptr<Modulo> {
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

} // namespace Oasis
