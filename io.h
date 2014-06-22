//===================================================================
// File:        io.h
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


#ifndef _IO_H_
#define _IO_H_


#include <stdio.h>


void setInput(FILE *stream);
void setOutput(FILE *stream);

int getBit(void);
int getByte(void);

void putBit(int b);
void putByte(int b);

void flushOutput(void);


#endif /* _IO_H_ */
