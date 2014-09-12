#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include <dlambda/type.hpp>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/eval.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/compiler/get_llvm_constant_value.hpp>
#include <dlambda/type_traits/integral_promotion.hpp>
#include <dlambda/type_traits/usual_arithmetic_conversion.hpp>
#include <dlambda/type_traits/is_pointer.hpp>
#include <dlambda/type_traits/is_signed.hpp>
#include <dlambda/type_traits/is_arithmetic.hpp>
#include <dlambda/type_traits/is_floating_point.hpp>
#include <dlambda/type_traits/is_aggregated.hpp>
#include <dlambda/type_traits/is_reference.hpp>
#include <dlambda/type_traits/is_lvalue_reference.hpp>
#include <dlambda/type_traits/is_array.hpp>
#include <dlambda/type_traits/is_enum.hpp>
#include <dlambda/compiler/node/native_cast.hpp>
#include <dlambda/compiler/node/arithmetic_cast.hpp>
#include <dlambda/compiler/node/dereference_cast.hpp>
#include <dlambda/compiler/node/reference_cast.hpp>
#include <dlambda/compiler/node/remove_const_cast.hpp>
#include <dlambda/compiler/node/array_cast.hpp>
#include <dlambda/compiler/node/implicit_cast.hpp>
#include <dlambda/compiler/node/remove_reference.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace compiler {
    expression implicit_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    ) {
      if(
        type_traits::is_arithmetic( value.type() ) &&
        type_traits::is_arithmetic( cast_to )
      ) {
        if( type_traits::is_enum( value.type() ) || type_traits::is_enum( cast_to ) )
          throw exceptions::invalid_cast();
        return arithmetic_cast( context, ir_builder, cast_to, value );
      }
      else if(
        value.type().which() == cast_to.which()
      ) {
        if(
          type_traits::is_reference( value.type() ) ||
          type_traits::is_pointer( value.type() )
        ) return native_cast( context, ir_builder, cast_to, value );
        else return remove_const_cast( context, ir_builder, cast_to, value );
      }
      else if(
        type_traits::is_reference( value.type() )
      ) {
        if( type_traits::is_arithmetic( cast_to ) ||
            type_traits::is_aggregated( cast_to )
        ) {
          const auto dereferenced = remove_reference( context, ir_builder, value );
          return implicit_cast( context, ir_builder, cast_to, dereferenced );
        }
        else if( type_traits::is_lvalue_reference( cast_to ) ) {
          return reference_cast( context, ir_builder, cast_to, value );
        }
      }
      else if( type_traits::is_array( value.type() ) ) {
          return array_cast( context, ir_builder, cast_to, value );
      }
      throw exceptions::invalid_cast();
    }
  }
}

