// This file is part of Eigen, a lightweight C++ template library
// for linear algebra.
//
// Copyright (C) 2022 Erik Schultheis (erik.schultheis@aalto.fi)
//
// This Source Code Form is subject to the terms of the Mozilla
// Public License v. 2.0. If a copy of the MPL was not distributed
// with this file, You can obtain one at http://mozilla.org/MPL/2.0/.

#ifndef EIGEN_PACKET_HELPERS_H
#define EIGEN_PACKET_HELPERS_H

#include "../InternalHeaderCheck.h"

/**
 * \file
 * \internal The purpose of this file is to forward-declare the packet traits types and
 * define the corresponding helper templates. This is required because we want to make use
 * of the helpers in generic expression templates that might be defined before the whole
 * packet machinery gets included (in particular in XprHelper.h)
 */

namespace Eigen {
namespace internal {

// these are documented in GenericPacketMath.h
template<typename T> struct packet_traits;
template<typename T> struct unpacket_traits;

//! Gets the underlying scalar type of a packet `P`
template<typename P>
using unpacket_underlying_t = typename unpacket_traits<P>::type;

//! Gets a packet of half the size for a packet type `P`, if available.
template<typename P>
using unpacket_half_t = typename unpacket_traits<P>::half;

//! Gets a packet of half the size for a packet type `P`, by applying half twice
template<typename P>
using unpacket_quarter_t = unpacket_half_t<unpacket_half_t<P>>;

//! Given a scalar type `S`, gets the corresponding full-sized packet type
template<typename S>
using packet_full_t = typename packet_traits<S>::type;

//! Given a scalar type `S`, gets the corresponding half-size packet, if it exists
template<typename S>
using packet_half_t = typename packet_traits<S>::half;

//! Given a scalar type `S`, gets the quarter-sized packet by applying half twice.
template<typename T>
using packet_quarter_t = packet_half_t<packet_half_t<T>>;

enum class PacketFraction {
    Full,
    Half,
    Quarter
};

template <PacketFraction Choice, typename Full, typename Half, typename Quarter>
struct packet_conditional;

template <typename Full, typename Half, typename Quarter>
struct packet_conditional<PacketFraction::Full, Full, Half, Quarter> { typedef Full type; };

template <typename Full, typename Half, typename Quarter>
struct packet_conditional<PacketFraction::Half, Full, Half, Quarter> { typedef Half type; };

template <typename Full, typename Half, typename Quarter>
struct packet_conditional<PacketFraction::Quarter, Full, Half, Quarter> { typedef Quarter type; };

/// This template alias allows to select either a full, half, or quarter packet for the given `Scalar` based on the first argument.
template<PacketFraction Choice, typename Scalar>
using packet_conditional_t = typename packet_conditional<Choice, packet_full_t<Scalar>, packet_half_t<Scalar>, packet_quarter_t<Scalar>>::type;

}   // namespace internal
}   // namespace Eigen
#endif //EIGEN_PACKET_HELPERS_H