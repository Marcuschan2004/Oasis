#include "Sum.hpp"
#include "Constant.hpp"
#include "Add.hpp"
#include "Multiply.hpp"
#include "Divide.hpp"
#include "Subtract.hpp"
#include <stdexcept>

namespace Oasis {

auto Sum::Evaluate(const std::unordered_map<std::string, double>& variables) const -> double {
    double lower = lowerBound_->Evaluate(variables);
    double upper = upperBound_->Evaluate(variables);
    double result = 0.0;

    for (double i = lower; i <= upper; ++i) {
        std::unordered_map<std::string, double> localVars = variables;
        localVars["i"] = i;  // Assuming the variable for summation is "i"
        result += function_->Evaluate(localVars);
    }

    return result;
}

auto Sum::Simplify() const -> std::unique_ptr<Expression> {
    // Apply specific summation identities if possible
    return ApplySummationIdentities();
}

auto Sum::Differentiate(const Expression& differentiationVariable) const -> std::unique_ptr<Expression> {
    // Differentiation of a summation is complex and often outside the scope of basic identities
    throw std::logic_error("Differentiation of a summation is not supported.");
}

auto Sum::Copy() const -> std::unique_ptr<Expression> {
    return std::make_unique<Sum>(lowerBound_->Clone(), upperBound_->Clone(), function_->Clone());
}

auto Sum::Copy(tf::Subflow& subflow) const -> std::unique_ptr<Expression> {
    return std::make_unique<Sum>(lowerBound_->Clone(), upperBound_->Clone(), function_->Clone());
}

auto Sum::Equals(const Expression& other) const -> bool {
    if (!other.Is<Sum>()) {
        return false;
    }

    const auto& otherSum = static_cast<const Sum&>(other);
    return lowerBound_->Compare(*otherSum.lowerBound_) &&
           upperBound_->Compare(*otherSum.upperBound_) &&
           function_->Compare(*otherSum.function_);
}

auto Sum::ToString() const -> std::string {
    return "Σ(" + lowerBound_->ToString() + ", " + upperBound_->ToString() + ", " + function_->ToString() + ")";
}

auto Sum::ApplySummationIdentities() const -> std::unique_ptr<Expression> {
    // Example: Apply the identity for summation of a constant
    if (function_->Is<Constant>()) {
        auto constantValue = function_->Evaluate({});
        auto lowerValue = lowerBound_->Evaluate({});
        auto upperValue = upperBound_->Evaluate({});

        double n = upperValue - lowerValue + 1;
        return std::make_unique<Multiply>(std::make_unique<Constant>(n, "n"), function_->Clone());
    }

    // Example: Gauss’s identity (sum of integers from 0 to n-1)
    if (function_->Is<Variable>() && lowerBound_->Is<Constant>() && upperBound_->Is<Constant>()) {
        auto lowerValue = lowerBound_->Evaluate({});
        auto upperValue = upperBound_->Evaluate({});
        auto variableName = function_->ToString();

        if (variableName == "i" && lowerValue == 0) {
            // Apply Gauss's identity: sum from 0 to n-1
            double n = upperValue;
            return std::make_unique<Divide>(
                std::make_unique<Multiply>(
                    std::make_unique<Constant>(n, "n"),
                    std::make_unique<Constant>(n - 1, "n-1")
                ),
                std::make_unique<Constant>(2, "2")
            );
        }
    }

    // Return a clone of the sum if no identity can be applied
    return Copy();
}

auto Sum::StructurallyEquivalent(const Expression& other) const -> bool {
    if (!other.Is<Sum>()) {
        return false;
    }

    const auto& otherSum = static_cast<const Sum&>(other);
    return lowerBound_->StructurallyEquivalent(*otherSum.lowerBound_) &&
           upperBound_->StructurallyEquivalent(*otherSum.upperBound_) &&
           function_->StructurallyEquivalent(*otherSum.function_);
}

auto Sum::StructurallyEquivalent(const Expression& other, tf::Subflow& subflow) const -> bool {
    if (!other.Is<Sum>()) {
        return false;
    }

    const auto& otherSum = static_cast<const Sum&>(other);
    return lowerBound_->StructurallyEquivalent(*otherSum.lowerBound_, subflow) &&
           upperBound_->StructurallyEquivalent(*otherSum.upperBound_, subflow) &&
           function_->StructurallyEquivalent(*otherSum.function_, subflow);
}

} // namespace Oasis
