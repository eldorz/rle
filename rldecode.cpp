#include <limits>

#include "rldecode.h"
#include "common.h"

using namespace std;

int main (int argc, char **argv) {

  // command line arguments
  if (argc < 2 || argc > 3) {
    usage(argv);
    exit(1);
  }

  // set operation mode
  int mode = STDOUT_MODE;
  if (argc == 3) mode = OUTFILE_MODE;

  // open files
  ifstream infile;
  infile.open(argv[1], ios::in | ios::binary);
  if (!infile.is_open()) {
    cerr << "Unable to open input file " << argv[1] << endl;
    exit(1);
  }

  ofstream outfile;
  if (mode == OUTFILE_MODE) {
    outfile.open(argv[2], ios::out | ios::binary);
    if (!outfile.is_open()) {
      cerr << "Unable to open output file " << argv[2] << endl;
      exit(1);
    }
  }

  // perform decoding
  decode(infile, outfile, mode);

  // close files
  infile.close();
  if (outfile.is_open()) outfile.close();

  return 0;
}

void decode(ifstream &infile, ofstream &outfile, int mode) {

  // read the first character
  char inbyte = 0;
  if (!infile.read(&inbyte, 1)) {
    if (infile.eof()) return; // empty file, do nothing
    cerr << "Failed reading first character of input file" << endl;
    exit(1);
  }
  
  char current_char =  0;
  bool char_signed = numeric_limits<char>::is_signed;
  unsigned int count = 0;

  while (infile) {
    // if first bit not set, print character
    if ((char_signed && inbyte >= 0) || (!char_signed && inbyte < 0x80)) {
      if (mode == STDOUT_MODE) cout << inbyte;
      else outfile << inbyte;
      current_char = inbyte;
      infile.read(&inbyte, 1);
      continue;
    }

    // otherwise get the number of characters
    // strip first bit
    count = inbyte & 0x7f;
    int i = 1; // count how many 7 bit frames to shift
    while (infile.read(&inbyte, 1) && 
           ((char_signed && inbyte < 0) || 
	    (!char_signed && inbyte >= 0x80))) {
      unsigned int temp = inbyte & 0x7f;
      temp <<= 7 * i++;
      count |= temp;
    }
    if (mode == STDOUT_MODE) cout << "[" << count << "]";
    else {
      // need to add three but note we've already printed one
      count += 2;
      for (unsigned i = 0; i < count; ++i) {
	outfile << current_char;
      }
    }
  }
}

