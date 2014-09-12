#ifndef DLAMBDA_TYPE_TRAITS_INTEGRAL_PROMORTION_HPP
#define DLAMBDA_TYPE_TRAITS_INTEGRAL_PROMORTION_HPP

#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    type integral_promotion( const type &type_ );
  }
}

#endif

