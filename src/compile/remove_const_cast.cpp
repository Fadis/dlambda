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
#include <dlambda/type_traits/remove_const.hpp>
#include <dlambda/exceptions.hpp>
#include <dlambda/compiler/node/remove_const_cast.hpp>

namespace dlambda {
  namespace compiler {
    expression remove_const_cast(
      const std::shared_ptr< llvm::LLVMContext > &,
      const std::shared_ptr< ir_builder_t > &,
      const type to,
      const expression &value
    ) {
      if( type_traits::remove_const( value.type() ) == type_traits::remove_const( to ) )
        return expression( to, value.llvm_type(), value.llvm_value() );
      else
        throw exceptions::invalid_cast();
    }
  }
}

