#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/variant/static_visitor.hpp>
#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_integral.hpp>
#include <dlambda/type_traits/is_floating_point.hpp>
#include <dlambda/type_traits/is_arithmetic.hpp>

namespace dlambda {
  namespace type_traits {
    bool is_arithmetic( const type &type_ ) {
      return is_integral( type_ ) || is_floating_point( type_ ); 
    }
  }
}

