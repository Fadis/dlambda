#include <iostream>
#include <dlambda/proto/node.hpp>
#include <dlambda/proto/node/match.hpp>
#include <dlambda/proto/node/get_top_operator_tag.hpp>
#include <dlambda/proto/eval.hpp>
#include <dlambda/compiler/compiler.hpp>
#include <dlambda/compiler/get_llvm_constant_value.hpp>

#include <llvm/IR/LLVMContext.h>

int main() {
  using namespace dlambda::proto;
  std::shared_ptr< llvm::LLVMContext > llvm_context(
    new llvm::LLVMContext()
  );
//  int a = 3;
  dlambda::compiler::module module( llvm_context );
/*  module.create_function< int( int, char ) >( "a1" )[
    lit( 3 ) + 0.1f
  ];
  module.create_function< int&( int& ) >( "a2" )[
    ++ param( 0 )
  ];
  module.create_function< bool( float ) >( "a3" )[
    param( 0 ) >= 3
  ];
  module.create_function< int*( int, char ) >( "a4" )[
    lit( &a ) + param( 1 )
  ];*/
  module.create_function< void( int&, int, int, int, int& ) >( "a5" )[
    if_( param( 1 ) > 10 )[
      if_( param( 1 ) > 20 )[
        param( 0 ) = param( 2 ) * 2
      ].else_[
        param( 0 ) = param( 2 )
      ]
    ].else_[
      param( 0 ) = param( 3 )
    ],
    while_( param( 0 ) < param( 1 ) )[
      ++param( 0 )
    ],
    for_( param( 0 ) = 0, param( 0 ) != 10, ++param( 0 ) )[
      param( 4 ) *= 3
    ]
  ];
  module.dump();
  /*
  dlambda::compiler::compile< int( int, char ) >( "a1", ( lit( 3 ) + 0.1f ), llvm_context );
  dlambda::compiler::compile< int( int, char ) >( "a2", ( lit( 3 ) + param( 1 ) ), llvm_context );
  dlambda::compiler::compile< int*( int, char ) >( "a3", lit( &a ), llvm_context );
  dlambda::compiler::compile< int*( int, char ) >( "a4", ( lit( &a ) + param( 1 ) ), llvm_context );
  dlambda::compiler::compile< int*( int, char ) >( "a5", ( + param( 0 ) ), llvm_context );
  dlambda::compiler::compile< int&( int& ) >( "a6", ( ++ param( 0 ) ), llvm_context );
  dlambda::compiler::compile< int( int& ) >( "a7", ( param( 0 ) -- ), llvm_context );
  dlambda::compiler::compile< uint64_t( uint64_t, int ) >( "a8", ( param( 0 ) << param( 1 ) ), llvm_context );
  dlambda::compiler::compile< bool( float ) >( "a9", ( param( 0 ) >= 3 ), llvm_context );
  dlambda::compiler::compile< bool( int, char ) >( "aa", ( param( 0 ) < param( 1 ) ), llvm_context );
  dlambda::compiler::compile< bool( bool, bool ) >( "ab", ( param( 0 ) || param( 1 ) ), llvm_context );
  dlambda::compiler::compile( ( lit( 3 ) - 4 ), llvm_context, argument_types );
  dlambda::compiler::compile( ( lit( 3 ) * 4 ), llvm_context, argument_types );
  dlambda::compiler::compile( ( lit( 3 ) / 4 ), llvm_context, argument_types );
  dlambda::compiler::compile( ( lit( 3 ) % 4 ), llvm_context, argument_types );*/
}

