#ifndef DLAMBDA_PROTO_NODE_HPP
#define DLAMBDA_PROTO_NODE_HPP

#include <boost/variant.hpp>
#include <dlambda/proto/node/literal.hpp>
#include <dlambda/proto/node/placeholder.hpp>
#include <dlambda/proto/operators.hpp>
#include <dlambda/proto/node/unary_operator.hpp>
#include <dlambda/proto/node/binary_operator.hpp>
#include <dlambda/proto/node/ternary_operator.hpp>
#include <dlambda/proto/node/function.hpp>
#include <dlambda/proto/node/make_node_fwd.hpp>
#include <dlambda/proto/node/if.hpp>
#include <dlambda/proto/node/while.hpp>
#include <dlambda/proto/node/for.hpp>
#include <dlambda/proto/node/construct.hpp>
#include <dlambda/proto/node/cast.hpp>
#include <dlambda/proto/is_node.hpp>
#include <dlambda/proto/node/make_node.hpp>
#include <dlambda/proto/node/convert_args.hpp>
#include <dlambda/proto/node/terminal.hpp>
#include <dlambda/proto/node/any.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/filter.hpp>

namespace dlambda {
  namespace proto {
    class node {
    public:
      typedef boost::variant<
        detail::any_operator,
        terminal,
        literal,
        placeholder,
        detail::unary_operator< node >,
        detail::binary_operator< node >,
        detail::ternary_operator< node >,
        detail::function< node >,
        detail::if_else< node >,
        detail::while_< node >,
        detail::for_< node >,
        detail::construct< node >,
        detail::cast< node >
      > raw_node;
      typedef detail::any_operator any;
      typedef detail::unary_operator< node > unary_operator;
      typedef detail::binary_operator< node > binary_operator;
      typedef detail::ternary_operator< node > ternary_operator;
      typedef detail::function< node > function;
      typedef detail::if_else< node > if_else;
      typedef detail::while_< node > while_;
      typedef detail::for_< node > for_;
      typedef detail::construct< node > construct;
      typedef detail::cast< node > cast;
      template< typename T >
      static node make_node( const T &value ) { return detail::make_node< node, T >()( value ); }
      template< typename T >
      static node make_node( T &&value ) { return detail::make_node< node, T >()( std::move( value ) ); }
      template< typename T >
      static raw_node make_raw_node( const T &value ) { return detail::make_raw_node< node, T >()( value ); }
      template< typename T >
      static raw_node make_raw_node( T &&value ) { return detail::make_node< raw_node, T >()( std::move( value ) ); }
      node( const node &src ) : raw( new raw_node( detail::make_raw_node< node, node >()( src ) ) ) {
      }
      node( node &&src ) : raw( new raw_node( detail::make_raw_node< node, node >()( src ) ) ) {
      }
      template< typename T, typename Enable = typename boost::enable_if< boost::mpl::not_< boost::is_same< node, T > > >::type >
      node( const T &src ) : raw( new raw_node( detail::make_raw_node< node, T >()( src ) ) ) {
      }
      template< typename T, typename Enable = typename boost::enable_if< boost::mpl::not_< boost::is_same< node, T > > >::type >
      node( T &&src ) : raw( new raw_node( detail::make_raw_node< node, T >()( std::move( src ) ) ) ) {
      }
      void alias( const node &src ) {
        raw = src.raw;
      }
      void alias( node &&src ) {
        raw = std::move( src.raw );
      }
      const raw_node &get() const { return *raw; }
      BOOST_PP_SEQ_FOR_EACH(
        DLAMBDA_PROTO_UNARY_OPERATOR_INSIDE,
        unused,
        BOOST_PP_SEQ_FILTER(
          DLAMBDA_PROTO_OPERATOR_SHOULD_BE_DEFINED_INSIDE,
          unused,
          DLAMBDA_PROTO_UNARY_OPERATORS
        )
      )
      node operator=( const node &right ) const {
        return node(
          detail::binary_operator< node >(
            detail::binary_operator< node > :: tag :: assign, std::array< node, 2u >{{ *this, right }}
          )
        );
      }
      node operator=( node &&right ) const {
        return node(
          detail::binary_operator< node >(
            detail::binary_operator< node > :: tag :: assign, std::array< node, 2u >{{ *this, std::move( right ) }}
          )
        );
      }
      template< typename T, typename Enable = typename boost::enable_if< boost::mpl::not_< boost::is_same< node, T > > >::type >
      node operator=( const T &right ) const {
        return node(
          detail::binary_operator< node >(
            detail::binary_operator< node > :: tag :: assign, std::array< node, 2u >{{ *this, make_node( right ) }}
          )
        );
      }
      template< typename T, typename Enable = typename boost::enable_if< boost::mpl::not_< boost::is_same< node, T > > >::type >
      node operator=( T &&right ) const {
        return node(
          detail::binary_operator< node >(
            detail::binary_operator< node > :: tag :: assign, std::array< node, 2u >{{ *this, make_node( std::move( right ) ) }}
          )
        );
      }
      template< typename T >
      node operator[]( const T &right ) const {
        return node(
          detail::binary_operator< node >(
            detail::binary_operator< node > :: tag :: subscript, std::array< node, 2u >{{ *this, make_node( right ) }}
          )
        );
      }
      template< typename T >
      node operator[]( T &&right ) const {
        return node(
          detail::binary_operator< node >(
            detail::binary_operator< node > :: tag :: subscript, std::array< node, 2u >{{ *this, make_node( std::move( right ) ) }}
          )
        );
      }
      node operator()() const {
        return node(
          function( *this, std::vector< node >() )
        );
      }
      template< typename ... Args >
      node operator()( const Args & ... args ) const {
        return node(
          function( *this, detail::convert_args< node >( args... ) )
        );
      }
      template< typename ... Args >
      node operator()( Args && ... args ) const {
        return node(
          function( *this, detail::convert_args< node >( args... ) )
        );
      }
      const raw_node &get_raw() const {
        return *raw;
      }
    private:
      std::shared_ptr< raw_node > raw;
    };
    template< typename T >
    detail::if_< node > if_( const T &value ) {
      return detail::if_< node >( detail::make_node< node, T >()( value ) );
    }
    template< typename T >
    detail::if_< node > if_( T &&value ) {
      return detail::if_< node >( detail::make_node< node, T >()( std::move( value ) ) );
    }
    template< typename T >
    detail::while_< node > while_( const T &value ) {
      return detail::while_< node >( detail::make_node< node, T >()( value ) );
    }
    template< typename T >
    detail::while_< node > while_( T &&value ) {
      return detail::while_< node >( detail::make_node< node, T >()( std::move( value ) ) );
    }
    template< typename Init, typename Cond, typename Each >
    detail::for_< node > for_( const Init &init, const Cond &cond, const Each &each ) {
      return detail::for_< node >(
        detail::make_node< node, Init >()( init ),
        detail::make_node< node, Cond >()( cond ),
        detail::make_node< node, Each >()( each )
      );
    }
    template< typename Init, typename Cond, typename Each >
    detail::for_< node > for_( Init &&init, Cond &&cond, Each &&each ) {
      return detail::for_< node >(
        detail::make_node< node, Init >()( std::move( init ) ),
        detail::make_node< node, Cond >()( std::move( cond ) ),
        detail::make_node< node, Each >()( std::move( each ) )
      );
    }
    inline node any() {
      return detail::make_node< node, node::any >()( node::any() );
    }
    template< typename T >
    node lit( const T &value ) {
      return detail::make_node< node, literal >()( literal( value ) );
    }
    template< typename T >
    node ref( T &value ) {
      return detail::make_node< node, literal >()( literal( value ) );
    }
    template< typename T >
    node param( const T index ) {
      return detail::make_node< node, placeholder >()( placeholder( index ) );
    }
    template< typename Type >
    node construct( const Type &type_ ) {
      return detail::make_node< node, detail::construct< node > >()(
        detail::construct< node >( type( type_ ), std::vector< node >() )
      );
    }
    template< typename Type, typename ... Args >
    node construct( const Type &type_, const Args & ... args ) {
      return detail::make_node< node, detail::construct< node > >()(
        detail::construct< node >( type( type_ ), detail::convert_args< node >( args... ) )
      );
    }
    template< typename Type, typename ... Args >
    node construct( const Type &type_, Args && ... args ) {
      return detail::make_node< node, detail::construct< node > >()(
        detail::construct< node >( type( type_ ), detail::convert_args< node >( args... ) )
      );
    }
    template< typename Type, typename T >
    node static_cast_( const Type &type_, const T &value ) {
      return detail::make_node< node, detail::cast< node > >()(
        detail::cast< node >( detail::cast< node >::cast_type_type::static_, type( type_ ), std::array< node, 1u >{{ node::make_node( value ) }} )
      );
    }
    template< typename Type, typename T >
    node static_cast_( const Type &type_, T &&value ) {
      return detail::make_node< node, detail::cast< node > >()(
        detail::cast< node >( detail::cast< node >::cast_type_type::static_, type( type_ ), std::array< node, 1u >{{ node::make_node( value ) }} )
      );
    }
    template< typename Type, typename T >
    node const_cast_( const Type &type_, const T &value ) {
      return detail::make_node< node, detail::cast< node > >()(
        detail::cast< node >( detail::cast< node >::cast_type_type::const_, type( type_ ), std::array< node, 1u >{{ node::make_node( value ) }} )
      );
    }
    template< typename Type, typename T >
    node const_cast_( const Type &type_, T &&value ) {
      return detail::make_node< node, detail::cast< node > >()(
        detail::cast< node >( detail::cast< node >::cast_type_type::const_, type( type_ ), std::array< node, 1u >{{ node::make_node( value ) }} )
      );
    }
    template< typename Type, typename T >
    node dynamic_cast_( const Type &type_, const T &value ) {
      return detail::make_node< node, detail::cast< node > >()(
        detail::cast< node >( detail::cast< node >::cast_type_type::dynamic, type( type_ ), std::array< node, 1u >{{ node::make_node( value ) }} )
      );
    }
    template< typename Type, typename T >
    node dynamic_cast_( const Type &type_, T &&value ) {
      return detail::make_node< node, detail::cast< node > >()(
        detail::cast< node >( detail::cast< node >::cast_type_type::dynamic, type( type_ ), std::array< node, 1u >{{ node::make_node( value ) }} )
      );
    }
    template< typename Type, typename T >
    node reinterpret_cast_( const Type &type_, const T &value ) {
      return detail::make_node< node, detail::cast< node > >()(
        detail::cast< node >( detail::cast< node >::cast_type_type::reinterpret, type( type_ ), std::array< node, 1u >{{ node::make_node( value ) }} )
      );
    }
    template< typename Type, typename T >
    node reinterpret_cast_( const Type &type_, T &&value ) {
      return detail::make_node< node, detail::cast< node > >()(
        detail::cast< node >( detail::cast< node >::cast_type_type::reinterpret, type( type_ ), std::array< node, 1u >{{ node::make_node( value ) }} )
      );
    }
    template< typename Type, typename T >
    node c_style_cast( const Type &type_, const T &value ) {
      return detail::make_node< node, detail::cast< node > >()(
        detail::cast< node >( detail::cast< node >::cast_type_type::c_style, type( type_ ), std::array< node, 1u >{{ node::make_node( value ) }} )
      );
    }
    template< typename Type, typename T >
    node c_style_cast( const Type &type_, T &&value ) {
      return detail::make_node< node, detail::cast< node > >()(
        detail::cast< node >( detail::cast< node >::cast_type_type::c_style, type( type_ ), std::array< node, 1u >{{ node::make_node( value ) }} )
      );
    }
    template< typename T >
    struct is_node : public detail::is_node< node, T > {};
    BOOST_PP_SEQ_FOR_EACH(
      DLAMBDA_PROTO_UNARY_OPERATOR_OUTSIDE,
      unused,
      BOOST_PP_SEQ_FILTER(
        DLAMBDA_PROTO_OPERATOR_SHOULD_BE_DEFINED_OUTSIDE,
        unused,
        DLAMBDA_PROTO_UNARY_OPERATORS
      )
    )
    BOOST_PP_SEQ_FOR_EACH(
      DLAMBDA_PROTO_BINARY_OPERATOR_OUTSIDE,
      unused,
      BOOST_PP_SEQ_FILTER(
        DLAMBDA_PROTO_OPERATOR_SHOULD_BE_DEFINED_OUTSIDE,
        unused,
        DLAMBDA_PROTO_BINARY_OPERATORS
      )
    )
    template<
      typename Left, typename Right,
      typename Enable = typename boost::enable_if<
        boost::mpl::or_<
          is_node< Left >, is_node< Right >,
          boost::is_same< Left, node >,
          boost::is_same< Right, node >
        >
      >::type
    >
    node operator,( const Left &left, const Right &right ) {
      return node(
        detail :: binary_operator < node > (
          detail :: binary_operator< node > :: tag :: comma, std::array< node, 2u >{{ node::make_node( left ) , node::make_node( right ) }}
        )
      );
    }
    template<
      typename Left, typename Right,
      typename Enable = typename boost::enable_if<
        boost::mpl::or_<
          is_node< Left >, is_node< Right >,
          boost::is_same< Left, node >,
          boost::is_same< Right, node >
        >
      >::type
    >
    node operator,( Left &&left, Right &&right ) {
      return node(
        detail :: binary_operator< node >(
          detail :: binary_operator< node > :: tag :: comma, std::array< node, 2u >{{ node::make_node( std::move( left ) ) , node::make_node( std::move( right ) ) }}
        )
      );
    }
  }
}

#endif


