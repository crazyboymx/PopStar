/*
 * StarPuzzle.h
 *
 *  Created on: 2013-7-13
 *      Author: maxing
 */

#ifndef STARPUZZLE_H_
#define STARPUZZLE_H_

#include "Star.h"
#include "cocos2d.h"
#include <string>
#include <set>

using namespace cocos2d;

#define MAX_PUZZLE_ROW  10
#define MAX_PUZZLE_COL  10

class PopStarSolver;

class HitResult {
public:
	enum HitResultType {
		HIT_NOTHING    = 0,
		HIT_STAR       = 1,
		HIT_POPSTAR    = 2,
	};
	HitResultType type;
	int data;
	CCPoint location;
	HitResult() : type(HIT_NOTHING), data(0) {};
};

class StarPuzzle {
public:
	enum PuzzleLevel {
		EASY        = 1,
		NORMAL      = 2,
		HARD        = 3,
		EXPERT      = 4
	};
	StarPuzzle(int row = MAX_PUZZLE_ROW, int col = MAX_PUZZLE_COL);
	virtual ~StarPuzzle();

	bool attach(CCNode* node);
	HitResult hitTest(const CCPoint& point);
	HitResult hitTestAuto();
	HitResult autoSelect();
	// 开始动画，返回动画总时间
	float puzzleStartAnimation();
	void popStar();
	void removeAndPopTailStar();
	void removeAndPopAllRemainStars();
	void applyChanges();
	bool isSolved();
	int remainStars();
	int bonus();
	int bonus(int remainCount);
	int score(int count);
	int evaluateMaxScore(); // 评估m_matrix中所能达到的最大分值

	void initPuzzle(PuzzleLevel level = NORMAL);
	void clearPuzzle();
	std::string serialization();
	void loadPuzzle(std::string s);

	void setTop(float top) { m_top = top; }
	void setLeft(float left) { m_left = left; }
	void setBottom(float bottom) { m_bottom = bottom; }
	void setRight(float right) { m_right = right; }
private:
	void hideAllStars();
	void resetStarsPosition();
	void doSelection(std::set<int>& selectList);
	void doUnSelection();
	void generatePuzzle(PuzzleLevel level);
	void generateEasyPuzzle();
	void generateNormalPuzzle();
	void generateHardPuzzle();
	void generateExpertPuzzle();
	void createStars();
	void explodeStar(int index);

	void dumpStatus();

	int row(int index);
	int column(int index);

	int m_row;
	int m_col;
	int m_size;
	Star* m_stars[MAX_PUZZLE_ROW * MAX_PUZZLE_COL];
	int m_matrix[MAX_PUZZLE_ROW * MAX_PUZZLE_COL];

	PopStarSolver* m_solver;
	int m_score;
	int m_step;
	std::set<int> m_selectedStarIndex;
	// 记录变更目标的来源,map<targetIndex, sourceIndex>
	std::map<int, int> m_changes;
	CCArray *m_emitterArray;
	float m_top;
	float m_left;
	float m_bottom;
	float m_right;
};

#endif /* STARPUZZLE_H_ */
