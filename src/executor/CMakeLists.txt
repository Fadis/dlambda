add_library(
  dlambda_executor
  SHARED
  executor.cpp
)
target_link_libraries( dlambda_executor ${Boost_LIBRARIES} dlambda_type_traits
  dlambda_proto dlambda_compiler -lLLVMTransformUtils -lLLVMipa -lLLVMAnalysis
  -lLLVMTarget -lLLVMMC -lLLVMObject -lLLVMCore )
set_target_properties( dlambda_executor PROPERTIES
	  VERSION 0.0.1
	  SOVERSION 0.0.1
)
add_executable(
  executor_test
  test.cpp
)
target_link_libraries( executor_test ${Boost_LIBRARIES} ${LLVM_ALL_LIBS} dlambda_type_traits dlambda_proto dlambda_compiler dlambda_executor )
