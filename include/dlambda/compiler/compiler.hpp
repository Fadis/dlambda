#ifndef DLAMBDA_COMPILER_COMPILE_HPP
#define DLAMBDA_COMPILER_COMPILE_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <dlambda/type.hpp>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/eval.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/get_llvm_function_type.hpp>

namespace dlambda {
  namespace compiler {
    class function {
    public:
      function(
        const std::shared_ptr< llvm::LLVMContext > &,
        const std::shared_ptr< ir_builder_t > &,
        const std::shared_ptr< llvm::Module > &,
        const std::string &,
        const types::function< dlambda::type > &
      );
      function( const function& ) = default;
      function( function&& ) = default;
      void operator[]( const proto::node &expr ) const;
    private:
      std::string function_name;
      std::shared_ptr< llvm::LLVMContext > context;
      std::shared_ptr< ir_builder_t > ir_builder;
      std::shared_ptr< llvm::Module > module;
      std::shared_ptr< llvm::Function > llvm_function;
      std::vector< std::shared_ptr< llvm::BasicBlock > > llvm_basic_block;
      proto::eval< expression > evaluator;
      std::shared_ptr< llvm::FunctionType > llvm_function_type;
      types::function< dlambda::type > function_type;
    };
    class module {
    public:
      module(
        const std::shared_ptr< llvm::LLVMContext > &
      );
      template< typename FunctionType >
      function create_function( const std::string &name ) const {
        return function( context, ir_builder, llvm_module, name, boost::get< types::function< dlambda::type > >( get_type< FunctionType >() ) );
      }
      template< typename FunctionType >
      function create_function() const {
        return function( context, ir_builder, llvm_module, "entry", boost::get< types::function< dlambda::type > >( get_type< FunctionType >() ) );
      }
      void dump() const {
        llvm_module->dump();
      }
      const std::shared_ptr< llvm::LLVMContext > &get_llvm_context() const {
        return context;
      }
      const std::shared_ptr< llvm::Module > &get_llvm_module() const {
        return llvm_module;
      }
    private:
      std::shared_ptr< llvm::LLVMContext > context;
      std::shared_ptr< ir_builder_t > ir_builder;
      std::shared_ptr< llvm::Module > llvm_module;
    };
  }
}

#endif

