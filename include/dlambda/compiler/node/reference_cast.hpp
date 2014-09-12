#ifndef DLAMBDA_COMPILER_NODE_REFERENCE_CAST_HPP
#define DLAMBDA_COMPILER_NODE_REFERENCE_CAST_HPP

#include <memory>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <llvm/IR/LLVMContext.h>
#include <dlambda/type.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>

#include <dlambda/type_traits/integral_promotion.hpp>
#include <dlambda/type_traits/is_pointer.hpp>
#include <dlambda/type_traits/is_signed.hpp>
#include <dlambda/type_traits/is_arithmetic.hpp>
#include <dlambda/type_traits/is_floating_point.hpp>

namespace dlambda {
  namespace compiler {
    namespace meta {

    }
    expression reference_cast(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const std::shared_ptr< ir_builder_t > &ir_builder,
      const type cast_to,
      const expression &value
    );
  }
}

#endif

