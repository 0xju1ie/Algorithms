// You need to complete this program for a part of your first project.

// Standard libraries
#include <string>
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <fstream>
using std::cout;
using std::ofstream;
using std::endl;


// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerLibrary.hh"

//generate random function
BigUnsigned generateRandom(BigUnsigned b)
{
   BigUnsigned a = BigUnsigned(1);
   for(int j = 0; b >= j; j++)
   {
      a = a * 10 + rand();
   }
   return a;
}

//implementation of Fermat test
bool passFermatTest(BigUnsigned a)
{
   BigUnsigned a1(1);
   BigUnsigned a2(2);
   bool pass = (modexp(a1, a-1, a) == 1) && (modexp(a2, a-1, a) == 1);
   return(pass);
}

//make sure it is reletively prime using extended Euclidean algo
bool passEulcideanAlgo(BigUnsigned a, BigUnsigned b)
{
   bool pass = (gcd(a, b) == 1);
   return pass;
}

//using fermat's test to genreate two large prime numbers (p,q), each have size >= 512 bits (155 decimal digits)
BigUnsigned generatePrime()
{
   BigUnsigned a = generateRandom(155);
   while(!passFermatTest(a))
   {
      a = generateRandom(155);
   }
   return a;
}

BigUnsigned getE(BigUnsigned a)
{
   BigUnsigned e = generateRandom(100);
   while(!passEulcideanAlgo(e, a))
   {
      e = generateRandom(100);
   }
   return e;
}

int main(){
	/* The library throws 'const char *' error messages when things go
	 * wrong.  It's a good idea to catch them using a 'try' block like this
	 * one.  Your C++ compiler might need a command-line option to compile
	 * code that uses exceptions. */
   srand(time(NULL));
	try {

        cout << "Getting those prime numbers, p and q..." << endl;

        //p and q are both large prime numbers

        //generate p
        BigUnsigned p = generatePrime();
        cout << "p is " << p << endl;

        //generate q
        BigUnsigned q = generatePrime();
        cout << "q is " << q << endl;

        cout << "Putting p and q into p_q.txt (one int per line, no whitespace)" << endl;
        ofstream p_q("p_q.txt");
        p_q << p << "\n" << q;
        p_q.close();

        cout << "Getting n..." << endl;
        BigUnsigned n = p*q;

        cout << "Finding (p-1)*(q-1)..." << endl;

        BigUnsigned multiplied = (p-1)*(q-1);

        //finding e
        cout << "Getting e..." << endl;
        BigUnsigned e = getE(multiplied);
        cout << "e is " << e << endl;

        cout << "Putting e into e_n.txt (one int per line, no whitespace)" << endl;
        ofstream e_n("e_n.txt");
        e_n << e << "\n" << n;
        e_n.close();

        //finding the pubic key, d
        cout << "Finding d..." << endl;
        BigUnsigned d = modinv(e, multiplied);
        cout << "d is " << d << endl;

        cout << "Putting d into d_n.txt (one int per line, no whitespace)" << endl;
        ofstream d_n("d_n.txt");
        d_n << d << "\n" << n;
        d_n.close();

        cout << "This is RSA Encrypted! yay" << endl;

	} catch(char const* err) {
		cout << "The library threw an exception:\n"
			<< err << endl;
	}

	return 0;
}
