#ifndef DLAMBDA_TYPE_HPP
#define DLAMBDA_TYPE_HPP

#include <array>
#include <vector>
#include <memory>
#include <boost/array.hpp>
#include <boost/optional.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/is_signed.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/mpl/is_sequence.hpp>
#include <boost/fusion/include/mpl.hpp>
#include <boost/mpl/pop_back.hpp>
#include <boost/mpl/back.hpp>

namespace dlambda {
  namespace types {
    struct any_type {
      bool operator==( const any_type& ) const {
        return true;
      }
      bool operator!=( const any_type& ) const {
        return false;
      }
    };
    struct void_ {
      bool operator==( const void_& ) const {
        return true;
      }
      bool operator!=( const void_& ) const {
        return false;
      }
    };
    struct bool_ {
      bool_( const bool_& ) = default;
      bool_( bool_&& ) = default;
      bool_( bool const__, bool volatile__ ) : const_( const__ ), volatile_( volatile__ ) {}
      bool_ &operator=( const bool_& ) = default;
      bool_ &operator=( bool_&& ) = default;
      bool const_;
      bool volatile_;
      constexpr static size_t length = 1u;
      bool operator==( const bool_ &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_;
      }
      bool operator!=( const bool_ &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_;
      }
    };
    struct char_ {
      char_( const char_& ) = default;
      char_( char_&& ) = default;
      char_( bool const__, bool volatile__, const boost::optional< bool > &signed__ ) : const_( const__ ), volatile_( volatile__ ), signed_( signed__ ) {}
      char_( bool const__, bool volatile__ ) : const_( const__ ), volatile_( volatile__ ), signed_( boost::none ) {}
      char_ &operator=( const char_& ) = default;
      char_ &operator=( char_&& ) = default;
      bool const_;
      bool volatile_;
      boost::optional< bool > signed_;
      constexpr static size_t length = sizeof( char ) * 8u;
      bool operator==( const char_ &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && signed_ == right.signed_;
      }
      bool operator!=( const char_ &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || signed_ != right.signed_;
      }
    };
    struct wchar_t_ {
      wchar_t_( const wchar_t_& ) = default;
      wchar_t_( wchar_t_&& ) = default;
      wchar_t_( bool const__, bool volatile__, bool signed__ ) : const_( const__ ), volatile_( volatile__ ), signed_( signed__ ) {}
      wchar_t_( bool const__, bool volatile__ ) : const_( const__ ), volatile_( volatile__ ), signed_( boost::none ) {}
      wchar_t_ &operator=( const wchar_t_& ) = default;
      wchar_t_ &operator=( wchar_t_&& ) = default;
      bool const_;
      bool volatile_;
      boost::optional< bool > signed_;
      constexpr static size_t length = sizeof( wchar_t ) * 8u;
      bool operator==( const wchar_t_ &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && signed_ == right.signed_;
      }
      bool operator!=( const wchar_t_ &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || signed_ != right.signed_;
      }
    };
    struct char16_t_ {
      char16_t_( const char16_t_& ) = default;
      char16_t_( char16_t_&& ) = default;
      char16_t_( bool const__, bool volatile__, bool signed__ ) : const_( const__ ), volatile_( volatile__ ), signed_( signed__ ) {}
      char16_t_( bool const__, bool volatile__ ) : const_( const__ ), volatile_( volatile__ ), signed_( boost::none ) {}
      char16_t_ &operator=( const char16_t_& ) = default;
      char16_t_ &operator=( char16_t_&& ) = default;
      bool const_;
      bool volatile_;
      boost::optional< bool > signed_;
      constexpr static size_t length = sizeof( char16_t ) * 8u;
      bool operator==( const char16_t_ &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && signed_ == right.signed_;
      }
      bool operator!=( const char16_t_ &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || signed_ != right.signed_;
      }
    };
    struct char32_t_ {
      char32_t_( const char32_t_& ) = default;
      char32_t_( char32_t_&& ) = default;
      char32_t_( bool const__, bool volatile__, bool signed__ ) : const_( const__ ), volatile_( volatile__ ), signed_( signed__ ) {}
      char32_t_( bool const__, bool volatile__ ) : const_( const__ ), volatile_( volatile__ ), signed_( boost::none ) {}
      char32_t_ &operator=( const char32_t_& ) = default;
      char32_t_ &operator=( char32_t_&& ) = default;
      bool const_;
      bool volatile_;
      boost::optional< bool > signed_;
      constexpr static size_t length = sizeof( char32_t ) * 8u;
      bool operator==( const char32_t_ &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && signed_ == right.signed_;
      }
      bool operator!=( const char32_t_ &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || signed_ != right.signed_;
      }
    };
    struct int_ {
      int_( const int_& ) = default;
      int_( int_&& ) = default;
      int_( bool const__, bool volatile__, size_t length_, bool signed__ ) : const_( const__ ), volatile_( volatile__ ), length( length_ ), signed_( signed__ ) {}
      int_ &operator=( const int_& ) = default;
      int_ &operator=( int_&& ) = default;
      bool const_;
      bool volatile_;
      size_t length;
      bool signed_;
      bool operator==( const int_ &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && length == right.length && signed_ == right.signed_;
      }
      bool operator!=( const int_ &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || length != right.length || signed_ != right.signed_;
      }
    };
    struct enum_ {
      enum_( const enum_& ) = default;
      enum_( enum_&& ) = default;
      enum_( bool const__, bool volatile__, size_t length_, bool signed__, const std::string &name_ ) : const_( const__ ), volatile_( volatile__ ), length( length_ ), signed_( signed__ ), name( name_ ) {}
      enum_ &operator=( const enum_& ) = default;
      enum_ &operator=( enum_&& ) = default;
      bool const_;
      bool volatile_;
      size_t length;
      bool signed_;
      std::string name;
      bool operator==( const enum_ &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && length == right.length && signed_ == right.signed_ && name == right.name;
      }
      bool operator!=( const enum_ &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || length != right.length || signed_ != right.signed_ || name != right.name;
      }
    };
    struct float_ {
      float_( const float_& ) = default;
      float_( float_&& ) = default;
      float_( bool const__, bool volatile__, size_t length_, bool ordered_ ) : const_( const__ ), volatile_( volatile__ ), length( length_ ), ordered( ordered_ ) {}
      float_ &operator=( const float_& ) = default;
      float_ &operator=( float_&& ) = default;
      bool const_;
      bool volatile_;
      size_t length;
      bool ordered;
      bool operator==( const float_ &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && length == right.length;
      }
      bool operator!=( const float_ &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || length != right.length;
      }
    };
    template< typename T >
    struct pointer {
      pointer( const pointer& ) = default;
      pointer( pointer&& ) = default;
      pointer( bool const__, bool volatile__, bool restrict__, const T &base_ ) : const_( const__ ), volatile_( volatile__ ), restrict_( restrict__ ), base( base_ ) {}
      pointer &operator=( const pointer& ) = default;
      pointer &operator=( pointer&& ) = default;
      bool const_;
      bool volatile_;
      bool restrict_;
      T base;
      bool operator==( const pointer< T > &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && restrict_ == right.restrict_ && base == right.base;
      }
      bool operator!=( const pointer< T > &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || restrict_ != right.restrict_ || base != right.base;
      }
    };
    template< typename T >
    struct complex {
      complex( const complex& ) = default;
      complex( complex&& ) = default;
      complex( bool const__, bool volatile__, const T &base_ ) : const_( const__ ), volatile_( volatile__ ), base( base_ ) {}
      complex &operator=( const complex& ) = default;
      complex &operator=( complex&& ) = default;
      bool const_;
      bool volatile_;
      T base;
      bool operator==( const complex< T > &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && base == right.base;
      }
      bool operator!=( const complex< T > &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || base != right.base;
      }
    };
    template< typename T >
    struct lref {
      lref( const lref& ) = default;
      lref( lref&& ) = default;
      lref( const T &base_ ) : base( base_ ) {}
      lref &operator=( const lref& ) = default;
      lref &operator=( lref&& ) = default;
      T base;
      bool operator==( const lref< T > &right ) const {
        return base == right.base;
      }
      bool operator!=( const lref< T > &right ) const {
        return base != right.base;
      }
    };
    template< typename T >
    struct rref {
      rref( const rref& ) = default;
      rref( rref&& ) = default;
      rref( const T &base_ ) : base( base_ ) {}
      rref &operator=( const rref& ) = default;
      rref &operator=( rref&& ) = default;
      T base;
      bool operator==( const rref< T > &right ) const {
        return base == right.base;
      }
      bool operator!=( const lref< T > &right ) const {
        return base != right.base;
      }
    };
    template< typename T >
    struct array {
      array( const array& ) = default;
      array( array&& ) = default;
      array( bool const__, bool volatile__, size_t length_, const T &base_ ) : const_( const__ ), volatile_( volatile__ ), length( length_ ), base( base_ ) {}
      array &operator=( const array& ) = default;
      array &operator=( array&& ) = default;
      bool const_;
      bool volatile_;
      size_t length;
      T base;
      bool operator==( const array< T > &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && base == right.base;
      }
      bool operator!=( const array< T > &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || base != right.base;
      }
    };
    template< typename T >
    struct tuple {
      tuple( const tuple& ) = default;
      tuple( tuple&& ) = default;
      template< typename Range >
      tuple( bool const__, bool volatile__, const Range &base_ ) : const_( const__ ), volatile_( volatile__ ), base( boost::begin( base_ ), boost::end( base_ ) ), packed( false ) {}
      template< typename Range >
      tuple( bool const__, const Range &base_, const std::string &name_ ) : const_( const__ ), base( boost::begin( base_ ), boost::end( base_ ) ), name( name_ ), packed( false ) {}
      template< typename Range >
      tuple( bool const__, const Range &base_, bool packed_ ) : const_( const__ ), base( boost::begin( base_ ), boost::end( base_ ) ), packed( packed_ ) {}
      template< typename Range >
      tuple( bool const__, const Range &base_, const std::string &name_, bool packed_ ) : const_( const__ ), base( boost::begin( base_ ), boost::end( base_ ) ), name( name_ ), packed( packed_ ) {}
      tuple &operator=( const tuple& ) = default;
      tuple &operator=( tuple&& ) = default;
      bool const_;
      bool volatile_;
      std::vector< T > base;
      std::string name;
      bool packed;
      bool operator==( const tuple< T > &right ) const {
        return const_ == right.const_ && boost::equal( base, right.base ) && name == right.name && packed == right.packed;
      }
      bool operator!=( const tuple< T > &right ) const {
        return const_ != right.const_ || !boost::equal( base, right.base ) || name != right.name || packed != right.packed;
      }
    };
    template< typename T >
    struct vector {
      vector( const vector& ) = default;
      vector( vector&& ) = default;
      vector( bool const__, bool volatile__, size_t length_, const T &base_ ) : const_( const__ ), volatile_( volatile__ ), length( length_ ), base( base_ ) {}
      vector &operator=( const vector& ) = default;
      vector &operator=( vector&& ) = default;
      bool const_;
      bool volatile_;
      T base;
      size_t length;
      bool operator==( const vector< T > &right ) const {
        return const_ == right.const_ && base == right.base && length == right.length;
      }
    };
    struct user_defined_type {
      user_defined_type(
        const std::string &name_,
        bool const__,
        bool volatile__,
        bool copy_constructible_,
        bool empty_,
        bool nothrow_assign_,
        bool nothrow_copy_,
        bool nothrow_move_assignable_,
        bool nothrow_move_constructible_,
        bool pod_,
        bool polymorphic_,
        bool stateless_,
        bool class__,
        bool union__,
        bool trivial_assign_,
        bool trivial_constructor_,
        bool trivial_copy_,
        bool trivial_destructor_
      ) :
        name( name_ ),
        const_( const__ ),
        volatile_( volatile__ ),
        copy_constructible( copy_constructible_ ),
        empty( empty_ ),
        nothrow_assign( nothrow_assign_ ),
        nothrow_copy( nothrow_copy_ ),
        nothrow_move_assignable( nothrow_move_assignable_ ),
        nothrow_move_constructible( nothrow_move_constructible_ ),
        pod( pod_ ),
        polymorphic( polymorphic_ ),
        stateless( stateless_ ),
        class_( class__ ),
        union_( union__ ),
        trivial_assign( trivial_assign_ ),
        trivial_constructor( trivial_constructor_ ),
        trivial_copy( trivial_copy_ ),
        trivial_destructor( trivial_destructor_ )
      {}
      std::string name;
      bool const_;
      bool volatile_;
      bool copy_constructible;
      bool empty;
      bool nothrow_assign;
      bool nothrow_copy;
      bool nothrow_move_assignable;
      bool nothrow_move_constructible;
      bool pod;
      bool polymorphic;
      bool stateless;
      bool class_;
      bool union_;
      bool trivial_assign;
      bool trivial_constructor;
      bool trivial_copy;
      bool trivial_destructor;
      bool operator==( const user_defined_type &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && name == right.name;
      }
      bool operator!=( const user_defined_type &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || name != right.name;
      }
    };
    template< typename T >
    struct function {
      function( const function& ) = default;
      function( function&& ) = default;
      template< typename Range >
      function( bool const__, bool volatile__, bool variable_, const T &result, const Range &args ) : const_( const__ ), volatile_( volatile__ ), variable( variable_ ), result_type( result ), args_type( boost::begin( args ), boost::end( args ) ) {}
      function &operator=( const function& ) = default;
      function &operator=( function&& ) = default;
      bool const_;
      bool volatile_;
      bool variable;
      T result_type;
      std::vector< T > args_type;
      bool operator==( const function< T > &right ) const {
        return const_ == right.const_ && volatile_ == right.volatile_ && variable == right.variable && result_type == right.result_type && boost::equal( args_type, right.args_type );
      }
      bool operator!=( const function< T > &right ) const {
        return const_ != right.const_ || volatile_ != right.volatile_ || variable != right.variable || result_type != right.result_type || !boost::equal( args_type, right.args_type );
      }
    };
    template< typename T >
    struct member_pointer {
      member_pointer( const member_pointer& ) = default;
      member_pointer( member_pointer&& ) = default;
      template< typename Range >
      member_pointer( const T &this_type_, const T &value_type_ ) : this_type( this_type_ ), value_type( value_type_ ) {}
      member_pointer &operator=( const member_pointer& ) = default;
      member_pointer &operator=( member_pointer&& ) = default;
      T this_type;
      T value_type;
      bool operator==( const member_pointer< T > &right ) const {
        return this_type == right.this_type && value_type == right.value_type;
      }
    };
  }
  typedef boost::make_recursive_variant<
    types::any_type,
    types::void_,
    types::bool_,
    types::char_,
    types::wchar_t_,
    types::char16_t_,
    types::char32_t_,
    types::int_,
    types::enum_,
    types::float_,
    types::pointer< boost::recursive_variant_ >,
    types::complex< boost::recursive_variant_ >,
    types::lref< boost::recursive_variant_ >,
    types::rref< boost::recursive_variant_ >,
    types::array< boost::recursive_variant_ >,
    types::tuple< boost::recursive_variant_ >,
    types::vector< boost::recursive_variant_ >,
    types::user_defined_type,
    types::function< boost::recursive_variant_ >,
    types::member_pointer< boost::recursive_variant_ >
  >::type type;
  typedef types::any_type any_type;
}

#endif


