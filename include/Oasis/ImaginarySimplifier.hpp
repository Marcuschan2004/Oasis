#ifndef OASIS_IMAGINARYSIMPLIFIER_HPP
#define OASIS_IMAGINARYSIMPLIFIER_HPP

#include "Expression.hpp"
#include "Exponent.hpp"
#include "Constant.hpp"
#include "Multiply.hpp"
#include "Imaginary.hpp"
#include <cmath>

namespace Oasis {

class ImaginarySimplifier {
public:
    static std::unique_ptr<Expression> Simplify(std::unique_ptr<Expression> expr) {
        // Check if the expression is an Exponent
        if (expr->GetType() == ExpressionType::Exponent) {
            const auto& exponentExpr = static_cast<const Exponent&>(*expr);

            // Check if the base is e
            if (exponentExpr.GetBase().GetType() == ExpressionType::Constant &&
                static_cast<const Constant&>(exponentExpr.GetBase()).GetValue() == M_E) {
                
                // Extract the exponent part and simplify it
                auto exponent = exponentExpr.GetExponent().Simplify();

                // Check if the exponent is of the form i * π * c
                if (IsImaginaryExponent(*exponent)) {
                    return SimplifyImaginaryExponent(std::move(exponent));
                }
            }
        }

        // Return the expression as-is if no simplification was applied
        return expr;
    }

private:
    static bool IsImaginaryExponent(const Expression& expr) {
        // Check if the expression is of the form i * π * c
        if (expr.GetType() == ExpressionType::Multiply) {
            const auto& multiplyExpr = static_cast<const Multiply&>(expr);

            // Check if one of the factors is an imaginary unit and another is π
            const auto& leftOperand = multiplyExpr.GetLeftOperand();
            const auto& rightOperand = multiplyExpr.GetRightOperand();

            bool hasImaginary = (leftOperand.GetType() == ExpressionType::Imaginary ||
                                 rightOperand.GetType() == ExpressionType::Imaginary);
            bool hasPi = (leftOperand.GetType() == ExpressionType::Constant &&
                          static_cast<const Constant&>(leftOperand).GetValue() == M_PI) ||
                         (rightOperand.GetType() == ExpressionType::Constant &&
                          static_cast<const Constant&>(rightOperand).GetValue() == M_PI);

            return hasImaginary && hasPi;
        }
        return false;
    }

    static std::unique_ptr<Expression> SimplifyImaginaryExponent(std::unique_ptr<Expression> expr) {
        // Assuming expr is of the form i * π * c where c is an integer
        const auto& multiplyExpr = static_cast<const Multiply&>(*expr);
        const auto& leftOperand = multiplyExpr.GetLeftOperand();
        const auto& rightOperand = multiplyExpr.GetRightOperand();

        // Extract the constant (c) from the multiplication
        double constantValue = 1.0;

        if (rightOperand.GetType() == ExpressionType::Constant) {
            constantValue = static_cast<const Constant&>(rightOperand).GetValue();
        } else if (leftOperand.GetType() == ExpressionType::Constant) {
            constantValue = static_cast<const Constant&>(leftOperand).GetValue();
        }

        // Simplify based on the value of the constant (c)
        double angle = constantValue * M_PI;
        double angleMod = fmod(angle, 2 * M_PI);

        if (angleMod == 0) {
            return std::make_unique<Constant>(1.0);
        } else if (angleMod == M_PI) {
            return std::make_unique<Constant>(-1.0);
        } else if (angleMod == M_PI / 2) {
            return std::make_unique<Imaginary>();
        } else if (angleMod == 3 * M_PI / 2) {
            return std::make_unique<Multiply>(std::make_unique<Constant>(-1.0), std::make_unique<Imaginary>());
        }

        // If none of the special cases apply, return the original expression
        return std::make_unique<Exponent>(
            std::make_unique<Constant>(M_E),
            std::make_unique<Multiply>(
                std::make_unique<Imaginary>(),
                std::make_unique<Constant>(constantValue * M_PI)
            )
        );
    }
};

} // namespace Oasis

#endif // OASIS_IMAGINARYSIMPLIFIER_HPP
