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
#include <dlambda/type_traits/is_promotable.hpp>
#include <dlambda/type_traits/is_lvalue_reference.hpp>
#include <dlambda/type_traits/integral_promotion.hpp>
#include <dlambda/type_traits/is_floating_point.hpp>
#include <dlambda/type_traits/remove_reference.hpp>
#include <dlambda/compiler/node/assign.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/node/remove_reference.hpp>
#include <dlambda/compiler/node/implicit_cast.hpp>
#include <dlambda/compiler/usual_arithmetic_conversion.hpp>
#include <dlambda/compiler/get_llvm_constant_value.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace compiler {
    expression assign(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left_expr, const expression &right_expr
    ) {
      std::cout << "assign" << std::endl;
      if( type_traits::is_lvalue_reference( left_expr.type() ) ) {
        const auto calcurated = implicit_cast( context, ir_builder, type_traits::remove_reference( left_expr.type() ), right_expr );
        ir_builder->CreateStore( calcurated.llvm_value().get(), left_expr.llvm_value().get() );
        return left_expr;
      }
      else throw exceptions::invalid_expression();
    }
  }
}

