#ifndef DLAMBDA_TYPE_TRAITS_IS_POINTER_HPP
#define DLAMBDA_TYPE_TRAITS_IS_POINTER_HPP

#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    namespace meta {
      template< typename T >
      struct is_pointer :
        public boost::mpl::bool_< false > {};
      template< typename T >
      struct is_pointer< types::pointer< T > > :
        public boost::mpl::bool_< true > {};
    }
    bool is_pointer( const type& );
  }
}

#endif

