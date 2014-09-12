#ifndef DLAMBDA_COMPILER_EXPRESSION_HPP
#define DLAMBDA_COMPILER_EXPRESSION_HPP

#include <boost/range/algorithm/copy.hpp>

#include <dlambda/type.hpp>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>

namespace dlambda {
  namespace compiler {
    class expression {
    public:
      expression(
        const dlambda::type &type__,
        const std::shared_ptr< llvm::Type > &llvm_type__,
        const std::shared_ptr< llvm::Value > &llvm_value__
      ) : type_( type__ ), llvm_type_( llvm_type__ ), llvm_value_( llvm_value__ ) {}
      expression(
        dlambda::type &&type__,
        std::shared_ptr< llvm::Type > &&llvm_type__,
        std::shared_ptr< llvm::Value > &&llvm_value__
      ) : type_( type__ ), llvm_type_( llvm_type__ ), llvm_value_( llvm_value__ ) {}
      expression(
        const dlambda::type &type__,
        const std::pair< std::shared_ptr< llvm::Type >, std::shared_ptr< llvm::Value > > &llvm_value__
      ) : type_( type__ ), llvm_type_( llvm_value__.first ), llvm_value_( llvm_value__.second ) {}
      expression(
        dlambda::type &&type__,
        std::pair< std::shared_ptr< llvm::Type >, std::shared_ptr< llvm::Value > > &&llvm_value__
      ) : type_( type__ ), llvm_type_( llvm_value__.first ), llvm_value_( llvm_value__.second ) {}
      expression( const expression& ) = default;
      expression( expression&& ) = default;
      expression &operator=( const expression& ) = default;
      expression &operator=( expression&& ) = default;
      const dlambda::type &type() const { return type_; }
      const std::shared_ptr< llvm::Type > &llvm_type() const { return llvm_type_; }
      const std::shared_ptr< llvm::Value > &llvm_value() const { return llvm_value_; }
    private:
      dlambda::type type_;
      std::shared_ptr< llvm::Type > llvm_type_;
      std::shared_ptr< llvm::Value > llvm_value_;
    };
  }
}

#endif

