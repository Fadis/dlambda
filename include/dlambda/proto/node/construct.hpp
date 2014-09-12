#ifndef DLAMBDA_PROTO_CONSTRUCT_HPP
#define DLAMBDA_PROTO_CONSTRUCT_HPP

#include <dlambda/type.hpp>
#include <dlambda/proto/node/make_node_fwd.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node >
      class construct {
      public:
        construct( const dlambda::type &type__, const std::vector< Node > &args__ ) : type_( type__ ), args_( args__ ) {}
        construct( const dlambda::type &type__, std::vector< Node > &&args__ ) : type_( type__ ), args_( std::move( args__ ) ) {}
        construct( dlambda::type &&type__, std::vector< Node > &&args__ ) : type_( std::move( type__ ) ), args_( std::move( args__ ) ) {}
        construct( const construct< Node > & ) = default;
        construct( construct< Node > && ) = default;
        construct &operator=( const construct< Node > & ) = default;
        construct &operator=( construct< Node > && ) = default;
        const dlambda::type &type() const { return type_; }
        const std::vector< Node > args() const { return args_; } 
      private:
        dlambda::type type_;
        std::vector< Node > args_;
      };
    }
  }
}

#endif


