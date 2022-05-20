/*
Project 4 - Part 6 / 9
Video: Chapter 5 Part 3
 
Create a branch named Part6
 
 Lambdas
 
    Do not delete your previous main. you will be adding to it.

    Build/Run often with this task to make sure you're not breaking the code with each step.
    I recommend committing after you get each step working so you can revert to a working version easily if needed.
 
 1) add two member functions named "apply()" to each of your Heap-Allocated Numeric Type wrappers.
         both apply() functions should work with chaining
 
 2) One of the apply() functions should takes a std::function<> object as the function argument.
    the std::function<> object should return *this;
 
 3) the other apply() function should take a function pointer.
    the function pointer should return void.
 
 4) Make both of the member functions's Callable Function Parameter use your owned object as it's single parameter.
         e.g. if you manage your owned object via std::unique_ptr<T>, you'd use this for your std::function argument:
             std::function< OwnedT&(std::unique_ptr<T>&)>
             
         if you managed your owned object via a raw pointer, you'd use this for your std::function argument:
             std::function<OwnedT&(T&)>
 
 5) call that Callable Function Parameter in the apply() member function.
         be sure to practice safe std::function usage (make sure it's not a nullptr function being called)
 
 6) copy the part6() function below and paste it after part4()
 7) call part6() after part4() is called at the end of main().

 8) fill in the missing arguments in part6 such that you produce the expected output.

 9) Make your lambda & free function update the value of your held object
 
 
build/run to make sure you don't have any errors
 
 If you need to see an example, look at https://bitbucket.org/MatkatMusic/pfmcpptasks/src/master/Projects/Project4/Part6Example.cpp
 */


/*
your program should generate the following output EXACTLY.
This includes the warnings.
The output should have zero warnings.



FloatType add result=4
FloatType subtract result=2
FloatType multiply result=4
FloatType divide result=0.25

DoubleType add result=4
DoubleType subtract result=2
DoubleType multiply result=4
DoubleType divide result=0.8

IntType add result=4
IntType subtract result=2
IntType multiply result=4
IntType divide result=1

Chain calculation = 590
New value of ft = (ft + 3.0f) * 1.5f / 5.0f = 0.975
---------------------

Initial value of dt: 0.8
Initial value of it: 590
Use of function concatenation (mixed type arguments)
New value of dt = (dt * it) / 5.0f + ft = 95.375
---------------------

Intercept division by 0
New value of it = it / 0 = error: integer division by zero is an error and will crash the program!
590
New value of ft = ft / 0 = warning: floating point division by zero!
inf
New value of dt = dt / 0 = warning: floating point division by zero!
inf
---------------------

The result of FloatType^4 divided by IntType is: 26.9136
The result of DoubleType times 3 plus IntType is : 67.3
The result of IntType divided by 3.14 multiplied by DoubleType minus FloatType is: 711
An operation followed by attempts to divide by 0, which are ignored and warns user:
error: integer division by zero is an error and will crash the program!
error: integer division by zero is an error and will crash the program!
error: integer division by zero is an error and will crash the program!
505521
FloatType x IntType  =  13143546
(IntType + DoubleType + FloatType) x 24 = 315447336
Power tests with FloatType
pow(ft1, floatExp) = 2^2 = 4
pow(ft1, itExp) = 4^2 = 16
pow(ft1, ftExp) = 16^2 = 256
pow(ft1, dtExp) = 256^2 = 65536
---------------------

Power tests with DoubleType
pow(dt1, doubleExp) = 2^2 = 4
pow(dt1, itExp) = 4^2 = 16
pow(dt1, ftExp) = 16^2 = 256
pow(dt1, dtExp) = 256^2 = 65536
---------------------

Power tests with IntType
pow(it1, intExp) = 2^2 = 4
pow(it1, itExp) = 4^2 = 16
pow(it1, ftExp) = 16^2 = 256
pow(it1, dtExp) = 256^2 = 65536
===============================

Point tests with float argument:
Point { x: 3, y: 6 }
Multiplication factor: 6
Point { x: 18, y: 36 }
---------------------

Point tests with FloatType argument:
Point { x: 3, y: 3 }
Multiplication factor: 3
Point { x: 9, y: 9 }
---------------------

Point tests with DoubleType argument:
Point { x: 3, y: 4 }
Multiplication factor: 4
Point { x: 12, y: 16 }
---------------------

Point tests with IntType argument:
Point { x: 3, y: 4 }
Multiplication factor: 5
Point { x: 15, y: 20 }
---------------------

Calling FloatType::apply() using a lambda (adds 7.0f) and FloatType as return type:
ft3 before: 3
ft3 after: 10
Calling FloatType::apply() using a free function (adds 7.0f) and void as return type:
ft3 before: 10
ft3 after: 17
---------------------

Calling DoubleType::apply() using a lambda (adds 6.0) and DoubleType as return type:
dt3 before: 4
dt3 after: 10
Calling DoubleType::apply() using a free function (adds 6.0) and void as return type:
dt3 before: 10
dt3 after: 16
---------------------

Calling IntType::apply() using a lambda (adds 5) and IntType as return type:
it3 before: 5
it3 after: 10
Calling IntType::apply() using a free function (adds 5) and void as return type:
it3 before: 10
it3 after: 15
---------------------

good to go!

Use a service like https://www.diffchecker.com/diff to compare your output.
*/


struct A {};
struct HeapA
{
    HeapA() : a(new A) {}
    ~HeapA()
    {
        delete a;
    }
    A* a = nullptr;
};

/*
 MAKE SURE YOU ARE NOT ON THE MASTER BRANCH

 Commit your changes by clicking on the Source Control panel on the left, entering a message, and click [Commit and push].
 
 If you didn't already:
    Make a pull request after you make your first commit
    pin the pull request link and this repl.it link to our DM thread in a single message.

 send me a DM to review your pull request when the project is ready for review.

 Wait for my code review.
 */

#include <iostream>
#include <cmath>
#include <functional>

struct FloatType;
struct DoubleType;
struct IntType;

// Float Type

struct FloatType
{
    explicit FloatType(float floatOnHeap) : value( new float(floatOnHeap) ) { }
    ~FloatType()
    {
        delete value;
    }
    
    operator float() const { return *value; }
    
    FloatType& operator+=( float fl );
    FloatType& operator-=( float fl );
    FloatType& operator*=( float fl );
    FloatType& operator/=( float fl );
    
    FloatType& pow( float fl );
    FloatType& pow( const FloatType& fl );
    FloatType& pow( const DoubleType& db );
    FloatType& pow( const IntType& it );
    
    FloatType& apply( std::function<FloatType&(float&)> );            // takes std::function as a parameter
    FloatType& apply( void(*funcPtr)(float&) );                       // functin pointer
private:
    float* value = nullptr;
    FloatType& powInternal( const float fl );
};


// Double Type

struct DoubleType
{
    explicit DoubleType(double doubleOnHeap) : value( new double(doubleOnHeap) ) { }
    ~DoubleType()
    {
        delete value;
    }
    
    operator double() const { return *value; }
    
    DoubleType& operator+=( double db );
    DoubleType& operator-=( double db );
    DoubleType& operator*=( double db );
    DoubleType& operator/=( double db );
    
    DoubleType& pow( double db );
    DoubleType& pow( const DoubleType& );
    DoubleType& pow( const FloatType& );
    DoubleType& pow( const IntType& );

    DoubleType& apply( std::function<DoubleType&(double&)> );
    DoubleType& apply( void(*funcPtr)(double&) );   
private:
    double* value = nullptr;
    DoubleType& powInternal( const double db );
};

// Int Type
struct IntType
{
    explicit IntType(int intOnHeap) : value( new int(intOnHeap) ) { }
    ~IntType()
    {
        delete value;
    }
    
    operator int() const { return *value; }
    
    IntType& operator+=( int it );
    IntType& operator-=( int it );
    IntType& operator*=( int it );
    IntType& operator/=( int it );
    
    IntType& pow( int it );
    IntType& pow( const IntType& );
    IntType& pow( const FloatType& );
    IntType& pow( const DoubleType& );

    IntType& apply( std::function<IntType&(int&)> );
    IntType& apply( void(*funcPtr)(int&) );
private:
    int* value = nullptr;
    IntType& powInternal( const int it );
};

// Float

FloatType& FloatType::operator+=( float fl )
{
    *value += fl;
    return *this;
}

FloatType& FloatType::operator-=( float fl )
{
    *value -= fl;
    return *this;
}

FloatType& FloatType::operator*=( float fl )
{
    *value *= fl;
    return *this;
}

FloatType& FloatType::operator/=( float fl )
{
    if ( fl == 0.f )
    {
        std::cout << "warning: floating point division by zero!" << std::endl;
    }
    
    *value /= fl;
    return *this;
}

FloatType& FloatType::powInternal( const float fl )
{
    *value = std::pow( *value, fl );
    return *this;
}

FloatType& FloatType::pow( float fl )
{
    return powInternal( fl );
}

FloatType& FloatType::pow( const FloatType& fl )
{
    return powInternal( fl );
}

FloatType& FloatType::pow( const DoubleType& db )
{
    return powInternal( static_cast<float>(db) );
}

FloatType& FloatType::pow( const IntType& it )
{
    return powInternal( it );
}

FloatType& FloatType::apply( std::function<FloatType&(float&)> func )
{
    if ( func != nullptr )
    {
        return func(*value);
    }
    return *this;
}

FloatType& FloatType::apply( void(*funcPtr)(float&) )
{
    if ( funcPtr != nullptr )
    {
        funcPtr(*value);
    }
    return *this;
}

// Double

DoubleType& DoubleType::operator+=( double db )
{
    *value += db;
    return *this;
}

DoubleType& DoubleType::operator-=( double db )
{
    *value -= db;
    return *this;
}

DoubleType& DoubleType::operator*=( double db )
{
    *value *= db;
    return *this;
}

DoubleType& DoubleType::operator/=( double db )
{
    if ( db == 0.0 )
    {
        std::cout << "warning: floating point division by zero!" << std::endl;
    }
    
    *value /= db;
    return *this;
}

DoubleType& DoubleType::powInternal( const double db )
{
    *value = std::pow( *value, db );
    return *this;
}

DoubleType& DoubleType::pow( double db )
{
    return powInternal( db );
}

DoubleType& DoubleType::pow( const DoubleType& db)
{
    return powInternal( db );
}

DoubleType& DoubleType::pow( const FloatType& fl )
{
    return powInternal( static_cast<double>(fl) );
}

DoubleType& DoubleType::pow( const IntType& it )
{
    return powInternal( it );
}

DoubleType& DoubleType::apply( std::function<DoubleType&(double&)> func )
{
    if( func != nullptr )
    {
        return func( *value );
    }
    return *this;
}

DoubleType& DoubleType::apply( void(*funcPtr)(double&) )
{
    if( funcPtr != nullptr )
    {
        funcPtr( *value );
    }
    return *this;
}

// Int

IntType& IntType::operator+=( int it )
{
    *value += it;
    return *this;
}

IntType& IntType::operator-=( int it )
{
    *value -= it;
    return *this;
}

IntType& IntType::operator*=( int it )
{
    *value *= it;
    return *this;
}

IntType& IntType::operator/=( int it )
{
    if ( it == 0 )
    {
        std::cout << "error: integer division by zero is an error and will crash the program!" << std::endl;
        
        return *this;
    }
    
    *value /= it;
    return *this;
}

IntType& IntType::powInternal( const int it )
{
    *value = static_cast<int>( std::pow( *value, it ) );
    return *this;
}

IntType& IntType::pow( int it )
{
    return powInternal( it );
}

IntType& IntType::pow( const IntType& it )
{
    return powInternal( it );
}

IntType& IntType::pow( const FloatType& fl )
{
    return powInternal( static_cast<int>(fl) );
}

IntType& IntType::pow( const DoubleType& db )
{
    return powInternal( static_cast<int>(db) );
}

IntType& IntType::apply( std::function<IntType&(int&)> func )
{
    if( func != nullptr )
    {
        return func( *value );
    }
    return *this;
}

IntType& IntType::apply( void(*funcPtr)(int&) )
{
    if( funcPtr != nullptr )
    {
        funcPtr( *value );
    }
    return *this;
}

// Point

struct Point
{
    Point( float fl1, float fl2 ) : x( fl1 ), y( fl2 ) {}
    Point( FloatType& fl1, FloatType& fl2 ) : Point( static_cast<float>(fl1), static_cast<float>(fl2) ) {}
    Point( DoubleType& fl1, DoubleType& fl2 ) : Point( static_cast<float>(fl1), static_cast<float>(fl2) ) {}
    Point( IntType& fl1, IntType& fl2 ) : Point( static_cast<float>(fl1), static_cast<float>(fl2) ) {}

    Point& multiply(float m)
    {
        x *= m;
        y *= m;
        return *this;
    }

    Point& multiply(FloatType& fl)
    {
        return multiply( static_cast<float>(fl ));
    }

    Point& multiply(DoubleType& db)
    {
        return multiply( static_cast<float>(db) );
    }
    
    Point& multiply(IntType& it)
    {
        return multiply( static_cast<float>(it) );
    }

    void toString()
    {
        std::cout << "Point { x: " << x << ", y: " << y << " }"<< std::endl;
    }
private:
    float x{0}, y{0};
};

void part3()
{
    FloatType ft( 5.5f );
    DoubleType dt( 11.1 );
    IntType it ( 34 );
    DoubleType pi( 3.14 );

    ft *= ft;
    ft *= ft;
    ft /= it;
    
    std::cout << "The result of FloatType^4 divided by IntType is: " << ft << std::endl;
    
    dt *= 3;
    dt += it;
    
    std::cout << "The result of DoubleType times 3 plus IntType is : " << dt << std::endl;
    
    it /= static_cast<int>(pi);
    it *= static_cast<int>(dt);
    it -= static_cast<int>(ft);
    
    std::cout << "The result of IntType divided by 3.14 multiplied by DoubleType minus FloatType is: " << it << std::endl;
    std::cout << "An operation followed by attempts to divide by 0, which are ignored and warns user: " << std::endl;
    
    it *= it;
    it /= 0;
    it /= 0.0f;
    it /= 0.0;
    std::cout << it << std::endl;
    
    it *= static_cast<int>(ft);
    std::cout << "FloatType x IntType  =  " << it << std::endl;
    
    it += static_cast<int>(dt);
    it += static_cast<int>(ft);
    it *= 24;
    
    std::cout << "(IntType + DoubleType + FloatType) x 24 = " << it << std::endl;
}
    
void part4()
{
    // ------------------------------------------------------------
    //                          Power tests
    // ------------------------------------------------------------
    FloatType ft1(2);
    DoubleType dt1(2);
    IntType it1(2);
    float floatExp = 2.0f;
    double doubleExp = 2.0;
    int intExp = 2;
    IntType itExp(2);
    FloatType ftExp(2.0f);
    DoubleType dtExp(2.0);
    
    // Power tests with FloatType
    std::cout << "Power tests with FloatType" << std::endl;
    std::cout << "pow(ft1, floatExp) = " << ft1 << "^" << floatExp << " = " << ft1.pow(floatExp)  << std::endl;
    std::cout << "pow(ft1, itExp) = " << ft1 << "^" << itExp << " = " << ft1.pow(itExp)  << std::endl;
    std::cout << "pow(ft1, ftExp) = " << ft1 << "^" << ftExp << " = " << ft1.pow(ftExp)  << std::endl;
    std::cout << "pow(ft1, dtExp) = " << ft1 << "^" << dtExp << " = " << ft1.pow(dtExp)  << std::endl;
    std::cout << "---------------------\n" << std::endl;

    // Power tests with DoubleType
    std::cout << "Power tests with DoubleType" << std::endl;
    std::cout << "pow(dt1, doubleExp) = " << dt1 << "^" << doubleExp << " = " << dt1.pow(intExp)  << std::endl;
    std::cout << "pow(dt1, itExp) = " << dt1 << "^" << itExp << " = " << dt1.pow(itExp)  << std::endl;
    std::cout << "pow(dt1, ftExp) = " << dt1 << "^" << ftExp << " = " << dt1.pow(ftExp)  << std::endl;
    std::cout << "pow(dt1, dtExp) = " << dt1 << "^" << dtExp << " = " << dt1.pow(dtExp)  << std::endl;
    std::cout << "---------------------\n" << std::endl;

    // Power tests with IntType
    std::cout << "Power tests with IntType" << std::endl;
    std::cout << "pow(it1, intExp) = " << it1 << "^" << intExp << " = " << it1.pow(intExp)  << std::endl;
    std::cout << "pow(it1, itExp) = " << it1 << "^" << itExp << " = " << it1.pow(itExp)  << std::endl;
    std::cout << "pow(it1, ftExp) = " << it1 << "^" << ftExp << " = " << it1.pow(ftExp)  << std::endl;
    std::cout << "pow(it1, dtExp) = " << it1 << "^" << dtExp << " = " << it1.pow(dtExp)  << std::endl;
    std::cout << "===============================\n" << std::endl;

    // ------------------------------------------------------------
    //                          Point tests
    // ------------------------------------------------------------
    FloatType ft2(3.0f);
    DoubleType dt2(4.0);
    IntType it2(5);
    float floatMul = 6.0f;

    // Point tests with float
    std::cout << "Point tests with float argument:" << std::endl;
    Point p0(ft2, floatMul);
    p0.toString();
    std::cout << "Multiplication factor: " << floatMul << std::endl;
    p0.multiply(floatMul);
    p0.toString();
    std::cout << "---------------------\n" << std::endl;

    // Point tests with FloatType
    std::cout << "Point tests with FloatType argument:" << std::endl;
    Point p1(ft2, ft2);
    p1.toString();
    std::cout << "Multiplication factor: " << ft2 << std::endl;
    p1.multiply(ft2);
    p1.toString();
    std::cout << "---------------------\n" << std::endl;

    // Point tests with DoubleType
    std::cout << "Point tests with DoubleType argument:" << std::endl;
    Point p2(ft2, static_cast<float>(dt2));
    p2.toString();
    std::cout << "Multiplication factor: " << dt2 << std::endl;
    p2.multiply(dt2);
    p2.toString();
    std::cout << "---------------------\n" << std::endl;

    // Point tests with IntType
    std::cout << "Point tests with IntType argument:" << std::endl;
    Point p3(ft2, static_cast<float>(dt2));
    p3.toString();
    std::cout << "Multiplication factor: " << it2 << std::endl;
    p3.multiply(it2);
    p3.toString();
    std::cout << "---------------------\n" << std::endl;
}

void part6()
{
    FloatType ft3(3.0f);
    DoubleType dt3(4.0);
    IntType it3(5);
    
    std::cout << "Calling FloatType::apply() using a lambda (adds 7.0f) and FloatType as return type:" << std::endl;
    std::cout << "ft3 before: " << ft3 << std::endl;
    ft3.apply( [](){} );
    std::cout << "ft3 after: " << ft3 << std::endl;
    std::cout << "Calling FloatType::apply() using a free function (adds 7.0f) and void as return type:" << std::endl;
    std::cout << "ft3 before: " << ft3 << std::endl;
    ft3.apply(myFloatFreeFunct);
    std::cout << "ft3 after: " << ft3 << std::endl;
    std::cout << "---------------------\n" << std::endl;

    std::cout << "Calling DoubleType::apply() using a lambda (adds 6.0) and DoubleType as return type:" << std::endl;
    std::cout << "dt3 before: " << dt3 << std::endl;
    dt3.apply( [](){} );
    std::cout << "dt3 after: " << dt3 << std::endl;
    std::cout << "Calling DoubleType::apply() using a free function (adds 6.0) and void as return type:" << std::endl;
    std::cout << "dt3 before: " << dt3 << std::endl;
    dt3.apply(myDoubleFreeFunct);
    std::cout << "dt3 after: " << dt3 << std::endl;
    std::cout << "---------------------\n" << std::endl;

    std::cout << "Calling IntType::apply() using a lambda (adds 5) and IntType as return type:" << std::endl;
    std::cout << "it3 before: " << it3 << std::endl;
    it3.apply( [](){} );
    std::cout << "it3 after: " << it3 << std::endl;
    std::cout << "Calling IntType::apply() using a free function (adds 5) and void as return type:" << std::endl;
    std::cout << "it3 before: " << it3 << std::endl;
    it3.apply(myIntFreeFunct);
    std::cout << "it3 after: " << it3 << std::endl;
    std::cout << "---------------------\n" << std::endl;
}

int main()
{
    //testing instruction 0
    HeapA heapA;

    //assign heap primitives
    FloatType ft ( 2.0f );
    DoubleType dt ( 2 );
    IntType it ( 2 ) ;
    
    ft += ft;
    std::cout << "FloatType add result=" << ft << std::endl;
    
    ft /= 2.0f;
    std::cout << "FloatType subtract result=" << ft << std::endl;
    
    ft *= 2.0f;
    std::cout << "FloatType multiply result=" << ft << std::endl;
    
    ft /= 16.0f;
    std::cout << "FloatType divide result=" << ft << std::endl << std::endl;

    dt += static_cast<double>(2.0f);
    std::cout << "DoubleType add result=" << dt << std::endl;
    
    dt -= static_cast<double>(2.0f);
    std::cout << "DoubleType subtract result=" << dt << std::endl;
    
    dt *= static_cast<double>(2.0f);
    std::cout << "DoubleType multiply result=" << dt << std::endl;
    
    dt /= static_cast<double>(5.f);
    std::cout << "DoubleType divide result=" << dt << std::endl << std::endl;

    it += 2;
    std::cout << "IntType add result=" << it << std::endl;
    
    it -= 2;
    std::cout << "IntType subtract result=" << it << std::endl;
    
    it *= 2;
    std::cout << "IntType multiply result=" << it << std::endl;
    
    it /= 3;
    std::cout << "IntType divide result=" << it << std::endl << std::endl;
    
    it *= 1000;
    it /= 2;
    it -= 10;
    it += 100;
    
    std::cout << "Chain calculation = " << it << std::endl;

    // FloatType object instanciation and method tests
    // --------
    ft += 3.0f;
    ft *= 1.5f;
    ft /= 5.0f;
    
    std::cout << "New value of ft = (ft + 3.0f) * 1.5f / 5.0f = " << ft << std::endl;
       
    std::cout << "---------------------\n" << std::endl;
    
    // DoubleType/IntType object instanciation and method tests
    // --------
    
    std::cout << "Initial value of dt: " << dt << std::endl;
    std::cout << "Initial value of it: " << it << std::endl;
    // --------
    dt *= it;
    dt /= static_cast<double>(5.0f);
    dt += static_cast<double>(ft);
    
    std::cout << "Use of function concatenation (mixed type arguments)" << std::endl;
    std::cout << "New value of dt = (dt * it) / 5.0f + ft = " << dt << std::endl;

    std::cout << "---------------------\n" << std::endl;
    
    // Intercept division by 0
    // --------
    std::cout << "Intercept division by 0" << std::endl;
    
    std::cout << "New value of it = it / 0 = " << (it /= 0) << std::endl;
    
    std::cout << "New value of ft = ft / 0 = " << (ft /= 0) << std::endl;
    
    std::cout << "New value of dt = dt / 0 = " << (dt /= 0) << std::endl;

    std::cout << "---------------------\n" << std::endl;

    part3();
    
    part4();
    
    part6();
    
    std::cout << "good to go!\n";

    return 0;
}
