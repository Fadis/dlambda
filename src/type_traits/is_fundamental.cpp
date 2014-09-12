#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_floating_point.hpp>
#include <dlambda/type_traits/is_integral.hpp>
#include <dlambda/type_traits/is_void.hpp>
#include <dlambda/type_traits/is_fundamental.hpp>

namespace dlambda {
  namespace type_traits {
    bool is_fundamental( const type &type_ ) {
      return is_integral( type_ ) || is_floating_point( type_ ) || is_void( type_ );
    }
  }
}

