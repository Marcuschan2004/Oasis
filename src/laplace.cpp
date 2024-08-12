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

} // namespace Oasis
