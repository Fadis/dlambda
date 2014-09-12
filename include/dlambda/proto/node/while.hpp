#ifndef DLAMBDA_PROTO_WHILE_HPP
#define DLAMBDA_PROTO_WHILE_HPP

#include <dlambda/proto/node/make_node_fwd.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node >
      class while_ {
      public:
        template< typename T >
        explicit while_( const T &cond ) : condition_( make_node< Node, T >()( cond ) ), if_true_( make_node< Node, int >()( 0 ) ) {}
        template< typename T >
        explicit while_( T &&cond ) : condition_( make_node< Node, T >()( std::move( cond ) ) ), if_true_( make_node< Node, int >()( 0 ) ) {}
        while_( const while_< Node > & ) = default;
        while_( while_< Node > && ) = default;
        while_ &operator=( const while_< Node > & ) = default;
        while_ &operator=( while_< Node > && ) = default;
        template< typename T >
        while_ &operator[]( const T &if_true__ ) {
          if_true_.alias( make_node< Node, T >()( if_true__ ) );
          return *this; 
        }
        const Node &condition() const { return condition_; }
        const Node &if_true() const { return if_true_; }
      private:
        Node condition_;
        Node if_true_;
      };
    }
  }
}

#endif


