#include <utility>
#include <boost/mpl/copy.hpp>
#include <boost/range/algorithm/copy.hpp>

#include <llvm/IR/LLVMContext.h>

#include <dlambda/type.hpp>
#include <dlambda/type_traits/usual_arithmetic_conversion.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/node/implicit_cast.hpp>
#include <dlambda/compiler/usual_arithmetic_conversion.hpp>

namespace dlambda {
  namespace compiler {
    std::pair< expression, expression > usual_arithmetic_conversion(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const expression &left, const expression &right
    ) {
      const auto result_type = type_traits::usual_arithmetic_conversion( left.type(), right.type() );
      return std::make_pair(
        implicit_cast( context, ir_builder, result_type, left ),
        implicit_cast( context, ir_builder, result_type, right )
      );
    }
  }
}

