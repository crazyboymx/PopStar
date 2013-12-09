/*
 * PopStarSolver.h
 *
 *  Created on: 2013-7-10
 *      Author: maxing
 */

#ifndef POPSTARSOLVER_H_
#define POPSTARSOLVER_H_

typedef int Star_t;

#include <set>
#include <map>

#define UNDEFINED -1

class PopStarSolver {
public:
	PopStarSolver(int row, int col);
	virtual ~PopStarSolver();
	void resize(int row, int col);
	void updateRow(int row);
	void updateColumn(int col);

	std::set<int> adjacent(int* matrix, int index);
	int adjacentCount(int* matrix, int index);
	std::map<int, int> step(int* matrix, int index);
	int next(int* matrix);
	bool island(int* matrix, int index);
	bool solved(int* matrix);
	int remain(int* matrix);
	int score(int count);
	int bonus(int* matrix);
	int bonus(int remainCount);
private:
	void adjacentInernal(int* matrix, int index, std::set<int>& adj);
	void fall(int* matrix, int min, int max, std::map<int, int>& changes);
	void bubbleZero(int* matrix, int col, std::map<int, int>& changes);
	void columnLeft(int* matrix, std::map<int, int>& changes);
	void fillColumn(int* matrix, int source, int dest, std::map<int, int>& changes);
	void copy(int* matrix, int* copyedMatrix);
	int nextOptimalStep(int* matrix, int depth, int* maxScore);
	int evaluate(int* orignalMatrix, int* popedMatrix, std::map<int, int>& dstToSrc);
	int evaluateIsolate(int* orignalMatrix, int* popedMatrix, std::map<int, int>& dstToSrc);
	int left(int index);
	int up(int index);
	int right(int index);
	int down(int index);
	int row(int index);
	int column(int index);

private:
	int m_row;
	int m_col;
	int m_size;

	int m_rDepth;
};

#endif /* POPSTARSOLVER_H_ */
