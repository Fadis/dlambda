#include <array>
#include <vector>
#include <memory>
#include <boost/array.hpp>
#include <boost/variant/recursive_variant.hpp>
#include <boost/variant/static_visitor.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
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
#include <dlambda/type.hpp>

namespace dlambda {
  namespace detail {
    struct bad_cast {};
    template< typename T >
    class is_arithmetic_type :
      public boost::mpl::or_<
        boost::is_same< type::enum_, T >,
        boost::is_same< type::int_, T >,
        boost::is_same< type::float_, T >,
        boost::is_same< type::bool_, T >,
        boost::is_same< type::pointer< type::type >, T >
      > {}
    class usual_arithmetic_conversions : public boost::static_visitor< type::type > {
    public:
      type::type operator()( const type::enum_ &left, const type::enum_ &right ) {
        if( left.name != right.name )
          throw bad_cast();
        else
          return left;
      }
      template< typename Type >
      type::type operator()(
        const type::enum_ &left,
        const Type &right,
        typename boost::enable_if<
          boost::mpl::and_<
            is_arithmetic_type< Type >,
            boost::mpl::not_< boost::is_same< Type, type::enum_ > >,
            is_arithmetic_type< Type >
          >
        >::type* = 0
      ) {
        throw bad_cast();
      }
      template< typename Type >
      type::type operator()(
        const Type &left,
        const type::enum_ &right,
        typename boost::enable_if<
          boost::mpl::and_<
            is_arithmetic_type< Type >,
            boost::mpl::not_< boost::is_same< Type, type::enum_ > >
          >
        >::type* = 0
      ) {
        throw bad_cast();
      }
      type::type operator()(
        const type::float_ &left,
        const type::float_ &right
      ) {
        return ( left.size > right.size ) ? type::float_( false, left. : right;
      }
      template< typename Type >
      type::type operator()(
        const type::float_ &left,
        const Type &right,
        typename boost::enable_if<
          boost::mpl::and_<
            is_arithmetic_type< Type >,
            boost::mpl::not_< boost::is_same< Type, type::enum_ > >,
            boost::mpl::not_< boost::is_same< Type, type::float_ > >
          >
        >::type* = 0
      ) {
        return left;
      }
      template< typename Type >
      type::type operator()(
        const Type &left,
        const type::float_ &right,
        typename boost::enable_if<
          boost::mpl::and_<
            is_arithmetic_type< Type >,
            boost::mpl::not_< boost::is_same< Type, type::enum_ > >,
            boost::mpl::not_< boost::is_same< Type, type::float_ > >
          >
        >::type* = 0
      ) {
        return right;
      }
      template< typename Type >
      type::type operator()(
        const type::int_ &left,
        const type::int_ &right,
      ) {
        if( left.length > right.length )
          return left;
        else if( left.length < right.length )
          return right;
        else {
          if( left.signed_ )
            return left;
          else if( right.signed_ )
            return right;
          else
            return left;
        }
      }
      template< typename Type >
      type::type operator()(
        const type::int_ &left,
        const type::bool_ &right,
      ) {
        static const type::int_ converted(  )
        if( left.length > right.length )
          return left;
        else if( left.length < right.length )
          return right;
        else {
          if( left.signed_ )
            return left;
          else if( right.signed_ )
            return right;
          else
            return left;
        }
      }
      type::type operator()( ... ) {
        throw bad_cast();
      }
    };
  }
  std::shared_ptr< llvm::Type > get_llvm_type(
    const std::shared_ptr< llvm::LLVMContext > &context,
    const type::type &type_
  ) {
    detail::get_llvm_type visitor( context );
    return apply_visitor( visitor, type_ );
  }
}

