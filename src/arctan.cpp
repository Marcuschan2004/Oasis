#include "Oasis/ArcTan.hpp"
#include <cmath>

namespace Oasis {

ArcTan::ArcTan(const Expression& operand) : UnaryExpression(operand) {}

auto ArcTan::Simplify() const -> std::unique_ptr<Expression> {
    auto simplifiedOperand = GetOperand().Simplify();
    if (auto realOperand = Real::Specialize(*simplifiedOperand)) {
        return std::make_unique<Real>(std::atan(realOperand->GetValue()));
    }
    return std::make_unique<ArcTan>(*simplifiedOperand);
}

auto ArcTan::Simplify(tf::Subflow& subflow) const -> std::unique_ptr<Expression> {
    std::unique_ptr<Expression> simplifiedOperand;
    subflow.emplace([this, &simplifiedOperand](tf::Subflow& sbf) {
        simplifiedOperand = GetOperand().Simplify(sbf);
    }).join();

    if (auto realOperand = Real::Specialize(*simplifiedOperand)) {
        return std::make_unique<Real>(std::atan(realOperand->GetValue()));
    }
    return std::make_unique<ArcTan>(*simplifiedOperand);
}

auto ArcTan::Specialize(const Expression& other) -> std::unique_ptr<ArcTan> {
    if (!other.Is<ArcTan>()) {
        return nullptr;
    }

    auto specialized = std::make_unique<ArcTan>();
    std::unique_ptr<Expression> otherGeneralized = other.Generalize();
    const auto& otherUnary = dynamic_cast<const ArcTan&>(*otherGeneralized);

    if (auto operand = Expression::Specialize(otherUnary.GetOperand()); operand != nullptr) {
        specialized->op = std::move(operand);
        return specialized;
    }

    return nullptr;
}

auto ArcTan::Specialize(const Expression& other, tf::Subflow& subflow) -> std::unique_ptr<ArcTan> {
    if (!other.Is<ArcTan>()) {
        return nullptr;
    }

    auto specialized = std::make_unique<ArcTan>();
    std::unique_ptr<Expression> otherGeneralized = other.Generalize(subflow);
    const auto& otherUnary = dynamic_cast<const ArcTan&>(*otherGeneralized);

    if (auto operand = Expression::Specialize(otherUnary.GetOperand(), subflow); operand != nullptr) {
        specialized->op = std::move(operand);
        return specialized;
    }

    return nullptr;
}

} // namespace Oasis
