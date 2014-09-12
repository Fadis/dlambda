#ifndef DLAMBDA_TYPE_TRAITS_IS_ARRAY_HPP
#define DLAMBDA_TYPE_TRAITS_IS_ARRAY_HPP

#include <dlambda/type_traits/is_same.hpp>
#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    namespace meta {
      template< typename Type >
      struct is_array :
        public boost::is_same< Type, types::array< type > > {};
    }
    bool is_array( const type& );
  }
}

#endif

