/* Fibonacci.cpp compares different C++ implementations of the classical fibonacci sequence problem
    Written on April 2013 by Chunjing Wang

    The problem:  to find the nth number in the fibonacci sequence
    
  what is Fibonacci sequence? 
    By definition, the first two numbers in the Fibonacci sequence are 0 and 1, and each subsequent number is the sum of the previous two.
    In mathematical terms, the sequence F(n) of Fibonacci numbers is defined by the recurrence relation: F(n) = F(n-1) + F(n-2)
    The first numbers in the sequence are: 0, 1, 1, 2, 3, 5, 8, 13, 21, 34 etc. 
*/

#include<iostream>
using namespace std;

/*==========================================Solution 1=====================================================
    solution 1: a naive solution is to make use of recurrence relation formula  F(n) = F(n-1) + F(n-2) and simulate recursive behavior 
    function name: Fibonacci_recursive
	input: index n
	return the nth entry in the sequence
	complexity: exponential of n
*/

int Fibonacci_recursive(int n) 
{
    if(n <= 0)
        return 0;

    if(n == 1)
        return 1; // these two are the base cases, initialize first two entries in the sequence

    // from n >= 2, we use this relation to calculate every subsequent entry from previous two entries
    return Fibonacci_recursive(n - 1) + Fibonacci_recursive(n - 2);   
}
/* The above solution is intuitive but inefficient: e.g. we calculate F(10) based on F(9) and F(8), and F(9) is calculated based on F(8) and F(7)
    and F(8) is calculated basaed on F(7) and F(6), so there are a lot duplicate calculations which will cause serious performance issues.
    The time complexity is O(2^n)
 */


/*==========================================Solution 2===============================================
   Solution 2: iterative solution, improved performance. This solution avoids duplicated calculations.
   we calculate each entry and store them for future usage, so for every entry f(n), we retrieve previously calculated f(n-1) and f(n-2) to get f(n)
   function name: Fibonacci_iterative
   input: index n
   return the nth entry in the sequence
   complexity: O(n)
*/

int Fibonacci_iterative(int n)
{
    int result[2] = {0, 1};
    if(n < 2)
        return result[n];  // if n = 0 or 1, return 0 or 1
        
    int fibNMinusOne = 1;  // initialize f(n-1) to be 1
    int fibNMinusTwo = 0;  // initialize f(n-2) to be 0
    int fibN; 
    //so we can calculate f(2) based on f(1) anf f(0)
    for(int i = 2; i <= n; ++i)
    {
        fibN = fibNMinusOne + fibNMinusTwo; //for each loop, we calculate f(n) = f(n-1) + f(n-2)

        fibNMinusTwo = fibNMinusOne;  //then update f(n-2) = f(n-1) and f(n-1) = f(n) to calcuate f(n+1) when index i increments
        fibNMinusOne = fibN;
    }

     return fibN;
}
// the time complexity is O(n) since the whole sequence is calcuated once. 

/*==========================================Solution 3===============================================
   Solution 3: we observe this linear algebra relation between F(n), F(n-1) and F(n-2)
    [1 1][f(1)]   [f(2)]  [1 1]^2    [f(3)]        [f(n)   ]    [1 1]^(n-1) [f(1)]
    [1 0][f(0)]= [f(1)], [1 0]    = [f(2)]        [f(n-1)] = [1 0]          [f(0)]
   with this formula, we will calculate the n-1 th power of the 2x2 matrix [1,1][1,0] and get the [1,1] entry of the result
   function name: Fibonacci_power
   input: index n
   return the nth entry in the sequence
   complexity: O(log(n))
*/

// Two function multiply and power are called in the Fibonacci_power function
void multiply(int F[2][2], int M[2][2]);  //multiply takes two matrices F and M, do matrix multiplication and return the product
void power(int F[2][2], int n); // power takes matrix F and return F^n

// Fibonacci_power function calls power function, raises the base matrix to its (n-1)th power
int Fibonacci_power(int n) 
{
  int F[2][2] = {{1,1},{1,0}}; //This the base matrix to be raised
  if(n == 0)
    return 0;  // f(0) = 0;
  power(F, n-1);  // otherwise raise F to its (n-1)th power
  return F[0][0]; // and return its top left corner entry 
}
 
// implementation of recursive function power
void power(int F[2][2], int n) 
{
  if( n == 0 || n == 1)
      return;  // base case, function just returns to previous recursion layer

  int M[2][2] = {{1,1},{1,0}};
  // now we need to consider whether n is odd or even, if it's even, a^(n/2) * a^(n/2) = a^n,  so to calculate a^n we call a^(n/2);
  power(F, n/2); 
  multiply(F, F);  // get the square value of F whenever the power function is called; the previous step divide power n to n/2 and this step gets back to n;
 
  if( n%2 != 0 )  // in cases if n is odd, a^((n-1)/2) * a^((n-1)/2) * a = a^n, so we need to multiply the base matrix M again
     multiply(F, M);
}
 
// implementation of function multiply, which takes two 2*2 matrices and return the products of their multiplication
void multiply(int F[2][2], int M[2][2])
{
  int x =  F[0][0]*M[0][0] + F[0][1]*M[1][0];
  int y =  F[0][0]*M[0][1] + F[0][1]*M[1][1];
  int z =  F[1][0]*M[0][0] + F[1][1]*M[1][0];
  int w =  F[1][0]*M[0][1] + F[1][1]*M[1][1];
 
  F[0][0] = x;
  F[0][1] = y;
  F[1][0] = z;
  F[1][1] = w;
}
/* ==========================================Solution 4=============================================== 
  A similar algorithm also in log(n) time is direct computation.
  The recurrence formula is (a^n -b^n)/sqrt(5) for the nth number in the sequence
  function name: Fibonacci_goldenratio
   input: index n
   return the nth entry in the sequence
   complexity: O(log(n))
*/
int Fibonacci_goldenratio(int n){
    double p_ratio = (1+sqrt(5.))/2.;  
    double n_ratio = (1-sqrt(5.))/2.; // compute the positive and negative golden ratios
    double p_power = pow(p_ratio, n);
    double n_power = pow(n_ratio, n); // computer the nth power of the ratios
	int fib_n = (int)((p_power-n_power)/sqrt(5.)); 
    return fib_n; // round to int
}

/* ==========================================Solution 5=============================================== 
  Solution 4: this solution utilizes a technique called template metaprogramming which allows execution
  of programs in compile time. All fibonacci sequence entries are generated on the fly by the compiler.
  struct/class name: Fibonacci_meta
  input: index n
  return the nth entry i the sequence
  complexity: constant
*/
template <int n>
struct Fibonacci_meta
{
       enum{
            value = Fibonacci_meta<n-1>::value + Fibonacci_meta<n-2>::value
       };
}; // template for fib_n

template <>
struct Fibonacci_meta<0>
{
	enum{
		value = 0
	};
}; // template for fib_0

template <>
struct Fibonacci_meta<1>
{
	enum{
		value = 1
	};
}; 

/* ==========================================Solution 6=============================================== 
  Solution : this solution utilizes a generator class to get the next fibonacci entry from the previous two entries
  The class generator_fib has 1) a constructor which initializes the first two entry; 2) a getnext() function whose every call will add a new entry
  3) reset() function which return to the starting state.
*/

class generator_fib
{
private:
	int prev1;
	int prev2;
public:
	generator_fib(): prev2(-1), prev1(1){}
	int getnext(){
		int curr = prev2 + prev1; // compute current value based on previous two entries
		prev2 = prev1;
		prev1 = curr; // update previous two entries to their next values
		return curr; // return current value
	}
	void reset(){
		int prev2 = -1;
		int prev1 = 1; // reset the generator to starting state
	}
}
