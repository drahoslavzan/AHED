//===================================================================
// File:        io.cc
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        Wed May  2 02:33:04 CEST 2012
// Comments:    Input/Ouput at bit level
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


#include "io.h"

#include <assert.h>
#include <stdio.h>


static FILE *input = NULL;
static FILE *output = NULL;

static int inputBuffer;
static unsigned inputOffset = 0;

static unsigned char outputBuffer;
static unsigned outputOffset = 8;


static int recvInput(void)
{
	assert(input != NULL);

	if(!inputOffset)
	{
		inputOffset = 8;

		inputBuffer = fgetc(input);

		return inputBuffer;
	}

	return 0;
}

static void sendOutput(void)
{
	assert(output != NULL);

	if(!outputOffset)
	{
		outputOffset = 8;

		fputc(outputBuffer, output);
	}
}


void setInput(FILE *stream)
{
	input = stream;
}

void setOutput(FILE *stream)
{
	output = stream;
}

int getBit(void)
{
	if(recvInput() == EOF)
		return EOF;
	
	--inputOffset;

	return ((1 << inputOffset) & inputBuffer) >> inputOffset;
}

int getByte(void)
{
	if(recvInput() == EOF)
		return EOF;
	
	int b = 8 - inputOffset;
	int c = ~(0xFF << inputOffset) & inputBuffer;

	inputOffset = 0;

	if(recvInput() == EOF)
		return EOF;

	inputOffset -= b;

	return (c << b) | (((0xFF << inputOffset) & inputBuffer) >> inputOffset);
}

void putBit(int b)
{
	sendOutput();

	--outputOffset;

	outputBuffer = (~(1 << outputOffset) & outputBuffer) | ((b & 1) << outputOffset);
}

void putByte(int b)
{
	sendOutput();

	int s = 8 - outputOffset;

	outputBuffer = ((0xFF << outputOffset) & outputBuffer) | ((b & 0xFF) >> s);

	outputOffset = 0;
	sendOutput();

	outputOffset -= s;

	outputBuffer = (b & 0xFF) << outputOffset;
}

void flushOutput(void)
{
	outputBuffer &= (0xFF << outputOffset);

	outputOffset = 0;
	sendOutput();
}

