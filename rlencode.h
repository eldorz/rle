#ifndef RLENCODE_H
#define RLENCODE_H

#include <iostream>
#include <fstream>

void usage(char **argv);
void encode(std::ifstream &infile, std::ofstream &outfile, int mode);
void output_code(std::ofstream &outfile, char byte, unsigned int count, 
  int mode);

#endif
