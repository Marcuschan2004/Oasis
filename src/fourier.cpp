#include "fourier.hpp"

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

} // namespace Oasis
