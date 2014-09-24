#ifndef DLAMBDA_EXECUTOR_EXECUTOR_HPP
#define DLAMBDA_EXECUTOR_EXECUTOR_HPP

#include <boost/preprocessor/comma.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/enum.hpp>
#include <boost/preprocessor/repeat.hpp>
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
#include <dlambda/exceptions.hpp>

namespace dlambda {
  namespace executor {
    template< typename FunctionType, typename Enable = void >
    class function {};

#define DLAMBDA_EXECUTOR_FUNCTION_ARGS_DEFINITION( z, index, unused ) \
  typename boost::mpl::at< \
    typename boost::function_types::parameter_types< FunctionType >::type BOOST_PP_COMMA() \
    boost::mpl::size_t< index > \
  >::type BOOST_PP_CAT( arg, index )
#define DLAMBDA_EXECUTOR_FUNCTION_ARGS( z, index, unused ) \
  BOOST_PP_CAT( arg, index )
#define DLAMBDA_EXECUTOR_FUNCTION( z, index, unused ) \
    template< \
      typename FunctionType \
    > \
    class function< FunctionType, \
      typename boost::enable_if< \
        boost::mpl::equal_to< \
          boost::mpl::size< \
            typename boost::function_types::parameter_types< \
              FunctionType \
            >::type \
          >, \
          boost::mpl::size_t< index > \
        > \
      >::type \
    > { \
    public: \
      typedef  \
        class function< FunctionType, \
          typename boost::enable_if< \
            boost::mpl::equal_to< \
              boost::mpl::size< \
                typename boost::function_types::parameter_types< \
                  FunctionType \
                >::type \
              >, \
              boost::mpl::size_t< index > \
            > \
          >::type \
        > self_type; \
      function( \
        const std::shared_ptr< llvm::Module > &module_, \
        const std::shared_ptr< llvm::EngineBuilder > &builder_, \
        const std::shared_ptr< llvm::ExecutionEngine > &engine_, \
        const std::shared_ptr< llvm::Function > &function_ \
      ) : llvm_module( module_ ), builder( builder_ ), engine( engine_ ), llvm_function( function_ ) {} \
      typename boost::function_types::result_type< FunctionType >::type \
      operator()( \
        BOOST_PP_ENUM( index, DLAMBDA_EXECUTOR_FUNCTION_ARGS_DEFINITION, ) \
      ) const { \
        const auto function = engine->getPointerToFunction( llvm_function.get() ); \
        if( !function ) \
          throw exceptions::no_such_function(); \
        return reinterpret_cast< FunctionType* >( function )( \
          BOOST_PP_ENUM( index, DLAMBDA_EXECUTOR_FUNCTION_ARGS, ) \
        ); \
      } \
      function( const self_type& ) = default; \
      function( self_type&& ) = default; \
    private: \
      std::shared_ptr< llvm::Module > llvm_module; \
      std::shared_ptr< llvm::EngineBuilder > builder; \
      std::shared_ptr< llvm::ExecutionEngine > engine; \
      std::shared_ptr< llvm::Function > llvm_function; \
    };

    BOOST_PP_REPEAT( 10, DLAMBDA_EXECUTOR_FUNCTION, )

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
      template< typename FunctionType >
      function< FunctionType > get_function() const {
        return get_function< FunctionType >( "entry" );
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

