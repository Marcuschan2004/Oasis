#ifndef OASIS_FOURIER_HPP
#define OASIS_FOURIER_HPP

#include "BoundedUnaryExpression.hpp"
#include <string>
#include <unordered_map>
#include <tinyxml2.h>

namespace Oasis {

template <IExpression OperandT, IExpression LowerBoundT = Expression, IExpression UpperBoundT = LowerBoundT>
class Fourier : public BoundedUnaryExpression<Fourier, OperandT, LowerBoundT, UpperBoundT> {
public:
    Fourier() = default;
    Fourier(const OperandT& operand, const LowerBoundT& lowerBound, const UpperBoundT& upperBound)
        : BoundedUnaryExpression<Fourier, OperandT, LowerBoundT, UpperBoundT>(operand, lowerBound, upperBound) {}

    [[nodiscard]] auto Evaluate(const std::unordered_map<std::string, double>& variables) const -> double override {
        // Simplified Fourier transform evaluation logic for demonstration purposes
        double operandValue = this->GetOperand().Evaluate(variables);
        double frequency = variables.at("frequency"); // Example usage of frequency from variables map

        // Apply the Fourier transform (this is highly simplified and context-dependent)
        return operandValue * std::exp(-2 * M_PI * frequency);
    }

    [[nodiscard]] auto Simplify() const -> std::unique_ptr<Expression> override {
        // Example: Simplify the Fourier transform of a constant or known function
        if (this->GetOperand().Is<Constant>()) {
            return std::make_unique<Constant>(0.0); // Fourier transform of a constant can be simplified
        }

        // Further simplification logic based on the operand type and the bounds
        return this->Copy();
    }

    [[nodiscard]] auto ToString() const -> std::string override {
        return "Fourier(" + this->GetOperand().ToString() + ")";
    }

    void SerializeToMathML(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument& doc) const override {
        auto applyElement = doc.NewElement("apply");

        // Create MathML elements
        auto fourierElement = doc.NewElement("fourier");
        applyElement->InsertEndChild(fourierElement);

        // Serialize the operand
        this->GetOperand().SerializeToMathML(applyElement, doc);

        // Serialize the bounds if they exist
        if (this->HasLowerBound()) {
            auto lowerBoundElement = doc.NewElement("lowlimit");
            this->GetLowerBound().SerializeToMathML(lowerBoundElement, doc);
            applyElement->InsertEndChild(lowerBoundElement);
        }

        if (this->HasUpperBound()) {
            auto upperBoundElement = doc.NewElement("uplimit");
            this->GetUpperBound().SerializeToMathML(upperBoundElement, doc);
            applyElement->InsertEndChild(upperBoundElement);
        }

        // Append to parent
        parent->InsertEndChild(applyElement);
    }

    static std::unique_ptr<Expression> DeserializeFromMathML(const tinyxml2::XMLElement* element) {
        if (std::string(element->Name()) == "apply") {
            auto firstChild = element->FirstChildElement();
            if (firstChild && std::string(firstChild->Name()) == "fourier") {
                auto operandElement = firstChild->NextSiblingElement();
                auto operand = Expression::DeserializeFromMathML(operandElement);

                std::unique_ptr<Expression> lowerBound, upperBound;

                auto nextElement = operandElement->NextSiblingElement();
                if (nextElement && std::string(nextElement->Name()) == "lowlimit") {
                    lowerBound = Expression::DeserializeFromMathML(nextElement->FirstChildElement());
                    nextElement = nextElement->NextSiblingElement();
                }

                if (nextElement && std::string(nextElement->Name()) == "uplimit") {
                    upperBound = Expression::DeserializeFromMathML(nextElement->FirstChildElement());
                }

                return std::make_unique<Fourier<Expression>>(
                    std::move(operand),
                    std::move(lowerBound ? *lowerBound : Expression{}),
                    std::move(upperBound ? *upperBound : Expression{})
                );
            }
        }
        return nullptr;
    }

    DECL_SPECIALIZE(Fourier)
    EXPRESSION_TYPE(Fourier)
    EXPRESSION_CATEGORY(UnExp)
};

} // namespace Oasis

#endif // OASIS_FOURIER_HPP
