//===================================================================
// File:        tree.cc
// Author:      Drahoslav Zan
// Email:       izan@fit.vutbr.cz
// Affiliation: Brno University of Technology,
//              Faculty of Information Technology
// Date:        Wed May  2 02:33:04 CEST 2012
// Comments:    Huffman tree manipulation using FGK algorithm
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


#include "tree.h"

#include <cstring>
#include <cassert>
#include <climits>


void HTree::rebuild()
{
	for(NodeIndex i = esc; i < ROOT; ++i)
		assert(node[i].weight <= node[i + 1].weight);

	rescale(ROOT);

	// Rebuild

	NodeIndex i = esc + 1;
	while(i < ROOT)
	{
		NodeIndex j = i + 1;
		if(node[i].weight > node[j].weight)
		{
			swap(i, j);
			i = esc + 1;
		}
		else ++i;
	}

	for(NodeIndex i = esc; i < ROOT; ++i)
		assert(node[i].weight <= node[i + 1].weight);
}

void HTree::rescale(HTree::NodeIndex n)
{
	assert(n != NONE);

	if(node[n].left == NONE) 		// is leaf
	{
		assert(node[n].right == NONE);

		node[n].weight = (node[n].weight + 1) / 2;

		return;
	}

	rescale(node[n].left);
	rescale(node[n].right);

	node[n].weight = node[node[n].left].weight + node[node[n].right].weight;
}

void HTree::swap(NodeIndex &a, NodeIndex &b)
{
	assert(a >= 0 && a < TREE_SZ);
	assert(b >= 0 && b < TREE_SZ);
	assert(a != ROOT);
	assert(b != ROOT);

	if(a == b)
		return;

	if(a == node[b].parent || b == node[a].parent)
		return;

	Node &aa = node[a];
	Node &bb = node[b];

	Node tmp;
	NodeIndex parent;

	tmp = aa;

	parent = aa.parent;
	if(aa.left == NONE) 		// is leaf
	{
		assert(aa.right == NONE);

		sym[(unsigned)aa.val] = b;
	}
	else
	{
		assert(aa.left != NONE);
		assert(aa.right != NONE);

		node[aa.left].parent = b;
		node[aa.right].parent = b;
	}

	aa = bb;
	aa.parent = parent;

	parent = bb.parent;
	if(bb.left == NONE) 		// is leaf
	{
		assert(bb.left == NONE);
		assert(bb.right == NONE);

		sym[(unsigned)bb.val] = a;
	}
	else
	{
		assert(bb.left != NONE);
		assert(bb.right != NONE);

		node[bb.left].parent = a;
		node[bb.right].parent = a;
	}

	bb = tmp;
	bb.parent = parent;

	assert(node[aa.parent].left == a || node[aa.parent].right == a);
	assert(node[bb.parent].left == b || node[bb.parent].right == b);

	// Swap(a,b)
	parent = a;
	a = b;
	b = parent;
}

void HTree::pathTo(HTree::NodeIndex i, HTree::Stack &stack) const
{
	assert(i >= 0 && i < TREE_SZ);

	assert(stack.empty());

	if(i == ROOT)
		return;

	NodeIndex p = node[i].parent;
	do
	{
		if(node[p].left == i)
			stack.push(0);
		else
		{
			assert(node[p].right == i);
			stack.push(1);
		}
		i = p;
		p = node[i].parent;
	} while(i != ROOT);
}

HTree::HTree()
{
	esc = ROOT;

	memset(sym, NONE, sizeof(sym));

	memset(node, 0, sizeof(node));

	node[ROOT].val = 0;
	node[ROOT].weight = 0;
	node[ROOT].parent = NONE;
	node[ROOT].left = NONE;
	node[ROOT].right = NONE;

	free = ROOT - 1;
}

HTree::NodeIndex HTree::find(unsigned char s) const
{
	return sym[(unsigned)s];
}

HTree::NodeIndex HTree::insert(unsigned char s)
{
	assert(find(s) == NONE);

	NodeIndex n = esc;

	assert(free > 0);

	node[n].right = free--;
	node[n].left = free--;

	Node &l = node[node[n].left];
	Node &r = node[node[n].right];

	l.val = 0;
	l.weight = 0;
	l.parent = n;
	l.left = NONE;
	l.right = NONE;

	r.val = s;
	r.weight = 1;
	r.parent = n;
	r.left = NONE;
	r.right = NONE;

	esc = node[n].left;
	sym[(unsigned)s] = node[n].right;

	return n;
}

void HTree::pathNode(HTree::NodeIndex n, HTree::Stack &stack) const
{
	pathTo(n, stack);
}

void HTree::pathEscape(HTree::Stack &stack) const
{
	pathTo(esc, stack);
}

void HTree::update(HTree::NodeIndex n)
{
	assert(n >= 0 && n < TREE_SZ);

	while(n != ROOT)
	{
		int i;
		for(i = n + 1; i < ROOT; ++i)
		{
			assert(node[n].weight <= node[i].weight);

			if(node[n].weight < node[i].weight)
				break;
		}
		--i;

		swap(n, i);

		node[n].weight++;
		n = node[n].parent;
	}

	node[n].weight++;

	if(node[n].weight == (UINT_MAX - 1))
		rebuild();
}

bool HTree::trace(HTree::NodeIndex &n, int p) const
{
	assert(n >= 0 && n < TREE_SZ);
	assert(p == 0 || p == 1);

	assert(node[n].left != NONE); 		// not leaf

	if(!p)
		n = node[n].left;
	else
		n = node[n].right;

	if(node[n].left == NONE)
		return true;

	return false;
}

bool HTree::isEscape(NodeIndex n) const
{
	assert(n >= 0 && n < TREE_SZ);

	return n == esc;
}

unsigned char HTree::getSymbol(NodeIndex n) const
{
	assert(n >= 0 && n < TREE_SZ);

	return node[n].val;
}

