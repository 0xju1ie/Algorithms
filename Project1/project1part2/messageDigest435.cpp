//Julie Aichinger

#include <string.h>
#include <iostream>
#include <fstream>
#include "sha256.h"
#include "BigIntegerLibrary.hh"
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;

BigUnsigned hashIt(char block[])
{
   cout << "converting string into hash using sha256" << endl;
   string blockStr(block);
   string inHashFileInfo = sha256(blockStr);

   cout << "turning hashed string into BigUnsigned" << endl;
   BigUnsignedInABase inHashFileNum = BigUnsignedInABase(inHashFileInfo, 16);
   return BigUnsigned(inHashFileNum);
}

int main(int argc, char *argv[])
{
   try
   {

      //Second part of your project starts here
      if (argc != 3 || (argv[1][0] != 's' && argv[1][0] != 'v'))
         cout << "wrong format! should be \"a.exe s filename\"";
      else
      {
         string filename = argv[2];

         //read the file
         std::streampos begin, end;
         ifstream myfile(filename.c_str(), std::ios::binary);
         begin = myfile.tellg();
         myfile.seekg(0, std::ios::end);
         end = myfile.tellg();
         std::streampos size = end - begin;
         

         myfile.seekg(0, std::ios::beg);
         char *memblock = new char[size];
         myfile.read(memblock, size); //read file; it's saved in the char array memblock
         myfile.close();

         //Generate a SHA256 hash of the info in the file to be signed
         BigUnsigned hashedContent = hashIt(memblock);
         cout << "This is the hashed document content: " << hashedContent << endl;

         if (argv[1][0] == 's')
         {
            cout << "\n"
                 << "Need to sign the doc.\n";
            cout << "Going to start the signing of the document process..." << endl;
            cout << "Loading in d_n.txt" << endl;
            ifstream d_n("d_n.txt");
            string d, n;
            while (d_n >> d >> n)
            {
            }
            d_n.close();

            cout << "Getting the values of d and n" << endl;

            BigUnsigned d_bu = stringToBigUnsigned(d);
            BigUnsigned n_bu = stringToBigUnsigned(n);
            cout << "this is d: " << d_bu << endl;
            cout << "this is n: " << n_bu << endl;

            cout << "signing the hashed content..." << endl;
            BigUnsigned signedInfo = modexp(hashedContent, d_bu, n_bu);
            cout << "This is the signed content: " << signedInfo << endl;

            cout << "saving this signed content to a file, file.txt.signature" << endl;
            ofstream signatureFile;
            signatureFile.open(filename+ ".signature", ofstream::out | ofstream::trunc);
            signatureFile << signedInfo;
            signatureFile.close();

            cout << "File: " << filename << " has been signed and saved" << endl;
         }
         else
         {
            cout << "\n"
                 << "Need to verify the signature...\n";
            
            ifstream signedfile(filename+ ".signature");
            
            string signatureStr;
            while (signedfile >> signatureStr)
            {
            }
            signedfile.close();
            cout << signatureStr << endl;
            BigUnsigned signature_bu = stringToBigUnsigned(signatureStr);
            cout << "This is the signature from the file.txt.signature file: " << signature_bu << endl;

            cout << "Loading in e_n.txt" << endl;
            ifstream e_n("e_n.txt");
            string e, n;
            while (e_n >> e >> n)
            {
            }
            e_n.close();

            cout << "Getting the values of e and n" << endl;

            BigUnsigned e_bu = stringToBigUnsigned(e);
            BigUnsigned n_bu = stringToBigUnsigned(n);
            cout << "this is e: " << e << endl;
            cout << "this is n: " << n << endl;

            cout << "getting the encrypted signature..." << endl;
            BigUnsigned encryptSig = modexp(signature_bu, e_bu, n_bu);
            cout << "This is the encrypted signature: " << encryptSig << endl;
            cout << "hashedcontent: " << hashedContent << endl;

            cout << "Comparing the hashed content with the encrypted signature to verify..." << endl;
            if (encryptSig == hashedContent)
            {
               cout << "Congrats! This has been verified. That means that the file has not been changed after it was signed." << endl;
            }
            else
            {
               cout << "Uh-oh. This file was modified after it was signed..." << endl;
            }
         }
         delete[] memblock;
      }
   }
   catch (char const *err)
   {
      std::cout << "The library threw an exception:\n"
                << err << std::endl;
   }
   return 0;
}
