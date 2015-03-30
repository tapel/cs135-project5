//Name: Taryn Apel
//Class: CS135
//Assignment: Project 5
//Date: 3/27/15

#include <iostream> // needed for cin and cout
#include <vector> // needed for dealing with arrays of dynamic size
#include <string> // needed for dealing with strings
#include <fstream> // needed for file streams
#include <sstream> // needed for string streams
#include <algorithm> // needed for find function
//#include "rotUtils.h" // i included this header in my own file to avoid dependencies

using namespace std;

// functions copied from professor-provided rotUtils.cpp and rotUtils.h
// so that i don't have to use a makefile to compile
// i was afraid you would not know i was using a makefile
// and as a result not be able to compile it
//    **** vvvvvvvvvv code provided from professor git pull vvvvvvvvv ****
const int MINCHAR = 33;
const int MAXCHAR = 126;
const int RANGECHAR = MAXCHAR - MINCHAR;
int rotate(int c, int n);
int rotateMinus(int);
int rotatePlus(int);

int rotate(int c, int n){
  if (n == 0) return c;
  int nRot = abs(n) % (RANGECHAR + 1);
  if(n > 0)
    return rotatePlus(c + nRot);
  else
    return rotateMinus(c - nRot);
}

int rotatePlus(int sum){
  int diff = sum - MAXCHAR;
  if (sum > MAXCHAR) sum = MINCHAR + diff - 1;
  return sum;
}

int rotateMinus(int sum){
  int diff = MINCHAR - sum;
  if (sum < MINCHAR) sum = MAXCHAR - diff + 1;
  return sum;
}
//    **** ^^^^^^^^^ code provided from professor git pull ^^^^^^^^^^ ****

/* ******* MY ORIGINAL CODE IS FROM HERE DOWN ******** */

int cipherkeymin = -50;
int cipherkeymax = 50;
bool keyfound = false;
string matchword = "";
int matchkey = cipherkeymin-1;

// cant use the old search() method from searcher.cpp
// because i stored the dynamic-length dictionary in a vector of strings
// instead of a simple array
// this is my function to search decrypted text against the dictionary
// adapted from http://www.cplusplus.com/forum/beginner/101175/
// because i didn't know about iterators
bool search(vector<string> dictionary, string key){
	std::vector<string>::iterator instance;
	instance = find(dictionary.begin(), dictionary.end(), key);
	if (instance != dictionary.end()) {
		return true;
	}
	else {
		return false;
	}
}

int main(int argc, char* argv[]){
	// declare variables
	// i needed a vector of strings instead of a normal array
	// because i was working with dynamic size
	// vector allows this more easily
	vector<string> wordlist;
	string wordline;
	ifstream dict(argv[2]);
	

	// read dictionary into array
	while ( getline (dict, wordline) ) {
		wordlist.push_back(wordline);
	}

	vector<string> encryptedwords;
	string encryptedline;
	ifstream encrypt(argv[1]);
	
	// read encrypted file into array
	while ( getline (encrypt, encryptedline) ) {
		encryptedwords.push_back(encryptedline);
		// new lines weren't appearing when i printed the decoded text at the end
		// so i brute forced them into the vector
		encryptedwords.push_back("\n");
	}

	// loop from -50 to 50
	// code was taking a while to process so i added a searching message
	cout << "Searching!\n";
	std::string thismessage ("");
	// shiftindex is the "key" number by which im shifting the alphabet to break the code
	for(int shiftindex = cipherkeymin; shiftindex <= cipherkeymax; shiftindex++) {
		// decrypt using rotate function
		// get chars from encryptedwords vector
		int numlinesencrypted = encryptedwords.size();
		int encryptedlineindex=0;
		std::string currentword ("");
		for (encryptedlineindex = 0; encryptedlineindex < numlinesencrypted; encryptedlineindex++) {
			// convert to input stream for getting characters to rotate
			std::istringstream iss;
  			iss.str (encryptedwords[encryptedlineindex]);
  			int asciichar;
  			int specialchar;
  			
  			// read decrypted contents as input string with istringstream
			while( (asciichar = iss.get()) != EOF){
				if(!isspace(asciichar) ){
					
					// testing purposes
					// cout << "       *** Original char=" << asciichar << "\n";
					
					asciichar = rotate(asciichar, shiftindex);
					
					// testing purposes
					// cout << "       *** Rotated  char=" << asciichar << "\n";
					
					// appending the ascii character we just rotated to currentword
					currentword.append<int>(1,asciichar);
				}
				else {
					specialchar = asciichar;
					
					// loop through dictionary
					// and find the dictionary array index for currentword
					keyfound = search(wordlist, currentword);
					// and if i found a dictionary index
					// here im recording what the matched word and cipher key are
					if (keyfound == true){
						matchword = currentword;
						matchkey = shiftindex;
					}
					
					// heres where im storing the whole decoded message
					// word by word
					// so i can print it later if the cipher key was right
					thismessage.append(currentword);
					// pushing unrotated characters (whitespace) into the message
					// so words dont blur together
					thismessage.append<int>(1,specialchar);
					// resetting current word whenever whitespace is found
					currentword="";
				}

			} // end of while loop through rotating characters
			

			
		} // end of loop through lines of encrypted text
		

		// and if i have decoded the message,
		// then break out of this outer for loop
		if (matchword != ""){
			cout << thismessage << "\n";
			cout << "Match found: " << matchword << "\n";
			cout << "The key is " << matchkey << "\n";
						
			break;
		}
		// if the key wasnt found, clear thismessage because it would be gibberish
		// since the rotation integer wasn't right
		else {
			thismessage = "";
		}

	} // end of loop through cipherkey shiftindexes
}

