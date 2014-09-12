#ifndef DLAMBDA_PROTO_CAST_HPP
#define DLAMBDA_PROTO_CAST_HPP

#include <dlambda/type.hpp>
#include <dlambda/proto/node/make_node_fwd.hpp>

namespace dlambda {
  namespace proto {
    namespace detail {
      template< typename Node >
      class cast {
      public:
        enum class cast_type_type {
          c_style,
          static_,
          const_,
          dynamic,
          reinterpret
        };
        cast( const cast_type_type &cast_type__, const dlambda::type &type__, const std::array< Node, 1u > &args__ ) : cast_type_( cast_type__ ), type_( type__ ), args_( args__ ) {}
        cast( const cast_type_type &cast_type__, const dlambda::type &type__, std::array< Node, 1u > &&args__ ) : cast_type_( cast_type__ ), type_( type__ ), args_( std::move( args__ ) ) {}
        cast( const cast_type_type &cast_type__, dlambda::type &&type__, std::array< Node, 1u > &&args__ ) : cast_type_( cast_type__ ), type_( std::move( type__ ) ), args_( std::move( args__ ) ) {}
        cast( const cast< Node > & ) = default;
        cast( cast< Node > && ) = default;
        cast &operator=( const cast< Node > & ) = default;
        cast &operator=( cast< Node > && ) = default;
        const cast_type_type &cast_type() const { return cast_type_; }
        const dlambda::type &type() const { return type_; }
        const std::array< Node, 1u > args() const { return args_; } 
      private:
        cast_type_type cast_type_;
        dlambda::type type_;
        std::array< Node, 1u > args_;
      };
    }
  }
}

#endif


