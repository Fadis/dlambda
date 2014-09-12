#ifndef DLAMBDA_PROTO_TERNARY_OPERATOR_HPP
#define DLAMBDA_PROTO_TERNARY_OPERATOR_HPP

#include <array>
#include <dlambda/proto/operators.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node >
      class ternary_operator {
      public:
        ternary_operator( const std::array< Node, 3u > &args__ ) : args_( args__ ) {}
        ternary_operator( std::array< Node, 3u > &&args__  ) : args_( args__ ) {}
        ternary_operator( const ternary_operator & ) = default;
        ternary_operator( ternary_operator< Node > && ) = default;
        ternary_operator &operator=( const ternary_operator< Node > & ) = default;
        ternary_operator &operator=( ternary_operator< Node > && ) = default;
        const std::array< Node, 3u > &args() const { return args_; }
      private:
        std::array< Node, 3u > args_;
      };
    }
  }
}

#endif


