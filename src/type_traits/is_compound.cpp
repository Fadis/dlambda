#include <dlambda/type.hpp>
#include <dlambda/type_traits/is_fundamental.hpp>
#include <dlambda/type_traits/is_compound.hpp>

namespace dlambda {
  namespace type_traits {
    bool is_compound( const type &type_ ) {
      return !is_fundamental( type_ );
    }
  }
}

