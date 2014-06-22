//===================================================================
// File:        tree.h
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        Wed May  2 02:33:04 CEST 2012
// Comments:    Huffman tree manipulation
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


#ifndef _TREE_H_
#define _TREE_H_


#include <iostream>
#include <stack>


// 8 bit symbol
static const int SYM_COUNT = 256;
static const int TREE_SZ = 2 * SYM_COUNT + 1;


class HTree
{
public:
	enum { NONE = -1, ROOT = TREE_SZ - 1 };
	typedef int NodeIndex;
	typedef std::stack<bool> Stack;
private:
	struct Node
	{
		unsigned char val;
		unsigned weight;
		NodeIndex parent;
		NodeIndex left;
		NodeIndex right;
	};
private:
	NodeIndex esc;
	NodeIndex sym[SYM_COUNT];
	NodeIndex free;
	Node node[TREE_SZ];
private:
	void rebuild();
	void rescale(NodeIndex n);
	void swap(NodeIndex &a, NodeIndex &b);
	void pathTo(NodeIndex i, Stack &stack) const;
public:
	HTree();

	NodeIndex find(unsigned char s) const;
	NodeIndex insert(unsigned char s);
	void update(NodeIndex n);

	void pathNode(NodeIndex n, Stack &stack) const;
	void pathEscape(Stack &stack) const;

	bool trace(NodeIndex &n, int p) const;
	bool isEscape(NodeIndex n) const;
	unsigned char getSymbol(NodeIndex n) const;
};


#endif /* _TREE_H_ */
