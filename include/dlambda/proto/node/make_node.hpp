#ifndef DLAMBDA_PROTO_MAKE_NODE_HPP
#define DLAMBDA_PROTO_MAKE_NODE_HPP

#include <boost/utility/enable_if.hpp>
#include <boost/mpl/not.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <dlambda/proto/node/literal.hpp>
#include <dlambda/proto/node/make_node_fwd.hpp>
#include <dlambda/proto/is_node.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node, typename T >
      struct make_raw_node<
        Node, T,
        typename boost::enable_if< boost::is_same< typename boost::remove_reference< typename boost::remove_cv< T >::type >::type, Node > >::type
      > {
        typename Node::raw_node operator()( const typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &value ) const {
          return value.get();
        }
        typename Node::raw_node operator()( typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &&value ) const {
          return value.get();
        }
      };
      template< typename Node, typename T >
      struct make_raw_node<
        Node, T,
        typename boost::enable_if<
          boost::mpl::and_<
            is_node<
              Node,
              typename boost::remove_reference<
                typename boost::remove_cv< T >::type
              >::type
            >,
            boost::mpl::not_<
              boost::is_same< T, if_< Node > >
            >
          >
        >::type
      > {
        typename Node::raw_node operator()( const typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &value ) const {
          return typename Node::raw_node( value );
        }
        typename Node::raw_node operator()( typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &&value ) const {
          return typename Node::raw_node( std::move( value ) );
        }
      };
      template< typename Node, typename T >
      struct make_raw_node<
        Node, T,
        typename boost::enable_if<
          boost::is_same< T, if_< Node > >
        >::type
      > {
        typename Node::raw_node operator()( const typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &value ) const {
          return typename Node::raw_node( static_cast< if_else< Node > >( value ) );
        }
        typename Node::raw_node operator()( typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &&value ) const {
          return typename Node::raw_node( static_cast< if_else< Node > >( value ) );
        }
      };
      template< typename Node, typename T >
      struct make_raw_node<
        Node, T,
        typename boost::enable_if< boost::mpl::not_<
          boost::mpl::or_<
            boost::is_same< typename boost::remove_reference< typename boost::remove_cv< T >::type >::type, Node >,
            is_node< Node, typename boost::remove_reference< typename boost::remove_cv< T >::type >::type >
          >
        > >::type
      > {
        typename Node::raw_node operator()( const typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &value ) const {
          return typename Node::raw_node( lit( value ) );
        }
        typename Node::raw_node operator()( typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &&value ) const {
          return typename Node::raw_node( lit( std::move( value ) ) );
        }
      };
      template< typename Node, typename T >
      struct make_node<
        Node, T,
        typename boost::enable_if< boost::is_same< typename boost::remove_reference< typename boost::remove_cv< T >::type >::type, Node > >::type
      > {
        Node operator()( const typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &value ) const {
          return value;
        }
        Node operator()( typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &&value ) const {
          return value;
        }
      };
      template< typename Node, typename T >
      struct make_node<
        Node, T,
        typename boost::enable_if<
          boost::mpl::and_<
            is_node<
              Node,
              typename boost::remove_reference<
                typename boost::remove_cv< T >::type
              >::type
            >,
            boost::mpl::not_<
              boost::is_same< T, if_< Node > >
            >
          >
        >::type
      > {
        Node operator()( const typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &value ) const {
          return Node( value );
        }
        Node operator()( typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &&value ) const {
          return Node( std::move( value ) );
        }
      };
      template< typename Node, typename T >
      struct make_node<
        Node, T,
        typename boost::enable_if<
          boost::is_same< T, if_< Node > >
        >::type
      > {
        Node operator()( const typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &value ) const {
          return Node( value.else_ );
        }
        Node operator()( typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &&value ) const {
          return Node( std::move( value.else_ ) );
        }
      };
      template< typename Node, typename T >
      struct make_node<
        Node, T,
        typename boost::enable_if< boost::mpl::not_<
          boost::mpl::or_<
            boost::is_same< typename boost::remove_reference< typename boost::remove_cv< T >::type >::type, Node >,
            is_node< Node, typename boost::remove_reference< typename boost::remove_cv< T >::type >::type >
          >
        > >::type
      > {
        Node operator()( const typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &value ) const {
          return Node( literal( value ) );
        }
        Node operator()( typename boost::remove_reference< typename boost::remove_cv< T >::type >::type &&value ) const {
          return Node( literal( std::move( value ) ) );
        }
      };
    }
  }
}

#endif


