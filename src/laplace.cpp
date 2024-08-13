#include "Laplace.hpp"

namespace Oasis {

// Specialize for Laplace
template <>
auto Laplace<Expression>::Specialize(const Expression& other) -> std::unique_ptr<Laplace<Expression>> {
    if (!other.Is<Laplace>()) {
        return nullptr;
    }
    return std::make_unique<Laplace<Expression>>(static_cast<const Laplace<Expression>&>(other));
}

template <>
auto Laplace<Expression>::Specialize(const Expression& other, tf::Subflow&) -> std::unique_ptr<Laplace<Expression>> {
    return Specialize(other);
}

// Implement the MathML serialization and deserialization
void Laplace<Expression>::SerializeToMathML(tinyxml2::XMLElement* parent, tinyxml2::XMLDocument& doc) const {
    auto applyElement = doc.NewElement("apply");

    // Create MathML elements
    auto laplaceElement = doc.NewElement("laplace");
    applyElement->InsertEndChild(laplaceElement);

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

std::unique_ptr<Expression> Laplace<Expression>::DeserializeFromMathML(const tinyxml2::XMLElement* element) {
    if (std::string(element->Name()) == "apply") {
        auto firstChild = element->FirstChildElement();
        if (firstChild && std::string(firstChild->Name()) == "laplace") {
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

            return std::make_unique<Laplace<Expression>>(
                std::move(operand),
                std::move(lowerBound ? *lowerBound : Expression{}),
                std::move(upperBound ? *upperBound : Expression{})
            );
        }
    }
    return nullptr;
}

} // namespace Oasis
