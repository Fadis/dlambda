#ifndef DLAMBDA_TYPE_TRAITS_IS_FLOATING_POINT_HPP
#define DLAMBDA_TYPE_TRAITS_IS_FLOATING_POINT_HPP

#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    namespace meta {
      template< typename Type >
      struct is_floating_point :
        public boost::mpl::or_<
          boost::is_same< Type, types::float_ >
        > {};
    }
    bool is_floating_point( const type& );
  }
}

#endif

