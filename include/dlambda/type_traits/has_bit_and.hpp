#ifndef DLAMBDA_TYPE_TRAITS_HAS_BIT_AND_HPP
#define DLAMBDA_TYPE_TRAITS_HAS_BIT_AND_HPP

#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    bool has_bit_and( const type& );
    bool has_bit_and( const type&, const type& );
    bool has_bit_and( const type&, const type&, const type& );
  }
}

#endif

