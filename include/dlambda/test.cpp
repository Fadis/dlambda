
#include <boost/fusion/container/vector.hpp>
#include <dlambda/get_llvm_type.hpp>

int main() {
  const auto type = dlambda::get_type< std::array< uint32_t, 5 >*& >();
  std::shared_ptr< llvm::LLVMContext > llvm_context( new llvm::LLVMContext() );
  const auto llvm_type = dlambda::get_llvm_type( llvm_context, type );
  llvm_type->dump();
}


