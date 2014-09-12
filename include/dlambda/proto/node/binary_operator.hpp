#ifndef DLAMBDA_PROTO_BINARY_OPERATOR_HPP
#define DLAMBDA_PROTO_BINARY_OPERATOR_HPP

#include <array>
#include <dlambda/proto/operators.hpp>
#include <boost/preprocessor.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node >
      class binary_operator {
      public:
        enum class tag {
          BOOST_PP_SEQ_FOR_EACH(
            DLAMBDA_PROTO_OPERATOR_TAG,
            unused,
            DLAMBDA_PROTO_BINARY_OPERATORS
          )
          comma
        };
        binary_operator( const tag &oper, const std::array< Node, 2u > &args__ ) : operator_( oper ), args_( args__ ) {}
        binary_operator( const tag &oper,  std::array< Node, 2u > &&args__  ) : operator_( oper ), args_( args__ ) {}
        binary_operator( const binary_operator & ) = default;
        binary_operator( binary_operator< Node > && ) = default;
        binary_operator &operator=( const binary_operator< Node > & ) = default;
        binary_operator &operator=( binary_operator< Node > && ) = default;
        tag get_operator() const { return operator_; }
        const std::array< Node, 2u > &args() const { return args_; }
      private:
        tag operator_;
        std::array< Node, 2u > args_;
      };
    }
  }
}

#endif


