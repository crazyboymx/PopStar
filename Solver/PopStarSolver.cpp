/*
 * PopStarSolver.cpp
 *
 *  Created on: 2013-7-10
 *      Author: maxing
 */

#include "PopStarSolver.h"
#include <set>
#include <map>
#include <cstring>

PopStarSolver::PopStarSolver(int row, int col)
    : m_row(row)
    , m_col(col)
    , m_size(row*col)
    , m_rDepth(1)
{
}

PopStarSolver::~PopStarSolver() {
}

void PopStarSolver::resize(int row, int col) {
	m_row = std::max(0, row);
	m_col = std::max(0, col);
	m_size = m_row * m_col;
}

void PopStarSolver::updateRow(int row) {
	resize(row, m_col);
}

void PopStarSolver::updateColumn(int col) {
    resize(m_row, col);
}

std::set<int> PopStarSolver::adjacent(int* matrix, int index) {
	std::set<int> result;
	adjacentInernal(matrix, index, result);
	return result;
}

int PopStarSolver::adjacentCount(int* matrix, int index) {
	return adjacent(matrix, index).size();
}

void PopStarSolver::adjacentInernal(int* matrix, int index, std::set<int>& adj) {
	adj.insert(index);
	int pos = left(index);
	if (pos != UNDEFINED && matrix[pos] == matrix[index] && adj.find(pos) == adj.end())
		adjacentInernal(matrix, pos, adj);
	pos = up(index);
	if (pos != UNDEFINED && matrix[pos] == matrix[index] && adj.find(pos) == adj.end())
		adjacentInernal(matrix, pos, adj);
	pos = right(index);
	if (pos != UNDEFINED && matrix[pos] == matrix[index] && adj.find(pos) == adj.end())
		adjacentInernal(matrix, pos, adj);
	pos = down(index);
	if (pos != UNDEFINED && matrix[pos] == matrix[index] && adj.find(pos) == adj.end())
		adjacentInernal(matrix, pos, adj);
}

std::map<int, int> PopStarSolver::step(int* matrix, int index) {
	std::set<int> adj = adjacent(matrix, index);
	int minCol = m_col;
	int maxCol = -1;
	int curCol = 0;
	std::set<int>::iterator iter = adj.begin();
	for (; iter != adj.end(); iter++) {
		curCol = column(*iter);
		minCol = std::min(minCol, curCol);
		maxCol = std::max(maxCol, curCol);
		matrix[*iter] = UNDEFINED;
	}
	std::map<int, int> changes;
	fall(matrix, minCol, maxCol, changes);
	columnLeft(matrix, changes);
	return changes;
}

int PopStarSolver::next(int* matrix) {
	int maxScore;
	return nextOptimalStep(matrix, 0, &maxScore);
}

bool PopStarSolver::island(int* matrix, int index) {
	int l = left(index);  int u = up(index);
	int r = right(index); int d = down(index);
	return (l == UNDEFINED || matrix[l] != matrix[index])
			&& (u == UNDEFINED || matrix[u] != matrix[index])
			&& (r == UNDEFINED || matrix[r] != matrix[index])
			&& (d == UNDEFINED || matrix[d] != matrix[index]);
}

bool PopStarSolver::solved(int* matrix) {
	for (int i = 0; i < m_size; i++) {
		if (matrix[i] != UNDEFINED && island(matrix, i) == false) {
			return false;
		}
	}
	return true;
}

int PopStarSolver::remain(int * matrix) {
	int remain = 0;
	for (int i = 0; i < m_size; i++) {
		if (matrix[i] != UNDEFINED)
			remain ++;
	}
	return remain;
}

int PopStarSolver::score(int count) {
	return count * count * 5;
}

int PopStarSolver::bonus(int* matrix) {
	return bonus(remain(matrix));
}

int PopStarSolver::bonus(int remainCount) {
	if (remainCount >= 10)
		return 0;
	return 2000 - remainCount * remainCount * 20;
}

void PopStarSolver::fall(int* matrix, int min, int max, std::map<int, int>& changes) {
	for (int i = min; i <= max; i++) {
		bubbleZero(matrix, i, changes);
	}
}

void PopStarSolver::bubbleZero(int* matrix, int col, std::map<int, int>& changes) {
	int tail = col;
	int tmp;
	for (int head = col; head < m_size; head += m_col) {
		if (matrix[head] != UNDEFINED && matrix[tail] == UNDEFINED) {
			matrix[tail] = matrix[head];
			matrix[head] = UNDEFINED;
			changes[tail] = head;
		}
		if (matrix[tail] != UNDEFINED)
			tail += m_col;
	}
}

void PopStarSolver::columnLeft(int* matrix, std::map<int, int>& changes) {
	int tail = 0;
	for (int head = 0; head < m_col; head++) {
		if (matrix[head] != UNDEFINED && matrix[tail] == UNDEFINED) {
			fillColumn(matrix, head, tail, changes);
		}
		if (matrix[tail] != UNDEFINED)
			tail++;
	}

}

void PopStarSolver::fillColumn(int* matrix, int source, int dest, std::map<int, int>& changes) {
	int sourceIndex,destIndex;
	for (int i = 0; i < m_row; i++) {
		sourceIndex = i * m_col + source;
		destIndex = i * m_col + dest;
		if (matrix[sourceIndex] != UNDEFINED) {
            matrix[destIndex] = matrix[sourceIndex];
            matrix[sourceIndex] = UNDEFINED;
            if (changes.find(sourceIndex) != changes.end()) {
            	changes[destIndex] = changes[sourceIndex];
            	changes.erase(sourceIndex);
            }
            else {
            	changes[destIndex] = sourceIndex;
            }
		}
		else
			break;
	}
}

void PopStarSolver::copy(int* matrix, int* copyedMatrix) {
	memcpy(copyedMatrix, matrix, m_size * sizeof(int));
}

int PopStarSolver::nextOptimalStep(int* matrix, int depth, int* maxScore) {
	int* copyedMatrix = new int[m_size];
	int maxStepScore = 0;
	int optimalStep = UNDEFINED;
	*maxScore = 0;
	std::set<int> processed;
	for (int index = 0; index < m_size; index ++) {
		if (matrix[index] == UNDEFINED || island(matrix, index)
				|| processed.find(index) != processed.end()) {
			continue;
		}
		copy(matrix, copyedMatrix);
		std::set<int> adj = adjacent(matrix, index);
		processed.insert(adj.begin(), adj.end());
		int score = 0;
		std::map<int, int> dstToSrc = step(copyedMatrix, index);
		score += evaluate(matrix, copyedMatrix, dstToSrc);
		if (depth < m_rDepth || remain(copyedMatrix) <= 20) {
			nextOptimalStep(copyedMatrix, depth+1, &maxStepScore);
			score += maxStepScore;
		}
		if (score > *maxScore) {
			*maxScore = score;
			optimalStep = index;
		}
		if (optimalStep == UNDEFINED)
			optimalStep = index;
	}
	return optimalStep;
}

int PopStarSolver::evaluate(int* orignalMatrix, int* popedMatrix, std::map<int, int>& dstToSrc) {
	int score = 0;
	std::map<int, int>::iterator iter = dstToSrc.begin();
	for (;iter != dstToSrc.end();iter++) {
		score -= this->score(adjacentCount(orignalMatrix, iter->second));
		score += this->score(adjacentCount(popedMatrix, iter->first));
	}
	score += bonus(popedMatrix);
	return score;
}

int PopStarSolver::evaluateIsolate(int* orignalMatrix, int* popedMatrix, std::map<int, int>& dstToSrc) {
	int isolateCnt = 0;
	int dstPos = 0;
	int srcPos = 0;
	std::map<int, int> srcToDst;
	std::map<int, int>::iterator iter = dstToSrc.begin();
	for (; iter != dstToSrc.end(); iter++) {
		srcToDst[iter->second] = iter->first;
	}
	std::set<int> processed;
	bool srcIsolate, dstIsolate;
	for (iter=dstToSrc.begin(); iter != dstToSrc.end(); iter++) {
		srcIsolate = island(orignalMatrix, iter->second);
		dstIsolate = island(popedMatrix, iter->first);
		if (dstIsolate && !srcIsolate)
			isolateCnt ++;
		else if (!dstIsolate && srcIsolate)
			isolateCnt --;
		// process dst left
		dstPos = left(iter->first);
		srcPos = srcToDst.find(dstPos) != srcToDst.end() ? dstToSrc[dstPos] : dstPos;
		if (dstPos != UNDEFINED && processed.find(dstPos) != processed.end()) {
			dstIsolate = island(popedMatrix, dstPos);
			srcIsolate = srcPos == dstPos ? dstIsolate : island(orignalMatrix, srcPos);
			if (dstIsolate && !srcIsolate)
				isolateCnt ++;
			else if (!dstIsolate && srcIsolate)
				isolateCnt --;
		}
		processed.insert(dstPos);
		processed.insert(srcPos);
		// process dst up
		dstPos = up(iter->first);
		srcPos = srcToDst.find(dstPos) != srcToDst.end() ? dstToSrc[dstPos] : dstPos;
		if (dstPos != UNDEFINED && processed.find(dstPos) != processed.end()) {
			dstIsolate = island(popedMatrix, dstPos);
			srcIsolate = srcPos == dstPos ? dstIsolate : island(orignalMatrix, srcPos);
			if (dstIsolate && !srcIsolate)
				isolateCnt ++;
			else if (!dstIsolate && srcIsolate)
				isolateCnt --;
		}
		processed.insert(dstPos);
		processed.insert(srcPos);
		// process dst right
		dstPos = right(iter->first);
		srcPos = srcToDst.find(dstPos) != srcToDst.end() ? dstToSrc[dstPos] : dstPos;
		if (dstPos != UNDEFINED && processed.find(dstPos) != processed.end()) {
			dstIsolate = island(popedMatrix, dstPos);
			srcIsolate = srcPos == dstPos ? dstIsolate : island(orignalMatrix, srcPos);
			if (dstIsolate && !srcIsolate)
				isolateCnt ++;
			else if (!dstIsolate && srcIsolate)
				isolateCnt --;
		}
		processed.insert(dstPos);
		processed.insert(srcPos);
		// process dst down
		dstPos = down(iter->first);
		srcPos = srcToDst.find(dstPos) != srcToDst.end() ? dstToSrc[dstPos] : dstPos;
		if (dstPos != UNDEFINED && processed.find(dstPos) != processed.end()) {
			dstIsolate = island(popedMatrix, dstPos);
			srcIsolate = srcPos == dstPos ? dstIsolate : island(orignalMatrix, srcPos);
			if (dstIsolate && !srcIsolate)
				isolateCnt ++;
			else if (!dstIsolate && srcIsolate)
				isolateCnt --;
		}
		processed.insert(dstPos);
		processed.insert(srcPos);
		// process src left
		srcPos = left(iter->second);
		dstPos = srcToDst.find(srcPos) != srcToDst.end() ? dstToSrc[srcPos] : srcPos;
		if (srcPos != UNDEFINED && processed.find(srcPos) != processed.end()) {
			dstIsolate = island(popedMatrix, dstPos);
			srcIsolate = srcPos == dstPos ? dstIsolate : island(orignalMatrix, srcPos);
			if (dstIsolate && !srcIsolate)
				isolateCnt ++;
			else if (!dstIsolate && srcIsolate)
				isolateCnt --;
		}
		processed.insert(dstPos);
		processed.insert(srcPos);
		// process src up
		srcPos = up(iter->second);
		dstPos = srcToDst.find(srcPos) != srcToDst.end() ? dstToSrc[srcPos] : srcPos;
		if (srcPos != UNDEFINED && processed.find(srcPos) != processed.end()) {
			dstIsolate = island(popedMatrix, dstPos);
			srcIsolate = srcPos == dstPos ? dstIsolate : island(orignalMatrix, srcPos);
			if (dstIsolate && !srcIsolate)
				isolateCnt ++;
			else if (!dstIsolate && srcIsolate)
				isolateCnt --;
		}
		processed.insert(dstPos);
		processed.insert(srcPos);
		// process src right
		srcPos = right(iter->second);
		dstPos = srcToDst.find(srcPos) != srcToDst.end() ? dstToSrc[srcPos] : srcPos;
		if (srcPos != UNDEFINED && processed.find(srcPos) != processed.end()) {
			dstIsolate = island(popedMatrix, dstPos);
			srcIsolate = srcPos == dstPos ? dstIsolate : island(orignalMatrix, srcPos);
			if (dstIsolate && !srcIsolate)
				isolateCnt ++;
			else if (!dstIsolate && srcIsolate)
				isolateCnt --;
		}
		processed.insert(dstPos);
		processed.insert(srcPos);
		// process src down
		srcPos = down(iter->second);
		dstPos = srcToDst.find(srcPos) != srcToDst.end() ? dstToSrc[srcPos] : srcPos;
		if (srcPos != UNDEFINED && processed.find(srcPos) != processed.end()) {
			dstIsolate = island(popedMatrix, dstPos);
			srcIsolate = srcPos == dstPos ? dstIsolate : island(orignalMatrix, srcPos);
			if (dstIsolate && !srcIsolate)
				isolateCnt ++;
			else if (!dstIsolate && srcIsolate)
				isolateCnt --;
		}
		processed.insert(dstPos);
		processed.insert(srcPos);
	}
	return isolateCnt;
}

int PopStarSolver::left(int index) {
	if ((index - 1 < 0) || ((int)((index-1)/m_col) != (int)(index/m_col)))
		return UNDEFINED;
	return index - 1;
}

int PopStarSolver::up(int index) {
	if (index + m_col >= m_size)
		return UNDEFINED;
	return index + m_col;
}

int PopStarSolver::right(int index) {
	if (index + 1 >= m_size || ((int)((index+1)/m_col) != (int)(index/m_col)))
		return UNDEFINED;
	return index + 1;
}

int PopStarSolver::down(int index) {
	if (index - m_col < 0)
		return UNDEFINED;
	return index - m_col;
}

int PopStarSolver::row(int index) {
	return (int)(index / m_col);
}

int PopStarSolver::column(int index) {
	return index % m_col;
}
