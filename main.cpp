/*
Project 4: Part 9 / 9
 video: Chapter 5 Part 8

Create a branch named Part9

 Rule of 3-5-0 and S.R.P.
 
 DO NOT EDIT YOUR PREVIOUS main().
 
 1) add the Leak Detector files from Project5
 
 2) move these macros after the JUCE_LEAK_DETECTOR macro :
 */

/*
 3) add JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Temporary) to the end of the  Temporary<> struct
 
 4) add JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Numeric) to the end of the Numeric<> struct
 
 if you compile it, you'll see lots of errors about deleted functions.
 
 5) Implement the Rule of 5 on Numeric<> and Temporary<> to fix this.
 
 You should end up with the same program output as Part 8's task if you did it right.
 */


/*
 If you did everything correctly, this is the output you should get:
 
I'm a Temporary<f> object, #0
I'm a Temporary<i> object, #0
I'm a Temporary<d> object, #0
f: -1.89
d: -3.024
i: -9
Point { x: -1.89, y: -9 }
d: 3.024
I'm a Temporary<d> object, #1
I'm a Temporary<d> object, #2
d: 1.49519e+08
Point { x: -2.82591e+08, y: -1.34567e+09 }
I'm a Temporary<f> object, #1
I'm a Temporary<i> object, #1
I'm a Temporary<i> object, #2
I'm a Temporary<i> object, #3
intNum: 5
I'm a Temporary<f> object, #2
f squared: 3.5721
I'm a Temporary<f> object, #3
f cubed: 45.5796
I'm a Temporary<d> object, #3
d squared: 2.2356e+16
I'm a Temporary<d> object, #4
d cubed: 1.11733e+49
I'm a Temporary<i> object, #4
i squared: 81
I'm a Temporary<i> object, #5
i cubed: 531441

Use a service like https://www.diffchecker.com/diff to compare your output.
*/

#include <iostream>

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

#include "LeakedObjectDetector.h"
#include <typeinfo>
#include <memory>
#include <iostream>
#include <cmath>
#include <functional>
#include <utility>

// #1
template<typename NumericType>
struct Temporary
{
    Temporary (NumericType t) : v (t)
    {
        std::cout << "I'm a Temporary<" << typeid(v).name() << "> object, #"
                  << counter++ << std::endl;
    }

    Temporary (Temporary&& other) : v (std::move (other.v)) {}  // move constructor
    
    Temporary& operator= (Temporary&& other)  //move assignment
    {
        v = std::move (other.v);
        return *this;
    }
    
    ~Temporary() = default;        // destructor
    
    operator NumericType() const { return v; }    // read-only function
    operator NumericType&() { return v; }         // read/write function
private:
    static int counter;
    NumericType v;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Temporary) // #3
};

// #2
template <typename NumericType>
int Temporary <NumericType>::counter = 0;

// #3
template <typename NumericType>
struct Numeric
{
    using Type = Temporary<NumericType>;
    
    explicit Numeric (NumericType numOnHeap) : value (std::make_unique<Type> (numOnHeap)) {}
    ~Numeric() = default;
    
    Numeric (Numeric&& other) : value (std::make_unique<Numeric&> (std::move (other.value))) {}  // move constructor
    
    Numeric& operator= (Numeric&& other) // move assignment
    {
        *value = std::move (other.value);
        return *this;
    }
    
    template <typename OtherType>
    Numeric& operator= (const OtherType& num)
    {
        *value = static_cast<NumericType> (num);
        return *this;
    }
    
    template <typename OtherType>
    Numeric& operator+= (const OtherType& num)
    {
        *value += static_cast<NumericType> (num);
        return *this;
    }
    
    template <typename OtherType>
    Numeric& operator-= (const OtherType& num)
    {
        *value -= static_cast<NumericType> (num);
        return *this;
    }
    
    template <typename OtherType>
    Numeric& operator*= (const OtherType& num)
    {
        *value *= static_cast<NumericType> (num);
        return *this;
    }
    
    template <typename N>
    Numeric& operator/= (const N& num)
    {
        if constexpr ( std::is_same<Type, int>::value )
        {
            if constexpr ( std::is_same<N, int>::value )
            {
                if ( num == 0 )
                {
                    std::cout << "error: integer division by zero is an error and will crash the program!\n" << std::endl;
                    return *this;
                }
            }
            else if ( std::abs( num ) <= std::numeric_limits<N>::epsilon() )
            {
                std::cout << "can't divide integers by zero!" << std::endl;
                return *this;
            }
        }
        else if ( std::abs( num ) <= std::numeric_limits<N>::epsilon() )
        {
            std::cout << "warning: floating point division by zero!" << std::endl;
        }
            
        *value /= static_cast<NumericType> (num);
        return *this;
    }
    
    // #6
    operator NumericType() const { return *value; }
    //operator NumericType&() { return *value; }
    
    // #5
    template <typename P>
    Numeric& pow( const P& num )
    {
        *value = static_cast<NumericType> (std::pow (*value, static_cast<NumericType>(num)));
        return *this;
    }
    
    template <typename N>
    Numeric& apply( std::function<Numeric&(N&)> num )
    
    {
        if ( num != nullptr )
        {
            return num( *value );
        }
        return *this;
    }
    
    // #4
    template <typename Callable>
    Numeric& apply( Callable callable )
    {
        callable( value );
        return *this;
    }
private:
    std::unique_ptr<Type> value;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Numeric) // #3
};

// #5 cube function
template <typename Type>
void cube( std::unique_ptr<Type>& num )
{
    auto& newNum = *num;
    newNum = newNum * newNum * newNum;
}

// Point
struct Point
{
    Point( float fl1, float fl2 ) : x( fl1 ), y( fl2 ) {}
    
    Point& multiply( float m )
    {
        x *= m;
        y *= m;
        return *this;
    }
    
    template <typename P>
    Point& multiply( const P& p )
    {
        return multiply( static_cast<float>(p) );
    }

    void toString()
    {
        std::cout << "Point { x: " << x << ", y: " << y << " }"<< std::endl;
    }
private:
    float x{0}, y{0};
};

template <typename NumFunc>
void myNumericFreeFunct( NumFunc& num )
{
    num += static_cast<NumFunc>(7.0);
}

void myFloatFreeFunct( float& fl )
{
    fl += 7.0f;
}

void myDoubleFreeFunct( double& db )
{
    db += 6.0;
}

void myIntFreeFunct( int& it )
{
    it += 5;
}

//void part3()
//{
//    Numeric<float>  ft( 5.5f );
//    Numeric<double> dt( 11.1 );
//    Numeric<int>    it ( 34 );
//    Numeric<double> pi( 3.14 );
//
//    ft *= ft;
//    ft *= ft;
//    ft /= static_cast<float>(it);
//
//    std::cout << "The result of FloatType^4 divided by IntType is: " << ft << std::endl;
//
//    dt *= 3;
//    dt += it;
//
//    std::cout << "The result of DoubleType times 3 plus IntType is : " << dt << std::endl;
//
//    it /= static_cast<int>(pi);
//    it *= static_cast<int>(dt);
//    it -= static_cast<int>(ft);
//
//    std::cout << "The result of IntType divided by 3.14 multiplied by DoubleType minus FloatType is: " << it << std::endl;
//    std::cout << "An operation followed by attempts to divide by 0, which are ignored and warns user: " << std::endl;
//
//    it *= it;
//    it /= 0;
//    it /= 0.0f;
//    it /= 0.0;
//    std::cout << it << std::endl;
//
//    it *= static_cast<int>(ft);
//    std::cout << "FloatType x IntType  =  " << it << std::endl;
//
//    it += static_cast<int>(dt);
//    it += static_cast<int>(ft);
//    it *= 24;
//
//    std::cout << "(IntType + DoubleType + FloatType) x 24 = " << it << std::endl;
//}
//
//void part4()
//{
//    // ------------------------------------------------------------
//    //                          Power tests
//    // ------------------------------------------------------------
//    Numeric<float>  ft1(2);
//    Numeric<double> dt1(2);
//    Numeric<int>    it1(2);
//    Numeric<int>    itExp(2);
//    Numeric<float>  ftExp(2.0f);
//    Numeric<double> dtExp(2.0);
//    float  floatExp = 2.0f;
//    double doubleExp = 2.0;
//    int    intExp = 2;
//
//    // Power tests with FloatType
//    std::cout << "Power tests with FloatType" << std::endl;
//    std::cout << "pow(ft1, floatExp) = " << ft1 << "^" << floatExp << " = " << ft1.pow(floatExp)  << std::endl;
//    std::cout << "pow(ft1, itExp) = " << ft1 << "^" << itExp << " = " << ft1.pow(itExp)  << std::endl;
//    std::cout << "pow(ft1, ftExp) = " << ft1 << "^" << ftExp << " = " << ft1.pow(ftExp)  << std::endl;
//    std::cout << "pow(ft1, dtExp) = " << ft1 << "^" << dtExp << " = " << ft1.pow(static_cast<float>(dtExp))  << std::endl;
//    std::cout << "---------------------\n" << std::endl;
//
//    // Power tests with DoubleType
//    std::cout << "Power tests with DoubleType" << std::endl;
//    std::cout << "pow(dt1, doubleExp) = " << dt1 << "^" << doubleExp << " = " << dt1.pow(intExp)  << std::endl;
//    std::cout << "pow(dt1, itExp) = " << dt1 << "^" << itExp << " = " << dt1.pow(itExp)  << std::endl;
//    std::cout << "pow(dt1, ftExp) = " << dt1 << "^" << ftExp << " = " << dt1.pow(static_cast<double>(ftExp))  << std::endl;
//    std::cout << "pow(dt1, dtExp) = " << dt1 << "^" << dtExp << " = " << dt1.pow(dtExp)  << std::endl;
//    std::cout << "---------------------\n" << std::endl;
//
//    // Power tests with IntType
//    std::cout << "Power tests with IntType" << std::endl;
//    std::cout << "pow(it1, intExp) = " << it1 << "^" << intExp << " = " << it1.pow(intExp)  << std::endl;
//    std::cout << "pow(it1, itExp) = " << it1 << "^" << itExp << " = " << it1.pow(itExp)  << std::endl;
//    std::cout << "pow(it1, ftExp) = " << it1 << "^" << ftExp << " = " << it1.pow(static_cast<int>(ftExp))  << std::endl;
//    std::cout << "pow(it1, dtExp) = " << it1 << "^" << dtExp << " = " << it1.pow(static_cast<int>(dtExp))  << std::endl;
//    std::cout << "===============================\n" << std::endl;
//
//    // ------------------------------------------------------------
//    //                          Point tests
//    // ------------------------------------------------------------
//    Numeric<float>  ft2(3.0f);
//    Numeric<double> dt2(4.0);
//    Numeric<int>    it2(5);
//    float  floatMul = 6.0f;
//
//    // Point tests with float
//    std::cout << "Point tests with float argument:" << std::endl;
//    Point p0(ft2, floatMul);
//    p0.toString();
//    std::cout << "Multiplication factor: " << floatMul << std::endl;
//    p0.multiply(floatMul);
//    p0.toString();
//    std::cout << "---------------------\n" << std::endl;
//
//    // Point tests with FloatType
//    std::cout << "Point tests with FloatType argument:" << std::endl;
//    Point p1(ft2, ft2);
//    p1.toString();
//    std::cout << "Multiplication factor: " << ft2 << std::endl;
//    p1.multiply(ft2);
//    p1.toString();
//    std::cout << "---------------------\n" << std::endl;
//
//    // Point tests with DoubleType
//    std::cout << "Point tests with DoubleType argument:" << std::endl;
//    Point p2(ft2, static_cast<float>(dt2));
//    p2.toString();
//    std::cout << "Multiplication factor: " << dt2 << std::endl;
//    p2.multiply(static_cast<float>(dt2));
//    p2.toString();
//    std::cout << "---------------------\n" << std::endl;
//
//    // Point tests with IntType
//    std::cout << "Point tests with IntType argument:" << std::endl;
//    Point p3(ft2, static_cast<float>(dt2));
//    p3.toString();
//    std::cout << "Multiplication factor: " << it2 << std::endl;
//    p3.multiply(it2);
//    p3.toString();
//    std::cout << "---------------------\n" << std::endl;
//}

//void part6()
//{
//    FloatType ft3(3.0f);
//    DoubleType dt3(4.0);
//    IntType it3(5);
//
//    std::cout << "Calling FloatType::apply() using a lambda (adds 7.0f) and FloatType as return type:" << std::endl;
//    std::cout << "ft3 before: " << ft3 << std::endl;
//    ft3.apply( [&ft3](float& fl) -> FloatType&
//    {
//        fl += 7.0f;
//        return ft3;
//    } );
//    std::cout << "ft3 after: " << ft3 << std::endl;
//    std::cout << "Calling FloatType::apply() using a free function (adds 7.0f) and void as return type:" << std::endl;
//    std::cout << "ft3 before: " << ft3 << std::endl;
//    ft3.apply(myFloatFreeFunct);
//    std::cout << "ft3 after: " << ft3 << std::endl;
//    std::cout << "---------------------\n" << std::endl;
//
//    std::cout << "Calling DoubleType::apply() using a lambda (adds 6.0) and DoubleType as return type:" << std::endl;
//    std::cout << "dt3 before: " << dt3 << std::endl;
//    dt3.apply( [&dt3](double& db) -> DoubleType&
//    {
//        db += 6.0;
//        return dt3;
//    } );
//    std::cout << "dt3 after: " << dt3 << std::endl;
//    std::cout << "Calling DoubleType::apply() using a free function (adds 6.0) and void as return type:" << std::endl;
//    std::cout << "dt3 before: " << dt3 << std::endl;
//    dt3.apply(myDoubleFreeFunct);
//    std::cout << "dt3 after: " << dt3 << std::endl;
//    std::cout << "---------------------\n" << std::endl;
//
//    std::cout << "Calling IntType::apply() using a lambda (adds 5) and IntType as return type:" << std::endl;
//    std::cout << "it3 before: " << it3 << std::endl;
//    it3.apply( [&it3](int& it) -> IntType&
//    {
//        it += 5;
//        return it3;
//    } );
//    std::cout << "it3 after: " << it3 << std::endl;
//    std::cout << "Calling IntType::apply() using a free function (adds 5) and void as return type:" << std::endl;
//    std::cout << "it3 before: " << it3 << std::endl;
//    it3.apply(myIntFreeFunct);
//    std::cout << "it3 after: " << it3 << std::endl;
//    std::cout << "---------------------\n" << std::endl;
//}

//void part7()
//{
//    Numeric<float>  ft3(3.0f);
//    Numeric<double> dt3(4.0);
//    Numeric<int>    it3(5);
//
//    std::cout << "Calling Numeric<float>::apply() using a lambda (adds 7.0f) and Numeric<float> as return type:" << std::endl;
//    std::cout << "ft3 before: " << ft3 << std::endl;
//
//    {
//        using Type = decltype(ft3)::Type;   // #4
//        using RtnType = decltype(ft3);
//
//        ft3.apply( [&ft3]( Type& num ) -> RtnType&
//        {
//            num += 7.0f;
//            return ft3;
//        } );
//    }
//
//    std::cout << "ft3 after: " << ft3 << std::endl;
//    std::cout << "Calling Numeric<float>::apply() twice using a free function (adds 7.0f) and void as return type:" << std::endl;
//    std::cout << "ft3 before: " << ft3 << std::endl;
//    ft3.apply(myNumericFreeFunct).apply(myNumericFreeFunct);
//    std::cout << "ft3 after: " << ft3 << std::endl;
//    std::cout << "---------------------\n" << std::endl;
//
//    std::cout << "Calling Numeric<double>::apply() using a lambda (adds 6.0) and Numeric<double> as return type:" << std::endl;
//    std::cout << "dt3 before: " << dt3 << std::endl;
//
//    {
//        using Type = decltype(dt3)::Type;  //#4
//        using RtnType = decltype(dt3);
//
//        dt3.apply( [&dt3]( Type& num ) -> RtnType&
//        {
//            num += 6.0;
//            return dt3;
//        } ); // This calls the templated apply fcn
//    }
//
//    std::cout << "dt3 after: " << dt3 << std::endl;
//    std::cout << "Calling Numeric<double>::apply() twice using a free function (adds 7.0) and void as return type:" << std::endl;
//    std::cout << "dt3 before: " << dt3 << std::endl;
//    dt3.apply(myNumericFreeFunct<double>).apply(myNumericFreeFunct<double>); // This calls the templated apply
//    std::cout << "dt3 after: " << dt3 << std::endl;
//    std::cout << "---------------------\n" << std::endl;
//
//    std::cout << "Calling Numeric<int>::apply() using a lambda (adds 5) and Numeric<int> as return type:" << std::endl;
//    std::cout << "it3 before: " << it3 << std::endl;
//
//    {
//        using Type = decltype(it3)::Type;  // #4
//        using RtnType = decltype(it3);
//
//        it3.apply( [&it3]( Type& num ) -> RtnType&
//        {
//            num += 5;
//            return it3;
//        } );
//    }
//    std::cout << "it3 after: " << it3 << std::endl;
//    std::cout << "Calling Numeric<int>::apply() twice using a free function (adds 7) and void as return type:" << std::endl;
//    std::cout << "it3 before: " << it3 << std::endl;
//    it3.apply(myNumericFreeFunct).apply(myNumericFreeFunct);
//    std::cout << "it3 after: " << it3 << std::endl;
//    std::cout << "---------------------\n" << std::endl;
//}

// #7
int main()
{
    Numeric<float> f(0.1f);
    Numeric<int> i(3);
    Numeric<double> d(4.2);
    
    f += 2.f;
    f -= i;
    f *= d;
    f /= 2.f;
    std::cout << "f: " << f << std::endl;
    
    d += 2.f;
    d -= i;
    d *= f;
    d /= 2.f;
    std::cout << "d: " << d << std::endl;
    
    i += 2.f; i -= f; i *= d; i /= 2.f;
    std::cout << "i: "<< i << std::endl;
    
    Point p(f, i);
    p.toString();
    
    d *= -1;
    std::cout << "d: " << d << std::endl;
    
    p.multiply(d.pow(f).pow(i));
    std::cout << "d: " << d << std::endl;
    
    p.toString();
    
    Numeric<float> floatNum(4.3f);
    Numeric<int> intNum(2);
    Numeric<int> intNum2(6);
    intNum = 2 + (intNum2 - 4) + static_cast<double>(floatNum) / 2.3;
    std::cout << "intNum: " << intNum << std::endl;
    
    {
        using Type = decltype(f)::Type;
        f.apply([&f](std::unique_ptr<Type>&value) -> decltype(f)&
                {
                    auto& v = *value;
                    v = v * v;
                    return f;
                });
        std::cout << "f squared: " << f << std::endl;
        
        f.apply( cube<Type> );
        std::cout << "f cubed: " << f << std::endl;
    }
    
    {
        using Type = decltype(d)::Type;
        d.apply([&d](std::unique_ptr<Type>&value) -> decltype(d)&
                {
                    auto& v = *value;
                    v = v * v;
                    return d;
                });
        std::cout << "d squared: " << d << std::endl;
        
        d.apply( cube<Type> );
        std::cout << "d cubed: " << d << std::endl;
    }
    
    {
        using Type = decltype(i)::Type;
        i.apply([&i](std::unique_ptr<Type>&value) -> decltype(i)&
                {
                    auto& v = *value;
                    v = v * v;
                    return i;
                });
        std::cout << "i squared: " << i << std::endl;
        
        i.apply( cube<Type> );
        std::cout << "i cubed: " << i << std::endl;
    }
}
