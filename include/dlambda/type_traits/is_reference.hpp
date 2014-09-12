#ifndef DLAMBDA_TYPE_TRAITS_IS_REFERENCE_HPP
#define DLAMBDA_TYPE_TRAITS_IS_REFERENCE_HPP

#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    namespace meta {
      template< typename T >
      struct is_reference :
        public boost::mpl::bool_< false > {};
      template< typename T >
      struct is_reference< types::lref< T > > :
        public boost::mpl::bool_< true > {};
      template< typename T >
      struct is_reference< types::rref< T > > :
        public boost::mpl::bool_< true > {};
    }
    bool is_reference( const type& );
  }
}

#endif

