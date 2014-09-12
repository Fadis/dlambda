#ifndef DLAMBDA_PROTO_NODE_TERMINAL_HPP
#define DLAMBDA_PROTO_NODE_TERMINAL_HPP

#include <dlambda/type.hpp>

namespace dlambda {
  namespace proto {
    class terminal {
    public:
      terminal( const type &value_type__ ) : value_type_( value_type__ ) {}
      terminal( const terminal & ) = default;
      terminal( terminal && ) = default;
      terminal &operator=( const terminal & ) = default;
      terminal &operator=( terminal && ) = default;
      const dlambda::type &type() const { return value_type_; } 
    private:
      dlambda::type value_type_;
    };
  }
}

#endif


