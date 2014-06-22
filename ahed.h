//===================================================================
// File:        ahed.h
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        Wed May  2 02:33:04 CEST 2012
// Comments:    Interface to coding library
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


#ifndef _AHED_H_
#define _AHED_H_


#include <stdio.h>
#include <stdint.h>


#define OK 			 0
#define ERROR 	-1


typedef struct
{
	int64_t uncodedSize;
	int64_t codedSize;
} tAHED;


int AHEDEncoding(tAHED *ahed, FILE *inputFile, FILE *outputFile);
int AHEDDecoding(tAHED *ahed, FILE *inputFile, FILE *outputFile);


#endif /* _AHED_H_ */
