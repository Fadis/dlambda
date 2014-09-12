#ifndef DLAMBDA_TYPE_TRAITS_IS_AGGREGATED_HPP
#define DLAMBDA_TYPE_TRAITS_IS_AGGREGATED_HPP

#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    namespace meta {
      template< typename T >
      struct is_aggregated :
        public boost::mpl::bool_< false > {};
      template< typename T >
      struct is_aggregated< types::array< T > > :
        public boost::mpl::bool_< true > {};
      template< typename T >
      struct is_aggregated< types::vector< T > > :
        public boost::mpl::bool_< true > {};
      template< typename T >
      struct is_aggregated< types::tuple< T > > :
        public boost::mpl::bool_< true > {};
      template<>
      struct is_aggregated< types::user_defined_type > :
        public boost::mpl::bool_< true > {};
    }
    bool is_aggregated( const type& );
  }
}

#endif

