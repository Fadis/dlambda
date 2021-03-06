#ifndef DLAMBDA_TYPE_TRAITS_IS_RVALUE_REFERENCE_HPP
#define DLAMBDA_TYPE_TRAITS_IS_RVALUE_REFERENCE_HPP

#include <boost/mpl/bool.hpp>
#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    namespace meta {
      template< typename T >
      struct is_rvalue_reference :
        public boost::mpl::bool_< false > {};
      template< typename T >
      struct is_rvalue_reference< types::rref< T > > :
        public boost::mpl::bool_< true > {};
    }
    bool is_rvalue_reference( const type& );
  }
}

#endif

