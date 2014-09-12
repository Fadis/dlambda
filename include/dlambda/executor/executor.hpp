#ifndef DLAMBDA_EXECUTOR_EXECUTOR_HPP
#define DLAMBDA_EXECUTOR_EXECUTOR_HPP

#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/ExecutionEngine/JIT.h>
#include <llvm/ExecutionEngine/JITEventListener.h>
#include <llvm/ExecutionEngine/MCJIT.h>
#pragma GCC diagnostic pop
#include <dlambda/compiler/compiler.hpp>

namespace dlambda {
  namespace executor {
    template< typename FunctionType >
    class function {
    public:
      function(
        const std::shared_ptr< llvm::Module > &module_,
        const std::shared_ptr< llvm::EngineBuilder > &builder_,
        const std::shared_ptr< llvm::ExecutionEngine > &engine_,
        const std::shared_ptr< llvm::Function > &function_
      ) : llvm_module( module_ ), builder( builder_ ), engine( engine_ ), llvm_function( function_ ) {
      }
      template< typename ...Args >
      typename boost::function_types::result_type< FunctionType >::type
      operator()( Args ...args ) const {
        const auto function = engine->getPointerToFunction( llvm_function.get() );
        if( !function )
          throw -1;
        return reinterpret_cast< FunctionType* >( function )( args... );
      }
      function( const function< FunctionType >& ) = default;
      function( function< FunctionType >&& ) = default;
    private:
      std::shared_ptr< llvm::Module > llvm_module;
      std::shared_ptr< llvm::EngineBuilder > builder;
      std::shared_ptr< llvm::ExecutionEngine > engine;
      std::shared_ptr< llvm::Function > llvm_function;
    };
    class module {
    public:
      module( const compiler::module &module, const std::string &triple );
      template< typename FunctionType >
      function< FunctionType > get_function( const std::string &name ) const {
        const std::shared_ptr< llvm::LLVMContext > context_ = context;
        const std::shared_ptr< llvm::EngineBuilder > builder_ = builder;
        const std::shared_ptr< llvm::Function > entry_point(
          llvm_module->getFunction( name.c_str() ),
          [context_,builder_]( const llvm::Function* ){}
        );
        if( !entry_point )
          throw -1;
        return function< FunctionType >( llvm_module, builder, engine, entry_point );
      }
    private:
      static void deleteBuilder(
        llvm::EngineBuilder *builder,
        std::shared_ptr< llvm::ExecutionEngine > engine,
        std::shared_ptr< std::string >,
        std::shared_ptr< llvm::Module >,
        std::shared_ptr< llvm::LLVMContext >
      );
      std::shared_ptr< llvm::LLVMContext > context;
      std::shared_ptr< llvm::Module > llvm_module;
      std::shared_ptr< llvm::EngineBuilder > builder;
      std::shared_ptr< llvm::ExecutionEngine > engine;
    };
  }
}

#endif

