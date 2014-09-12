#ifndef DLAMBDA_VALUE_HPP
#define DLAMBDA_VALUE_HPP

#include <cstdint>
#include <vector>
#include <boost/variant/recursive_variant.hpp>
#include <boost/any.hpp>

namespace dlambda {
  typedef boost::make_recursive_variant<
    uint64_t,
    int64_t,
    void*,
    bool,
    float,
    double,
    long double,
    std::vector< boost::recursive_variant_ >,
    boost::any
  >::type value;
}

#endif

