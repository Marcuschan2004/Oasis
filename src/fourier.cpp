#include "Fourier.hpp"

namespace Oasis {

// Specialize for Fourier
template <>
auto Fourier<Expression>::Specialize(const Expression& other) -> std::unique_ptr<Fourier<Expression>> {
    if (!other.Is<Fourier>()) {
        return nullptr;
    }
    return std::make_unique<Fourier<Expression>>(static_cast<const Fourier<Expression>&>(other));
}

template <>
auto Fourier<Expression>::Specialize(const Expression& other, tf::Subflow&) -> std::unique_ptr<Fourier<Expression>> {
    return Specialize(other);
}

// Implement the MathML serialization and deserialization
void Fourier<Expression>::SerializeToMathML(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument& doc) const {
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

std::unique_ptr<Expression> Fourier<Expression>::DeserializeFromMathML(const tinyxml2::XMLElement* element) {
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

} // namespace Oasis
