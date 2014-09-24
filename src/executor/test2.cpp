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
  module.create_function< void( int&, volatile int&, int ) >()[
    for_( param( 0 ) = 0, param( 0 ) != param( 2 ), ++param( 0 ) )[
      param( 1 ) *= 2
    ]
  ];
  dlambda::executor::module executable( module, "x86_64-pc-linux" );
  const auto f = executable.get_function< void( int&, volatile int&, int ) >();
  int count = 0;
  int value = 3;
  f( count, value, 10 );
  std::cout << value << std::endl;
}

