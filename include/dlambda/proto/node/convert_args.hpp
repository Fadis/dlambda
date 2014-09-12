#ifndef DLAMBDA_PROTO_CONVERT_ARGS_HPP
#define DLAMBDA_PROTO_CONVERT_ARGS_HPP

#include <vector>
#include <dlambda/proto/node/make_node_fwd.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node, typename Head >
      void convert_args_( std::vector< Node > &converted, const Head &head ) {
        converted.emplace_back( make_node< Node, Head >()( head ) );
      }
      template< typename Node, typename Head >
      void convert_args_( std::vector< Node > &converted, Head &&head ) {
        converted.emplace_back( make_node< Node, Head >()( head ) );
      }
      template< typename Node, typename Head, typename ... Tail >
      void convert_args_( std::vector< Node > &converted, const Head &head, const Tail & ... tail ) {
        converted.emplace_back( make_node< Node, Head >()( head ) );
        convert_args_( converted, tail... );
      }
      template< typename Node, typename Head, typename ... Tail >
      void convert_args_( std::vector< Node > &converted, Head &&head, Tail && ... tail ) {
        converted.emplace_back( make_node< Node, Head >()( head ) );
        convert_args_( converted, tail... );
      }
      template< typename Node, typename ... Args >
      std::vector< Node > convert_args( const Args & ... args ) {
        std::vector< Node > converted;
        convert_args_( converted, args... );
        return std::move( converted );
      }
      template< typename Node, typename ... Args >
      std::vector< Node > convert_args( Args && ... args ) {
        std::vector< Node > converted;
        convert_args_( converted, args... );
        return std::move( converted );
      }
    }
  }
}

#endif


