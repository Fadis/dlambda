struct a { static struct {} x; };
typedef decltype( a::x ) b;
typedef struct {} c;

void f( b ) {} // <- _Z1fN1aUt_E
void g( c ) {} // <- _Z1g1c 
