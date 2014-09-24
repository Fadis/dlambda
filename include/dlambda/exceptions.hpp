#ifndef DLAMBDA_EXCEPTIONS_HPP
#define DLAMBDA_EXCEPTIONS_HPP

namespace dlambda {
  namespace exceptions {
    struct unexpected_type {};
    struct invalid_type_conversion {};
    struct invalid_value {};
    struct invalid_cast {};
    struct invalid_expression {};
    struct not_implemented {};
    struct no_such_function {};
    struct argument_out_of_range {};
    struct invalid_module {};
    struct internal_compiler_error {};
    struct unsupported_target_machine {};
    struct llvm_compilation_failed {};
  }
}

#endif

