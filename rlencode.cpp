#include <iostream>
#include <fstream>

#include "rlencode.h"

#define STDOUT_MODE 0
#define OUTFILE_MODE 1
#define START_FLAG -1

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

  // perform encoding
  encode(infile, outfile, mode);

  // close files
  infile.close();
  if (outfile.is_open()) outfile.close();

  return 0;
}

void usage(char **argv) {
  cerr << "Usage: " << argv[0] << " INFILE [OUTFILE]" << endl;
}

void encode(ifstream &infile, ofstream &outfile, int mode) {

  // read the first character
  char inbyte = 0;
  if (!infile.read(&inbyte,1)) {
    // failed reading first character
    if (infile.eof()) return;  // empty file, do nothing
    cerr << "Failed reading first character of input file" << endl;
    infile.close();
    outfile.close();
    exit(1);
  }
    
  char oldbyte = START_FLAG;
  unsigned int count = 1;

  // read input stream one byte at a time
  while(infile) {
    
    // check that input is standard ASCII
    if (inbyte > 127) {
      cerr << "Input file contains non-standard ASCII" << endl;
      infile.close();
      if (outfile) outfile.close();
      exit(1);
    }

    // increment count if same as before
    if (inbyte == oldbyte) count++;
    // otherwise output code for previous character and reset count
    else if (oldbyte != START_FLAG) {
      output_code(outfile, oldbyte, count, mode);
      count = 1;
    }

    // get a new character
    oldbyte = inbyte;
    infile.read(&inbyte,1);
  }

  // output the last character
  output_code(outfile, oldbyte, count, mode);

  if (mode == STDOUT_MODE) cout << endl;

  return;
}

void output_code(ofstream &outfile, char byte, unsigned int count, int mode) {

  // output the characters only if there are 2 or less
  if (count <= 2) {
    while (count-- > 0) {
      if (mode == OUTFILE_MODE) outfile << byte;
      else cout << byte;
    }
    return;
  }
  
  // otherwise output byte and count
  if (mode == OUTFILE_MODE) outfile << byte;
  else cout << byte;
  
  if (mode == OUTFILE_MODE) {
    while (count > 0x7F) {
      // won't fit into seven bits, need to split
      char least_sig = count & 0x7F;
      // prepend a flag bit and output
      least_sig |= 0x80;
      outfile << least_sig;
      count >>= 7;
    }

    // output last 7 bits
    count |= 0x80;
    outfile << (char)count;
  }
  else cout << "[" << count << "]";
}
