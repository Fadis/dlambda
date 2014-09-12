#ifndef DLAMBDA_TYPE_TRAITS_MATCH_HPP
#define DLAMBDA_TYPE_TRAITS_MATCH_HPP
#include <boost/variant.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/utility/enable_if.hpp>

#include <dlambda/type_traits/is_same.hpp>

namespace dlambda {
  namespace type_traits {
    bool match( const type&, const type& );
  }
}

#endif


