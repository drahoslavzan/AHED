//===================================================================
// File:        main.cc
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        Wed May  2 02:33:04 CEST 2012
// Comments:    Program entry point
// Project:     Adaptive Huffman Coding (AHC)
//-------------------------------------------------------------------
// Copyright (C) 2012 Drahoslav Zan
//
// This file is part of AHC.
//
// AHC is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// AHC is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with AHC. If not, see <http://www.gnu.org/licenses/>.
//===================================================================
// vim: set nowrap sw=2 ts=2


#include "ahed.h"

#include <getopt.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>


using namespace std;


extern char *optarg;
extern int optind, opterr, optopt;


static const char *progName;


void usage(ostream &stream, int ecode)
{
	stream << "USAGE: " << progName << " -{c|x} [OPTIONS]" << endl;
	stream << "Type `" << progName << " -h' for more information" << endl;

	exit(ecode);
}

void help(ostream &stream, int ecode)
{
	stream << "USAGE: " << progName << " -{c|x} [OPTIONS]" << endl << endl;
	stream
			<< "OPTIONS:" << endl
			<< "    -i <file>    Use <file> as input." << endl
			<< "    -o <file>    Use <file> as output." << endl
			<< "    -l <file>    Enable logging to <file>." << endl
			<< "    -c           Encode." << endl
			<< "    -x           Decode." << endl
			<< "    -h           Show this help and exit." << endl
			<< endl
			<< "DESCRIPTION:" << endl
			<< "File text de/compression using adaptive Huffman coding." << endl;

	exit(ecode);
}

int main(int argc, char **argv)
{
	progName = argv[0];

	if(argc == 1)
		usage(cout, 0);

	const char *infile = NULL;
	const char *outfile = NULL;
	const char *log = NULL;

	int (*action)(tAHED *, FILE *, FILE *) = NULL;

	int opt;
	while((opt = getopt(argc, argv, "i:o:l:cxh")) != -1)
		switch(opt)
		{
			case 'i':
				infile = optarg;
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'l':
				log = optarg;
				break;
			case 'c':
				action = &AHEDEncoding;
				break;
			case 'x':
				action = &AHEDDecoding;
				break;
			case 'h':
				help(cout, 0);
			case '?':
				usage(cerr, 1);
		}

	if(action == NULL)
	{
		cerr << "ERROR: Either -c or -x option must be provided" << endl;
		return 1;
	}

	FILE *input = stdin;
	FILE *output = stdout;

	if(infile != NULL)
	{
		input = fopen(infile, "r");
		if(input == NULL)
		{
			cerr << "ERROR: Cannot open file -- " << infile << endl;

			return 1;
		}
	}

	if(outfile != NULL)
	{
		output = fopen(outfile, "w");
		if(output == NULL)
		{
			cerr << "ERROR: Cannot open file -- " << outfile << endl;

			fclose(input);

			return 1;
		}
	}

	tAHED stat;

	if(action(&stat, input, output) == ERROR)
	{
		cerr << "ERROR: Coding failure" << endl;

		fclose(input);
		fclose(output);

		return 1;
	}

	if(log != NULL)
	{
		ofstream lf(log);

		if(!lf.good())
			cerr << "WARNING: Cannot open file -- " << log << endl;
		else
		{
			lf << "uncodedSize = " << stat.uncodedSize << endl;
			lf << "codedSize   = " << stat.codedSize << endl;
		}
	}

	fclose(input);
	fclose(output);

	return 0;
}
