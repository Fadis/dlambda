#ifndef DLAMBDA_TYPE_TRAITS_CONDITIONAL_HPP
#define DLAMBDA_TYPE_TRAITS_CONDITIONAL_HPP

#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    type conditional( bool, const type&, const type& );
  }
}

#endif

