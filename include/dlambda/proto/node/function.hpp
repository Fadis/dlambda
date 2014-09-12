#ifndef DLAMBDA_PROTO_FUNCTION_HPP
#define DLAMBDA_PROTO_FUNCTION_HPP

#include <vector>
#include <dlambda/proto/operators.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node >
      struct function {
      public:
        function( const Node &callee_, const std::vector< Node > &args__ ) : callee( callee_ ), args_( args__ ) {}
        function( const Node &callee_, std::vector< Node > &&args__  ) : callee( callee_ ), args_( args__ ) {}
        function( Node &&callee_, std::vector< Node > &&args__  ) : callee( callee_ ), args_( args__ ) {}
        function( const function & ) = default;
        function( function< Node > && ) = default;
        function &operator=( const function< Node > & ) = default;
        function &operator=( function< Node > && ) = default;
        const Node &get_callee() const { return callee; }
        const std::vector< Node > &args() const { return args_; }
      private:
        Node callee;
        std::vector< Node > args_;
      };
    }
  }
}

#endif


