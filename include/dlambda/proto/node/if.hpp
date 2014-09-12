#ifndef DLAMBDA_PROTO_IF_HPP
#define DLAMBDA_PROTO_IF_HPP

#include <dlambda/proto/node/make_node_fwd.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node >
      class if_;
      template< typename Node >
      class if_else {
      public:
        friend class if_< Node >;
        template< typename T >
        explicit if_else( const T &condition__ ) :
          condition_( make_node< Node, T >()( condition__ ) ),
          if_true_( make_node< Node, int >()( 0 ) ),
          if_false_( make_node< Node, int >()( 0 ) ) {}
        template< typename T >
        explicit if_else( T &&condition__ ) :
          condition_( make_node< Node, T >()( condition__ ) ),
          if_true_( make_node< Node, int >()( 0 ) ),
          if_false_( make_node< Node, int >()( 0 ) ) {}
        if_else( const if_else< Node > & ) = default;
        if_else( if_else< Node > && ) = default;
        if_else &operator=( const if_else< Node > & ) = default;
        if_else &operator=( if_else< Node > && ) = default;
        template< typename T >
        if_else &operator[]( const T &if_false__ ) {
          if_false_.alias( make_node< Node, T >()( if_false__ ) );
          return *this; 
        }
        operator Node () const {
          return Node( *this );
        }
        const Node &condition() const { return condition_; }
        const Node &if_true() const { return if_true_; }
        const Node &if_false() const { return if_false_; }
      private:
        Node condition_;
        Node if_true_;
        Node if_false_;
      };
      template< typename Node >
      class if_ {
      public:
        template< typename T >
        explicit if_( const T &cond_ ) : else_( cond_ ) {}
        template< typename T >
        explicit if_( T &&cond_ ) : else_( cond_ ) {}
        if_( const if_< Node > & ) = default;
        if_( if_< Node > && ) = default;
        if_ &operator=( const if_< Node > & ) = default;
        if_ &operator=( if_< Node > && ) = default;
        template< typename T >
        if_ &operator[]( const T &if_true_ ) {
          else_.if_true_.alias( if_true_ );
          return *this; 
        }
        if_else< Node > else_;
        operator Node () const {
          return else_;
        }
      };
    }
  }
}

#endif


