
#include "Modulo.hpp"

namespace Oasis {

Modulo<Expression, Expression>::Modulo(const Expression& dividend, const Expression& divisor)
    : BinaryExpression(dividend, divisor) {}

auto Modulo<Expression, Expression>::Simplify() const -> std::unique_ptr<Expression> {
    // Simplification logic for Modulo operation
    // Placeholder implementation
    return nullptr;
}

auto Modulo<Expression, Expression>::Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> {
    // Parallel simplification logic for Modulo operation
    // Placeholder implementation
    return nullptr;
}

auto Modulo<Expression, Expression>::Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> {
    // Differentiation logic for Modulo operation
    // Placeholder implementation
    return nullptr;
}

auto Modulo<Expression, Expression>::Integrate(const Expression& integrationVariable) -> std::unique_ptr<Expression> {
    // Integration logic for Modulo operation
    // Placeholder implementation
    return nullptr;
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
