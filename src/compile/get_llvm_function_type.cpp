#include <array>
#include <vector>
#include <memory>
#include <boost/array.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/variant/get.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>
#include <llvm/IR/Type.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/StringRef.h>
#include <dlambda/type.hpp>
#include <dlambda/get_type.hpp>
#include <dlambda/type_traits/is_function.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace compiler {
    std::shared_ptr< llvm::FunctionType > get_llvm_function_type(
      const std::shared_ptr< llvm::LLVMContext > &context,
      const type &type_
    ) {
      if( !type_traits::is_function( type_ ) )
        throw exceptions::unexpected_type();
      const types::function< type > &function_type = boost::get< types::function< type > >( type_ );
      std::shared_ptr< llvm::LLVMContext > context_ = context;
      std::shared_ptr< std::vector< std::shared_ptr< llvm::Type > > > argument_types(
        new std::vector< std::shared_ptr< llvm::Type > >()
      );
      std::shared_ptr< std::vector< llvm::Type* > > raw_argument_types(
        new std::vector< llvm::Type* >()
      );
      std::transform(
        function_type.args_type.begin(), function_type.args_type.end(), std::back_inserter( *argument_types ),
        [context]( const type &type_ ) {
				      return get_llvm_type( context, type_ );
				    }
      );
      std::transform(
        argument_types->begin(), argument_types->end(), std::back_inserter( *raw_argument_types ),
        []( const std::shared_ptr< llvm::Type > &type_ ) {
				      return type_.get();
				    }
      );
      const auto result_type = get_llvm_type( context, function_type.result_type );
      if( !raw_argument_types->empty() ) {
        return std::shared_ptr< llvm::FunctionType >(
          llvm::FunctionType::get(
            result_type.get(),
            llvm::ArrayRef< llvm::Type* >( *raw_argument_types ),
            function_type.variable
          ),
          [context_,result_type,argument_types]( llvm::Type* ){}
        );
      }
      else {
        return std::shared_ptr< llvm::FunctionType >(
          llvm::FunctionType::get(
            result_type.get(),
            function_type.variable
          ),
          [context_,result_type,argument_types]( llvm::Type* ){}
        );
      }
    }
  }
}

