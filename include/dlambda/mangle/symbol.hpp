#ifndef DLAMBDA_MANGLE_SYMBOL_HPP
#define DLAMBDA_MANGLE_SYMBOL_HPP

namespace dlambda {
  namespace mangle {
    struct symbol {
      type::type value_type;
      std::vector< std::string > name;
    };
  }
}

#endif
