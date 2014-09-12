#ifndef DLAMBDA_PROTO_FOR_HPP
#define DLAMBDA_PROTO_FOR_HPP

#include <dlambda/proto/node/make_node_fwd.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node >
      class for_ {
      public:
        template< typename Init, typename Cond, typename Each >
        explicit for_(
          const Init &init__, const Cond &cond__, const Each &each__
        ) :
          initialize_( make_node< Node, Init >()( init__ ) ),
          condition_( make_node< Node, Cond >()( cond__ ) ),
          each_( make_node< Node, Each >()( each__ ) ),
          if_true_( make_node< Node, int >()( 0 ) ) {}
        template< typename Init, typename Cond, typename Each >
        explicit for_(
          Init &&init__, Cond &&cond__, Each &&each__
        ) :
          initialize_( make_node< Node, Init >()( init__ ) ),
          condition_( make_node< Node, Init >()( cond__ ) ),
          each_( make_node< Node, Init >()( each__ ) ),
          if_true_( make_node< Node, int >()( 0 ) ) {}
        for_( const for_< Node > & ) = default;
        for_( for_< Node > && ) = default;
        for_ &operator=( const for_< Node > & ) = default;
        for_ &operator=( for_< Node > && ) = default;
        template< typename T >
        for_ &operator[]( const T &if_true__ ) {
          if_true_.alias( make_node< Node, T >()( if_true__ ) );
          return *this; 
        }
        const Node &initialize() const { return initialize_; }
        const Node &condition() const { return condition_; }
        const Node &each() const { return each_; }
        const Node &if_true() const { return if_true_; }
      private:
        Node initialize_;
        Node condition_;
        Node each_;
        Node if_true_;
      };
    }
  }
}

#endif


