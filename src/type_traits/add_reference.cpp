#include <dlambda/type.hpp>
#include <dlambda/type_traits/add_reference.hpp>
#include <dlambda/type_traits/add_lvalue_reference.hpp>

namespace dlambda {
  namespace type_traits {
    type add_reference( const type &type_ ) {
      return add_lvalue_reference( type_ );
    }
  }
}

