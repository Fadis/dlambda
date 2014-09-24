#include <boost/range/algorithm/copy.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/preprocessor/if.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/spirit/include/karma.hpp>
#include <llvm/IR/Value.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

#include <dlambda/type.hpp>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/eval.hpp>
#include <dlambda/type_traits/usual_arithmetic_conversion.hpp>
#include <dlambda/type_traits/is_arithmetic.hpp>
#include <dlambda/type_traits/is_pointer.hpp>
#include <dlambda/type_traits/is_reference.hpp>
#include <dlambda/type_traits/is_void.hpp>
#include <dlambda/compiler/get_llvm_type.hpp>
#include <dlambda/compiler/get_llvm_constant_value.hpp>
#include <dlambda/compiler/ir_builder.hpp>
#include <dlambda/compiler/expression.hpp>
#include <dlambda/compiler/node/remove_reference.hpp>
#include <dlambda/compiler/node/address_to_pointer.hpp>
#include <dlambda/compiler/node/static_cast.hpp>
#include <dlambda/compiler/node/const_cast.hpp>
#include <dlambda/compiler/node/reinterpret_cast.hpp>
#include <dlambda/compiler/node/c_style_cast.hpp>
#include <dlambda/compiler/node/implicit_cast.hpp>
#include <dlambda/compiler/node/unary_plus.hpp>
#include <dlambda/compiler/node/negate.hpp>
#include <dlambda/compiler/node/address_of.hpp>
#include <dlambda/compiler/node/complement.hpp>
#include <dlambda/compiler/node/logical_not.hpp>
#include <dlambda/compiler/node/pre_inc.hpp>
#include <dlambda/compiler/node/pre_dec.hpp>
#include <dlambda/compiler/node/post_inc.hpp>
#include <dlambda/compiler/node/post_dec.hpp>
#include <dlambda/compiler/node/dereference.hpp>
#include <dlambda/compiler/node/plus.hpp>
#include <dlambda/compiler/node/minus.hpp>
#include <dlambda/compiler/node/multiplies.hpp>
#include <dlambda/compiler/node/divides.hpp>
#include <dlambda/compiler/node/modulus.hpp>
#include <dlambda/compiler/node/shift_left.hpp>
#include <dlambda/compiler/node/shift_right.hpp>
#include <dlambda/compiler/node/less.hpp>
#include <dlambda/compiler/node/greater.hpp>
#include <dlambda/compiler/node/less_equal.hpp>
#include <dlambda/compiler/node/greater_equal.hpp>
#include <dlambda/compiler/node/equal_to.hpp>
#include <dlambda/compiler/node/not_equal_to.hpp>
#include <dlambda/compiler/node/logical_or.hpp>
#include <dlambda/compiler/node/logical_and.hpp>
#include <dlambda/compiler/node/bitwise_or.hpp>
#include <dlambda/compiler/node/bitwise_and.hpp>
#include <dlambda/compiler/node/bitwise_xor.hpp>
#include <dlambda/compiler/node/plus_assign.hpp>
#include <dlambda/compiler/node/minus_assign.hpp>
#include <dlambda/compiler/node/multiplies_assign.hpp>
#include <dlambda/compiler/node/divides_assign.hpp>
#include <dlambda/compiler/node/modulus_assign.hpp>
#include <dlambda/compiler/node/shift_left_assign.hpp>
#include <dlambda/compiler/node/shift_right_assign.hpp>
#include <dlambda/compiler/node/bitwise_or_assign.hpp>
#include <dlambda/compiler/node/bitwise_and_assign.hpp>
#include <dlambda/compiler/node/bitwise_xor_assign.hpp>
#include <dlambda/compiler/node/assign.hpp>
#include <dlambda/compiler/node/subscript.hpp>
#include <dlambda/compiler/node/mem_ptr.hpp>
#include <dlambda/compiler/node/comma.hpp>
#include <dlambda/compiler/get_llvm_constant_value.hpp>
#include <dlambda/compiler/get_llvm_function_type.hpp>
#include <dlambda/compiler/compiler.hpp>
#include <dlambda/exceptions.hpp>

#define DLAMBDA_COMPILER_UNARY_OPERATOR( s, data, oper ) \
  evaluator.push( \
    BOOST_PP_IF( \
      BOOST_PP_EQUAL( DLAMBDA_PROTO_OD_ELEM( 3, oper ), DLAMBDA_PROTO_WITH_INT ), \
      proto::any() DLAMBDA_PROTO_OD_ELEM( 1, oper ), \
      DLAMBDA_PROTO_OD_ELEM( 1, oper ) proto::any() \
    ), \
    [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > { \
      const auto &expr = boost::get< proto::node::unary_operator >( node_.get() ); \
      const auto value = evaluator( expr.args()[ 0 ] ); \
      return DLAMBDA_PROTO_OD_ELEM( 0, oper ) ( context, ir_builder, value ); \
    } \
  );

#define DLAMBDA_COMPILER_BINARY_OPERATOR( s, data, oper ) \
  evaluator.push( \
    proto::any() DLAMBDA_PROTO_OD_ELEM( 1, oper ) proto::any(), \
    [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > { \
      const auto &expr = boost::get< proto::node::binary_operator >( node_.get() ); \
      const auto left = evaluator( expr.args()[ 0 ] ); \
      const auto right = evaluator( expr.args()[ 1 ] ); \
      return DLAMBDA_PROTO_OD_ELEM( 0, oper ) ( context, ir_builder, left, right ); \
    } \
  );

namespace dlambda {
  namespace compiler {
    module::module(
      const std::shared_ptr< llvm::LLVMContext > &context__
    ) : context( context__ ), ir_builder( new ir_builder_t( *context__ ), [context__]( ir_builder_t *ptr ){ delete ptr; } ) {
      const std::shared_ptr< llvm::LLVMContext > &context_ = context;
      const auto module_name = to_string( boost::uuids::random_generator()() );
      llvm_module.reset(
        new llvm::Module( module_name.c_str(), *context ),
        [context_]( llvm::Module *ptr ){ delete ptr; }
      );
    }
      
    function::function(
      const std::shared_ptr< llvm::LLVMContext > &context__,
      const std::shared_ptr< ir_builder_t > &ir_builder_, 
      const std::shared_ptr< llvm::Module > &module_,
      const std::string &function_name_,
      const types::function< dlambda::type > &function_type_
    ) : function_name( function_name_ ),
    context( context__ ), ir_builder( ir_builder_ ), module( module_ ), evaluator( function_type_.args_type ),
    llvm_function_type( get_llvm_function_type( context__, function_type_ ) ), function_type( function_type_ ) {
      const std::shared_ptr< llvm::LLVMContext > &context_ = context;
      llvm_function.reset(
        llvm::Function::Create( llvm_function_type.get(), llvm::Function::ExternalLinkage, function_name.c_str() ),
        [context_]( llvm::Function* ){}
      );
      llvm_basic_block.emplace_back(
        llvm::BasicBlock::Create( *context, function_name.c_str(), llvm_function.get() ),
        [context_]( llvm::BasicBlock* ){}
      );
      ir_builder->SetInsertPoint( llvm_basic_block.back().get() );
      module->getFunctionList().push_back( llvm_function.get() );

      evaluator.push(
        proto::literal( any_type() ),
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &literal_ = boost::get< proto::literal >( node_.get() );
          return address_to_pointer( context, ir_builder, expression( literal_.type(), get_llvm_constant_value( context, literal_ ) ) );
        }
      );
      evaluator.push(
        proto::literal( any_type() ),
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &literal_ = boost::get< proto::literal >( node_.get() );
          return address_to_pointer( context, ir_builder, expression( literal_.type(), get_llvm_constant_value( context, literal_ ) ) );
        }
      );
      evaluator.push(
        proto::terminal( any_type() ),
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &placeholder_ = boost::get< proto::placeholder >( node_.get() );
          const auto range = boost::make_iterator_range( llvm_function->arg_begin(), llvm_function->arg_end() );
          if( placeholder_.index() >= size_t( boost::distance( range ) ) )
            throw exceptions::argument_out_of_range();
          return expression(
            function_type.args_type[ placeholder_.index() ],
            std::pair< std::shared_ptr<llvm::Type>, std::shared_ptr<llvm::Value> >(
              get_llvm_type( context, function_type.args_type[ placeholder_.index() ] ),
              std::shared_ptr<llvm::Value>(
                &*boost::next( boost::begin( range ), placeholder_.index() ),
                [context_]( llvm::Value* ){}
              )
            )
          );
        }
      );
      BOOST_PP_SEQ_FOR_EACH(
        DLAMBDA_COMPILER_UNARY_OPERATOR,
        unused,
        DLAMBDA_PROTO_UNARY_OPERATORS
      )
      BOOST_PP_SEQ_FOR_EACH(
        DLAMBDA_COMPILER_BINARY_OPERATOR,
        unused,
        DLAMBDA_PROTO_BINARY_OPERATORS_WITHOUT_SUBSCRIPT
      )
      evaluator.push(
        proto::any()[ proto::any() ],
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &expr = boost::get< proto::node::binary_operator >( node_.get() );
          const auto left = evaluator( expr.args()[ 0 ] );
          const auto right = evaluator( expr.args()[ 1 ] );
          return subscript( context, ir_builder, left, right );
        }
      );
      evaluator.push(
        ( proto::any(), proto::any() ),
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &expr = boost::get< proto::node::binary_operator >( node_.get() );
          evaluator( expr.args()[ 0 ] );
          return  evaluator( expr.args()[ 1 ] );
        }
      );
      evaluator.push(
        proto::static_cast_( any_type(), proto::any() ),
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &expr = boost::get< proto::node::cast >( node_.get() );
          const auto value = evaluator( expr.args()[ 0 ] );
          return static_cast_( context, ir_builder, expr.type(), value );
        }
      );
      evaluator.push(
        proto::const_cast_( any_type(), proto::any() ),
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &expr = boost::get< proto::node::cast >( node_.get() );
          const auto value = evaluator( expr.args()[ 0 ] );
          return const_cast_( context, ir_builder, expr.type(), value );
        }
      );
      evaluator.push(
        proto::reinterpret_cast_( any_type(), proto::any() ),
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &expr = boost::get< proto::node::cast >( node_.get() );
          const auto value = evaluator( expr.args()[ 0 ] );
          return reinterpret_cast_( context, ir_builder, expr.type(), value );
        }
      );
      evaluator.push(
        proto::c_style_cast( any_type(), proto::any() ),
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &expr = boost::get< proto::node::cast >( node_.get() );
          const auto value = evaluator( expr.args()[ 0 ] );
          return c_style_cast( context, ir_builder, expr.type(), value );
        }
      );
      evaluator.push(
        proto::if_( proto::any() )[ proto::any() ].else_[ proto::any() ],
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &expr = boost::get< proto::node::if_else >( node_.get() );
          namespace karma = boost::spirit::karma;
          const std::string if_then_name = std::string( "if.then" );
          const std::string if_else_name = std::string( "if.else" );
          const std::string if_end_name = std::string( "if.end" );
          const auto condition = evaluator( expr.condition() );
          llvm_basic_block.emplace_back(
            llvm::BasicBlock::Create( *context, if_then_name.c_str(), llvm_function.get() ),
            [context_]( llvm::BasicBlock* ){}
          );
          const auto if_then_block = llvm_basic_block.back();
          llvm_basic_block.emplace_back(
            llvm::BasicBlock::Create( *context, if_else_name.c_str(), llvm_function.get() ),
            [context_]( llvm::BasicBlock* ){}
          );
          const auto if_else_block = llvm_basic_block.back();
          llvm_basic_block.emplace_back(
            llvm::BasicBlock::Create( *context, if_end_name.c_str(), llvm_function.get() ),
            [context_]( llvm::BasicBlock* ){}
          );
          const auto if_end_block = llvm_basic_block.back();
          ir_builder->CreateCondBr( condition.llvm_value().get(), if_then_block.get(), if_else_block.get() );
          ir_builder->SetInsertPoint( if_then_block.get() );
          evaluator( expr.if_true() );
          ir_builder->CreateBr( if_end_block.get() );
          ir_builder->SetInsertPoint( if_else_block.get() );
          evaluator( expr.if_false() );
          ir_builder->CreateBr( if_end_block.get() );
          ir_builder->SetInsertPoint( if_end_block.get() );
          const auto type = get_type< void >();
          const auto llvm_type = get_llvm_type( context, type );
          return expression( type, llvm_type, std::shared_ptr< llvm::Value >( nullptr ) ); 
        }
      );
      evaluator.push(
        proto::while_( proto::any() )[ proto::any() ],
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &expr = boost::get< proto::node::while_ >( node_.get() );
          namespace karma = boost::spirit::karma;
          const std::string while_cond_name = std::string( "while.cond" );
          const std::string while_body_name = std::string( "while.body" );
          const std::string while_end_name = std::string( "while.end" );
          llvm_basic_block.emplace_back(
            llvm::BasicBlock::Create( *context, while_cond_name.c_str(), llvm_function.get() ),
            [context_]( llvm::BasicBlock* ){}
          );
          const auto while_cond_block = llvm_basic_block.back();
          llvm_basic_block.emplace_back(
            llvm::BasicBlock::Create( *context, while_body_name.c_str(), llvm_function.get() ),
            [context_]( llvm::BasicBlock* ){}
          );
          const auto while_body_block = llvm_basic_block.back();
          llvm_basic_block.emplace_back(
            llvm::BasicBlock::Create( *context, while_end_name.c_str(), llvm_function.get() ),
            [context_]( llvm::BasicBlock* ){}
          );
          const auto while_end_block = llvm_basic_block.back();
          ir_builder->CreateBr( while_cond_block.get() );
          ir_builder->SetInsertPoint( while_cond_block.get() );
          const auto condition = evaluator( expr.condition() );
          ir_builder->CreateCondBr( condition.llvm_value().get(), while_body_block.get(), while_end_block.get() );
          ir_builder->SetInsertPoint( while_body_block.get() );
          evaluator( expr.if_true() );
          ir_builder->CreateBr( while_cond_block.get() );
          ir_builder->SetInsertPoint( while_end_block.get() );
          const auto type = get_type< void >();
          const auto llvm_type = get_llvm_type( context, type );
          return expression( type, llvm_type, std::shared_ptr< llvm::Value >( nullptr ) ); 
        }
      );
      evaluator.push(
        proto::for_( proto::any(), proto::any(), proto::any() )[ proto::any() ],
        [&] ( const proto::eval< expression > &, const proto::node &node_ ) -> boost::optional< expression > {
          const auto &expr = boost::get< proto::node::for_ >( node_.get() );
          namespace karma = boost::spirit::karma;
          const std::string for_cond_name = std::string( "for.cond" );
          const std::string for_body_name = std::string( "for.body" );
          const std::string for_inc_name = std::string( "for.inc" );
          const std::string for_end_name = std::string( "for.end" );
          llvm_basic_block.emplace_back(
            llvm::BasicBlock::Create( *context, for_cond_name.c_str(), llvm_function.get() ),
            [context_]( llvm::BasicBlock* ){}
          );
          const auto for_cond_block = llvm_basic_block.back();
          llvm_basic_block.emplace_back(
            llvm::BasicBlock::Create( *context, for_body_name.c_str(), llvm_function.get() ),
            [context_]( llvm::BasicBlock* ){}
          );
          const auto for_body_block = llvm_basic_block.back();
          llvm_basic_block.emplace_back(
            llvm::BasicBlock::Create( *context, for_inc_name.c_str(), llvm_function.get() ),
            [context_]( llvm::BasicBlock* ){}
          );
          const auto for_inc_block = llvm_basic_block.back();
          llvm_basic_block.emplace_back(
            llvm::BasicBlock::Create( *context, for_end_name.c_str(), llvm_function.get() ),
            [context_]( llvm::BasicBlock* ){}
          );
          const auto for_end_block = llvm_basic_block.back();
          evaluator( expr.initialize() );
          ir_builder->CreateBr( for_cond_block.get() );
          ir_builder->SetInsertPoint( for_cond_block.get() );
          const auto condition = evaluator( expr.condition() );
          ir_builder->CreateCondBr( condition.llvm_value().get(), for_body_block.get(), for_end_block.get() );
          ir_builder->SetInsertPoint( for_body_block.get() );
          evaluator( expr.if_true() );
          ir_builder->CreateBr( for_inc_block.get() );
          ir_builder->SetInsertPoint( for_inc_block.get() );
          evaluator( expr.each() );
          ir_builder->CreateBr( for_cond_block.get() );
          ir_builder->SetInsertPoint( for_end_block.get() );
          const auto type = get_type< void >();
          const auto llvm_type = get_llvm_type( context, type );
          return expression( type, llvm_type, std::shared_ptr< llvm::Value >( nullptr ) ); 
        }
      );
    }
    void function::operator[]( const proto::node &expr ) const {
      ir_builder->SetInsertPoint( llvm_basic_block.back().get() );
      const auto final_value = evaluator( expr );
      if( function_type.result_type == final_value.type() )
        ir_builder->CreateRet( final_value.llvm_value().get() );
      else if( !type_traits::is_void( function_type.result_type ) ) {
        const auto converted_final_value = implicit_cast( context, ir_builder, function_type.result_type, final_value );
        ir_builder->CreateRet( converted_final_value.llvm_value().get() );
      }
      else
        ir_builder->CreateRetVoid();
    }
  }
}

