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
#include <dlambda/type_traits/is_promotable.hpp>
#include <dlambda/type_traits/is_reference.hpp>
#include <dlambda/compiler/node/type_only_cast.hpp>
#include <dlambda/compiler/node/address_cast.hpp>
#include <dlambda/exceptions.hpp>
#include <dlambda/compiler/node/reinterpret_cast.hpp>

namespace dlambda {
  namespace compiler {
    expression reinterpret_cast_(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    ) {
      if(
        ( type_traits::is_reference( value.type() ) ||
        type_traits::is_pointer( value.type() ) ) &&
        ( type_traits::is_reference( cast_to ) ||
        type_traits::is_pointer( cast_to ) )
      ) return type_only_cast( context, ir_builder, cast_to, value );
      else if(
        type_traits::is_pointer( value.type() ) ||
        type_traits::is_promotable( cast_to )
      ) return address_cast( context, ir_builder, cast_to, value );
      else if(
        type_traits::is_promotable( value.type() ) ||
        type_traits::is_pointer( cast_to )
      ) return address_cast( context, ir_builder, cast_to, value );
      throw exceptions::invalid_cast();
    }
  }
}

