#include <iostream>
#include <llvm/Support/TargetSelect.h>

#include <dlambda/proto/node.hpp>
#include <dlambda/proto/node/match.hpp>
#include <dlambda/proto/node/get_top_operator_tag.hpp>
#include <dlambda/proto/eval.hpp>
#include <dlambda/compiler/compiler.hpp>
#include <dlambda/compiler/get_llvm_constant_value.hpp>
#include <dlambda/executor/executor.hpp>

#include <llvm/IR/LLVMContext.h>

int main() {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  using namespace dlambda::proto;
  std::shared_ptr< llvm::LLVMContext > llvm_context( new llvm::LLVMContext() );
  dlambda::compiler::module module( llvm_context );
  module.create_function< float( int, char ) >( "a1" )[
    lit( 3 ) + 0.1f
  ];
  module.create_function< int&( int& ) >( "a2" )[
    ++ param( 0 )
  ];
  module.create_function< void( int&, int, int, int, int& ) >( "a3" )[
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
  dlambda::executor::module executable( module, "x86_64-pc-linux" );
  const auto a1 = executable.get_function< float( int, char ) >( "a1" );
  const auto a2 = executable.get_function< int&( int& ) >( "a2" );
  std::cout << a1( 3, 'a' ) << std::endl;
  int a = 1;
  std::cout << a2( a ) << std::endl;
  std::cout << a << std::endl;
}

