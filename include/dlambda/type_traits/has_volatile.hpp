#ifndef DLAMBDA_TYPE_TRAITS_HAS_VOLATILE_HPP
#define DLAMBDA_TYPE_TRAITS_HAS_VOLATILE_HPP

#include <boost/mpl/has_xxx.hpp>

namespace dlambda {
  namespace type_traits {
    namespace detail {
      BOOST_MPL_HAS_XXX_TRAIT_DEF(volatile_)
    }
  }
}

#endif

