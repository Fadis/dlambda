add_library(
  dlambda_proto
  SHARED
  match.cpp
)
target_link_libraries( dlambda_proto ${Boost_LIBRARIES} ${LLVM_LIBS} dlambda_type_traits )
set_target_properties( dlambda_proto PROPERTIES
	  VERSION 0.0.1
	  SOVERSION 0.0.1
	)
add_executable(
  test
  test.cpp
)
target_link_libraries( test ${Boost_LIBRARIES} ${LLVM_LIBS} dlambda_type_traits dlambda_proto )
add_executable(
  test2
  test2.cpp
)
target_link_libraries( test2 ${Boost_LIBRARIES} ${LLVM_LIBS} dlambda_type_traits dlambda_proto )
