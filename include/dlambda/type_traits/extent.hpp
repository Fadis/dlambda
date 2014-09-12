#ifndef DLAMBDA_TYPE_TRAITS_EXTENT_HPP
#define DLAMBDA_TYPE_TRAITS_EXTENT_HPP

#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>

namespace dlambda {
  namespace type_traits {
    size_t extent( const type &type_, size_t level = 0u );
  }
}

#endif

