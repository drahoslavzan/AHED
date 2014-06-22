//===================================================================
// File:        ahed.cc
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        Wed May  2 02:33:04 CEST 2012
// Comments:    Implementation of coding library
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
#include "io.h"
#include "tree.h"

#include <cstdio>


//using namespace std;


// Handling end of data stream:
//
// New symbol:
//   ESCAPE 0 SYMBOL
// End of stream
//   ESCAPE 1


int AHEDEncoding(tAHED *ahed, FILE *inputFile, FILE *outputFile)
{
	setOutput(outputFile);

	HTree tree;
	HTree::Stack stack;

	int c;
	while((c = fgetc(inputFile)) != EOF)
	{
		int n = tree.find(c);

		if(n == HTree::NONE)
		{
			tree.pathEscape(stack);

			while(!stack.empty())
			{
				putBit(stack.top());

				//cout << stack.top();

				stack.pop();
			}

			putBit(0); 			// not EOF

			putByte(c);

			//cout << (char)c;

			tree.update(tree.insert(c));
		}
		else
		{
			tree.pathNode(n, stack);

			while(!stack.empty())
			{
				putBit(stack.top());

				//cout << stack.top();

				stack.pop();
			}

			tree.update(n);
		}
	}

	//cout << endl;

	tree.pathEscape(stack);

	while(!stack.empty())
	{
		putBit(stack.top());
		stack.pop();
	}

	putBit(1); 			// EOF

	flushOutput();

	ahed->uncodedSize = ftell(inputFile);
	ahed->codedSize = ftell(outputFile);

	return OK;
}

int AHEDDecoding(tAHED *ahed, FILE *inputFile, FILE *outputFile)
{
	setInput(inputFile);

	HTree tree;

	int c = getBit();

	if(!c) 		// not EOF
	{
		int c = getByte();

		//cout << (char)c;

		fputc(c, outputFile);

		tree.update(tree.insert(c));

		for(;;)
		{
			int c = getBit();

			if(c == EOF) return ERROR;

			HTree::NodeIndex i = HTree::ROOT;
			while(!tree.trace(i, c))
				if((c = getBit()) == EOF) return ERROR;

			if(tree.isEscape(i))
			{
				if(getBit()) 		// EOF
					break;

				c = getByte();

				if(tree.find(c) != HTree::NONE) return ERROR;

				//cout << (char)c;

				fputc(c, outputFile);

				tree.update(tree.insert(c));
			}
			else
			{
				//cout << tree.getSymbol(i);

				fputc(tree.getSymbol(i), outputFile);

				tree.update(i);
			}
		}
	}

	ahed->uncodedSize = ftell(outputFile);
	ahed->codedSize = ftell(inputFile);

	return OK;
}

