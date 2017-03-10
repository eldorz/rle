#include <iostream>
#include <fstream>

#include "rlencode.h"

#define STDOUT_MODE 0
#define OUTFILE_MODE 1

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
  if (mode == OUTFILE_MODE) outfile_encode(infile, outfile);

  // close files
  infile.close();
  if (outfile.is_open()) outfile.close();

  return 0;
}

void usage(char **argv) {
  cerr << "Usage: " << argv[0] << " INFILE [OUTFILE]" << endl;
}

void outfile_encode(ifstream &infile, ofstream &outfile) {

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
    
  char oldbyte = 255;
  int count = 1;

  // read input stream one byte at a time
  while(infile) {
    
    // check that input is standard ASCII
    if (inbyte > 127) {
      cerr << "Input file contains non-standard ASCII" << endl;
      infile.close();
      outfile.close();
      exit(1);
    }

    // count the number of occurrences of this character
    if (inbyte == oldbyte) count++;
    else output_code(outfile, oldbyte, count);

    // get a new character
    oldbyte = inbyte;
    infile.read(&inbyte,1);
  }

  return;
}

void output_code(ofstream &outfile, char byte, int count) {
  outfile << byte << " " << count << endl;
}
