#include "Oasis/Tan.hpp"
#include <cmath>

namespace Oasis {

Tan::Tan(const Expression& operand) : UnaryExpression(operand) {}

auto Tan::Simplify() const -> std::unique_ptr<Expression> {
    auto simplifiedOperand = GetOperand().Simplify();
    if (auto realOperand = Real::Specialize(*simplifiedOperand)) {
        return std::make_unique<Real>(std::tan(realOperand->GetValue()));
    }
    return std::make_unique<Tan>(*simplifiedOperand);
}

auto Tan::Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> {
    std::unique_ptr<Expression> simplifiedOperand;
    subflow.emplace([this, &simplifiedOperand](tf::Subflow& sbf) {
        simplifiedOperand = GetOperand().Simplify(sbf);
    }).join();

    if (auto realOperand = Real::Specialize(*simplifiedOperand)) {
        return std::make_unique<Real>(std::tan(realOperand->GetValue()));
    }
    return std::make_unique<Tan>(*simplifiedOperand);
}

auto Tan::Specialize(const Expression& other) -> std::unique_ptr<Tan> {
    if (!other.Is<Tan>()) {
        return nullptr;
    }

    auto specialized = std::make_unique<Tan>();
    std::unique_ptr<Expression> otherGeneralized = other.Generalize();
    const auto& otherUnary = dynamic_cast<const Tan&>(*otherGeneralized);

    if (auto operand = Expression::Specialize(otherUnary.GetOperand()); operand != nullptr) {
        specialized->op = std::move(operand);
        return specialized;
    }

    return nullptr;
}

auto Tan::Specialize(const Expression& other, tf::Subflow& subflow) -> std::unique_ptr<Tan> {
    if (!other.Is<Tan>()) {
        return nullptr;
    }

    auto specialized = std::make_unique<Tan>();
    std::unique_ptr<Expression> otherGeneralized = other.Generalize(subflow);
    const auto& otherUnary = dynamic_cast<const Tan&>(*otherGeneralized);

    if (auto operand = Expression::Specialize(otherUnary.GetOperand(), subflow); operand != nullptr) {
        specialized->op = std::move(operand);
        return specialized;
    }

    return nullptr;
}

} // namespace Oasis
