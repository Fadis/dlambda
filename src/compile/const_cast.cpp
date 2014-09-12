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
#include <dlambda/type_traits/is_rvalue_reference.hpp>
#include <dlambda/type_traits/is_array.hpp>
#include <dlambda/compiler/node/native_cast.hpp>
#include <dlambda/compiler/node/arithmetic_cast.hpp>
#include <dlambda/compiler/node/dereference_cast.hpp>
#include <dlambda/compiler/node/reference_cast.hpp>
#include <dlambda/compiler/node/remove_const_cast.hpp>
#include <dlambda/compiler/node/array_cast.hpp>
#include <dlambda/compiler/node/remove_reference.hpp>
#include <dlambda/exceptions.hpp>
#include <dlambda/compiler/node/const_cast.hpp>

namespace dlambda {
  namespace compiler {
    expression const_cast_(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    ) {
      if(
        type_traits::is_reference( value.type() ) ||
        type_traits::is_pointer( value.type() )
      ) {
        if(
          value.type().which() == cast_to.which()
        ) return remove_const_cast( context, ir_builder, cast_to, value );
        else if(
          type_traits::is_reference( value.type() ) &&
          type_traits::is_pointer( cast_to )
        ) {
          const auto dereferenced = remove_reference( context, ir_builder, value );
          return const_cast_( context, ir_builder, cast_to, dereferenced );
        }
        else if(
          type_traits::is_rvalue_reference( value.type() ) &&
          type_traits::is_lvalue_reference( cast_to )
        ) return reference_cast( context, ir_builder, cast_to, value );
      }
      throw exceptions::invalid_cast();
    }
  }
}

