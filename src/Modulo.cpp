#include "Modulo.hpp"
#include "Divide.hpp"
#include "Subtract.hpp"
#include "Multiply.hpp"

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

    if (this->GetMostSigOp() == this->GetLeastSigOp()) {
        return std::make_unique<Constant>(0); // x % x = 0
    }

    // Simplify both sides if possible
    auto simplifiedDividend = this->GetMostSigOp().Simplify();
    auto simplifiedDivisor = this->GetLeastSigOp().Simplify();

    if (simplifiedDividend && simplifiedDivisor) {
        return std::make_unique<Modulo<Expression, Expression>>(*simplifiedDividend, *simplifiedDivisor);
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

    // Modulo differentiation uses the following general formula:
    // d(u % v)/dx = du - (v * floor(u/v) * dv + u * dv / v)
    auto quotient = std::make_unique<Divide>(u, v);
    auto floorQuotient = std::make_unique<Floor>(*quotient);
    auto quotientDerivative = quotient->Differentiate(differentiationVariable);
    auto term1 = std::make_unique<Multiply>(v, *floorQuotient);
    auto term2 = std::make_unique<Multiply>(term1, dv);
    auto result = std::make_unique<Subtract>(du, std::make_unique<Divide>(term2, v));

    return result;
}

auto Modulo<Expression, Expression>::Integrate(const Expression& integrationVariable) -> std::unique_ptr<Expression> {
    throw std::logic_error("Integration of modulo operation is not generally defined.");
}

auto Modulo<Expression, Expression>::Evaluate(const std::unordered_map<std::string, double>& variables) const -> double {
    double dividendValue = this->GetMostSigOp().Evaluate(variables);
    double divisorValue = this->GetLeastSigOp().Evaluate(variables);

    if (divisorValue == 0) {
        throw std::runtime_error("Division by zero in modulo operation.");
    }

    return std::fmod(dividendValue, divisorValue);
}

auto Modulo<Expression, Expression>::Compare(const Expression& other) const -> bool {
    if (!other.Is<Modulo>()) {
        return false;
    }

    const auto& otherModulo = static_cast<const Modulo<Expression, Expression>&>(other);
    return this->GetMostSigOp().Compare(otherModulo.GetMostSigOp()) &&
           this->GetLeastSigOp().Compare(otherModulo.GetLeastSigOp());
}

auto Modulo<Expression, Expression>::Clone() const -> std::unique_ptr<Expression> {
    return std::make_unique<Modulo<Expression, Expression>>(this->GetMostSigOp(), this->GetLeastSigOp());
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

