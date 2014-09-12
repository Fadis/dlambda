#include <utility>
#include <boost/mpl/copy.hpp>
#include <boost/range/algorithm/copy.hpp>

#include <llvm/IR/LLVMContext.h>

#include <dlambda/type.hpp>
#include <dlambda/type_traits/integral_promotion.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/node/implicit_cast.hpp>
#include <dlambda/compiler/integral_promotion.hpp>

namespace dlambda {
  namespace compiler {
    expression integral_promotion(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &value
    ) {
      const auto result_type = type_traits::integral_promotion( value.type() );
      return implicit_cast( context, ir_builder, result_type, value );
    }
  }
}

