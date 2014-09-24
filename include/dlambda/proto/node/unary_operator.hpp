#ifndef DLAMBDA_PROTO_UNARY_OPERATOR_HPP
#define DLAMBDA_PROTO_UNARY_OPERATOR_HPP

#include <array>
#include <dlambda/proto/operators.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node >
      class unary_operator {
      public:
        enum class tag {
          BOOST_PP_SEQ_FOR_EACH(
            DLAMBDA_PROTO_OPERATOR_TAG,
            unused,
            DLAMBDA_PROTO_UNARY_OPERATORS
          )
        };
        unary_operator( const tag &oper, const std::array< Node, 1u > &args__ ) : operator_( oper ), args_( args__ ) {}
        unary_operator( const tag &oper,  std::array< Node, 1u > &&args__  ) : operator_( oper ), args_( args__ ) {}
        unary_operator( const unary_operator & ) = default;
        unary_operator( unary_operator< Node > && ) = default;
        unary_operator &operator=( const unary_operator< Node > & ) = default;
        unary_operator &operator=( unary_operator< Node > && ) = default;
        tag get_operator() const { return operator_; }
        std::array< Node, 1u > args() const { return args_; }
      private:
        tag operator_;
        std::array< Node, 1u > args_;
      };
    }
  }
}

#endif


