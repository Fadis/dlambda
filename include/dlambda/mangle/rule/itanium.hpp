#ifndef DLAMBDA_MANGLE_RULE_ITANIUM_HPP
#define DLAMBDA_MANGLE_RULE_ITANIUM_HPP

#include <boost/strong_typedef.hpp>

namespace dlambda {
  namespace mangle {
    BOOST_STRONG_TYPEDEF( std::string, source_name_type );
    BOOST_STRONG_TYPEDEF( size_t, unnamed_type_order_type );
    BOOST_STRONG_TYPEDEF( std::string, user_defined_literal_type );
    struct user_defined_literal {
      std::string name;
    }
    struct cast {
      type::type target;
    }
    struct vender_extended_operator {
      uint32_t id;
      std::string name;
    }
    enum operator_name_t {
      NEW,
      NEW_ARRAY,
      DELETE,
      DELETE_ARRAY,
      UNARY_PLUS,
      UNARY_MINUS,
      ADDRESS_OF,
      DEREFERENCE,
      COMPLEMENT,
      PLUS,
      NEGATE,
      MULTIPLIES,
      DIVIDES,
      MODULUS,
      BITWISE_AND,
      BITWISE_OR,
      BITWISE_XOR,
      ASSIGN,
      PLUS_ASSIGN,
      MINUS_ASSIGN,
      MULTIPLIES_ASSIGN,
      DIVIDES_ASSIGN,
      MODULUS_ASSIGN,
      BITWISE_AND_ASSIGN,
      BITWISE_OR_ASSIGN,
      BITWISE_XOR_ASSIGN,
      SHIFT_LEFT,
      SHIFT_RIGHT,
      SHIFT_LEFT_ASSIGN,
      SHIFT_RIGHT_ASSIGN,
      EQUAL_TO,
      NOT_EQUAL_TO,
      LESS_THAN,
      GREATER_THAN,
      LESS_EQUAL,
      GREATER_EQUAL,
      LOGICAL_NOT,
      LOGICAL_AND,
      LOGICAL_OR,
      POST_INC,
      POST_DEC,
      COMMA,
      MEM_PTR,
      MEMBER,
      FUNCTION,
      SUBSCRIPT,
      IF_ELSE
    };
    BOOST_STRONG_TYPEDEF( operator_name_t, operator_name_type )
    BOOST_STRONG_TYPEDEF( boost::variant< operator_name_type > );

    namespace parser {
      template< typename Iterator, typename RuleType >
      class itanium : public boost::spirit::qi::grammar< Iterator, RuleType > {
      public:
        itanium() : itanium::base_type( root ) {
          root = "_Z" >> encoding;
          encoding = function|data|special_name;
          function = name >> bare_function_type;
          data = name.alias();
          name = nested_name|unscoped_name|template_|local_name;
          template_ = unscoped_template_name >> template_args;
          unscoped_name = unqualified_name|standard_unqualified_name;
          standard_unqualified_name = "St" >> unqualified_name;
          unscoped_template_name = unscoped_name|substitution;
          nested_name = "N" >> -cv_qualifiers >> -ref_qualifiers >> prefix >> unqualified_name >> "E"
        }
      };
    }
  }
}

#endif

