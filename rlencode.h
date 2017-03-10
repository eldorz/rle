#ifndef RLENCODE_H
#define RLENCODE_H

#include <iostream>
#include <fstream>

void usage(char **argv);
void outfile_encode(std::ifstream &infile, std::ofstream &outfile);
void output_code(std::ofstream &outfile, char byte, int count);

#endif
