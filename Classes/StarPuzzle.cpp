/*
 * StarPuzzle.cpp
 *
 *  Created on: 2013-7-13
 *      Author: maxing
 */

#include "StarPuzzle.h"
#include "PopStarSolver.h"
#include "GameScene.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>

StarPuzzle::StarPuzzle(int row, int col)
    : m_row(row)
    , m_col(col)
    , m_size(row * col)
    , m_score(0)
    , m_step(0)
{
	for (int i = 0; i < m_size; i++) {
		m_stars[i] = 0;
	}
	m_solver = new PopStarSolver(row, col);
	m_emitterArray = new CCArray;
}

StarPuzzle::~StarPuzzle() {
	clearPuzzle();
	delete m_solver;
	m_emitterArray->release();
}

bool StarPuzzle::attach(CCNode* node) {
	if (node == NULL)
		return false;
	bool succ = true;
	for (int i = 0; i < m_size; i++) {
		if (m_stars[i] != 0) {
			succ &= m_stars[i]->attach(node);
		}
	}
	return succ;
}

HitResult StarPuzzle::hitTest(const CCPoint& point) {
	int row = std::max(0, (int)((point.y - m_bottom) / STAR_HEIGHT));
	int col = std::max(0, (int)((point.x - m_left) / STAR_WIDTH));
	int index = row * m_col + col;
	CCLog("point %0.2f, %0.2f, row %d, col %d", point.x, point.y, row, col);
	HitResult result;
	if (index < m_size && m_stars[index] != 0) {
		if (m_selectedStarIndex.find(index) != m_selectedStarIndex.end()) {
			m_changes = m_solver->step(m_matrix, index);
			result.type = HitResult::HIT_POPSTAR;
			result.data = m_selectedStarIndex.size();
			result.location = m_stars[index]->getPosition();
		}
		else {
			std::set<int> adjacent = m_solver->adjacent(m_matrix, index);
		    doUnSelection();
		    if (adjacent.size() <= 1) {
		    	result.type = HitResult::HIT_NOTHING;
				result.data = 0;
		    }
		    else {
				doSelection(adjacent);
				result.type = HitResult::HIT_STAR;
				result.data = m_selectedStarIndex.size();
		    }
		}
	}
	else {
		doUnSelection();
		result.type = HitResult::HIT_NOTHING;
		result.data = 0;
	}
	return result;
}

HitResult StarPuzzle::hitTestAuto() {
	int index;
	if (m_selectedStarIndex.size() <= 0)
		index = m_solver->next(m_matrix);
	else
		index = *m_selectedStarIndex.begin();
	HitResult result;
	if (index < m_size && m_stars[index] != 0) {
		if (m_selectedStarIndex.find(index) != m_selectedStarIndex.end()) {
			m_changes = m_solver->step(m_matrix, index);
			result.type = HitResult::HIT_POPSTAR;
			result.data = m_selectedStarIndex.size();
			result.location = m_stars[index]->getPosition();
		}
		else {
			std::set<int> adjacent = m_solver->adjacent(m_matrix, index);
		    doUnSelection();
		    if (adjacent.size() <= 1) {
		    	m_selectedStarIndex.clear();
		    	result.type = HitResult::HIT_NOTHING;
				result.data = 0;
		    }
		    else {
				doSelection(adjacent);
				result.type = HitResult::HIT_STAR;
				result.data = m_selectedStarIndex.size();
		    }
		}
	}
	else {
		result.type = HitResult::HIT_NOTHING;
		result.data = 0;
	}
	return result;
}

HitResult StarPuzzle::autoSelect() {
	int index = m_solver->next(m_matrix);
	HitResult result;
	std::set<int> adjacent = m_solver->adjacent(m_matrix, index);
	doUnSelection();
	if (adjacent.size() <= 1) {
		m_selectedStarIndex.clear();
		result.type = HitResult::HIT_NOTHING;
		result.data = 0;
	}
	else {
		doSelection(adjacent);
		result.type = HitResult::HIT_STAR;
		result.data = m_selectedStarIndex.size();
	}
}

float StarPuzzle::puzzleStartAnimation() {
	float delayTime = 0.0f;
	float delayFactor = 0.08f;
	float baseActionTime = 0.8f;
	float actionTime = 0.0f;
	CCFiniteTimeAction *delay, *action, *seq;
	for (int i = 0; i < m_size; i++) {
		if (m_stars[i] != 0) {
			int r = row(i);
			int c = column(i);
			m_stars[i]->setPosition(m_left + c * STAR_WIDTH, m_top);
			delayTime = r * delayFactor * 2 + delayFactor * (i % 2);
			delay = CCDelayTime::create(delayTime);
			actionTime = (1 - r * STAR_HEIGHT / (m_top - m_bottom)) * baseActionTime;
			action = CCMoveTo::create(actionTime, CCPoint(m_left + c * STAR_WIDTH, m_bottom + r * STAR_HEIGHT));
			seq = CCSequence::create(delay, action, NULL);
			m_stars[i]->runAction(seq);
		}
	}
	return delayTime+0.2f;
}

void StarPuzzle::popStar() {
    std::set<int>::iterator iter = m_selectedStarIndex.begin();
    for (; iter != m_selectedStarIndex.end(); iter++) {
    	explodeStar(*iter);
    }
    GameScene::instance()->playEffect("broken.mp3");
    m_selectedStarIndex.clear();
}

void StarPuzzle::removeAndPopTailStar() {
	for (int i = m_size-1; i >= 0; i --) {
		if (m_stars[i] != NULL) {
			m_matrix[i] = UNDEFINED;
			explodeStar(i);
			GameScene::instance()->playEffect("broken.mp3");
			return;
		}
	}
}

void StarPuzzle::removeAndPopAllRemainStars() {
	for (int i = 0; i < m_size; i++) {
		if (m_stars[i] != NULL) {
			m_matrix[i] = UNDEFINED;
			explodeStar(i);
		}
	}
	GameScene::instance()->playEffect("broken.mp3");
}

void StarPuzzle::applyChanges() {
    std::map<int, int>::iterator iter = m_changes.begin();
    int newPos;
    for (; iter != m_changes.end(); iter++) {
    	newPos = iter->first;
        m_stars[newPos] = m_stars[iter->second];
        m_stars[iter->second] = NULL;
        m_stars[newPos]->moveTo(m_left + column(newPos) * STAR_WIDTH, m_bottom + row(newPos) * STAR_HEIGHT);
    }
    m_changes.clear();
}

bool StarPuzzle::isSolved() {
	return m_solver->solved(m_matrix);
}

int StarPuzzle::remainStars() {
	return m_solver->remain(m_matrix);
}

int StarPuzzle::bonus() {
	return m_solver->bonus(m_matrix);
}

int StarPuzzle::bonus(int remainCount) {
	return m_solver->bonus(remainCount);
}

int StarPuzzle::score(int count) {
	return m_solver->score(count);
}

int StarPuzzle::evaluateMaxScore() {
	int* copyedMatrix = new int[m_size];
	memcpy(copyedMatrix, m_matrix, m_size * sizeof(int));
	int score = 0;
	int index = UNDEFINED;
	while (!m_solver->solved(copyedMatrix)) {
		index = m_solver->next(copyedMatrix);
		score += m_solver->score(m_solver->adjacentCount(copyedMatrix, index));
		m_solver->step(copyedMatrix, index);
	}
	score += m_solver->bonus(copyedMatrix);
	delete[] copyedMatrix;
	return score;
}

void StarPuzzle::initPuzzle(PuzzleLevel level) {
	for (int i = 0; i < 10; i++) {
		clearPuzzle();
		generatePuzzle(level);
		if (evaluateMaxScore() >= 2000)
			break;
	}
	createStars();
	hideAllStars();
}

void StarPuzzle::clearPuzzle() {
	for (int i = 0; i < m_size; i++) {
		if (m_stars[i] != NULL) {
			delete m_stars[i];
			m_stars[i] = NULL;
		}
	}
	CCObject* it = NULL;
	CCARRAY_FOREACH(m_emitterArray, it) {
		CCParticleSystem *emitter = dynamic_cast<CCParticleSystem*>(it);
		emitter->removeFromParent();
	}
	m_emitterArray->removeAllObjects();
	m_score = 0;
	m_step = 0;
}

std::string StarPuzzle::serialization() {
	std::string s;
	for (int i = 0; i < m_size; i++) {
		if (m_stars[i] == 0)
			s += '0';
		else
			s += '0'+m_stars[i]->type();
	}
	return s;
}

void StarPuzzle::loadPuzzle(std::string s) {
	assert(s.lenght() == m_size);
	clearPuzzle();
	for (int i = 0; i < m_size; i++) {
		if (s[i] == '0')
			m_matrix[i] = UNDEFINED;
		else
			m_matrix[i] = s[i] - '0';
	}
	createStars();
	hideAllStars();
}

void StarPuzzle::hideAllStars() {
	for (int i = 0; i < m_size; i++) {;
		if (m_stars[i] != 0) {
			int r = row(i);
			int c = column(i);
			m_stars[i]->setPosition(m_left + c * STAR_WIDTH, m_top);
		}
	}
}

void StarPuzzle::resetStarsPosition() {
	for (int i = 0; i < m_size; i++) {
		if (m_stars[i] != 0) {
			int r = row(i);
			int c = column(i);
			m_stars[i]->setPosition(m_left + c * STAR_WIDTH, m_bottom + r * STAR_HEIGHT);
		}
	}
}

void StarPuzzle::doSelection(std::set<int>& selectList) {
	if (selectList.size() <= 1)
		return;
	m_selectedStarIndex = selectList;
	std::set<int>::iterator iter = m_selectedStarIndex.begin();
	for (; iter != m_selectedStarIndex.end(); iter++) {
		if (m_stars[*iter] != 0)
			m_stars[*iter]->select();
	}
}

void StarPuzzle::doUnSelection() {
	std::set<int>::iterator iter = m_selectedStarIndex.begin();
	for (; iter != m_selectedStarIndex.end(); iter++) {
		if (m_stars[*iter] != 0)
			m_stars[*iter]->unselect();
	}
	m_selectedStarIndex.clear();
}

void StarPuzzle::generatePuzzle(PuzzleLevel level) {
	switch (level) {
		case EASY:   generateEasyPuzzle();     break;
		case NORMAL: generateNormalPuzzle();   break;
		case HARD:   generateHardPuzzle();     break;
		case EXPERT: generateExpertPuzzle();   break;
		default:     generateNormalPuzzle();   break;
	}
}

void StarPuzzle::generateEasyPuzzle() {
}

void StarPuzzle::generateNormalPuzzle() {
	for (int i = 0; i < m_size; i++) {
		m_matrix[i] = rand() % 5 + 1;
	}
}

void StarPuzzle::generateHardPuzzle() {
}

void StarPuzzle::generateExpertPuzzle() {
}

void StarPuzzle::createStars() {
	for (int i = 0; i < m_size; i++) {
		if (m_matrix[i] != UNDEFINED)
			m_stars[i] = new Star(Star::getType(m_matrix[i]));
		else
			m_stars[i] = NULL;
	}
}

void StarPuzzle::explodeStar(int index) {
	m_emitterArray->addObject(m_stars[index]->explode());
	delete m_stars[index];
	m_stars[index] = NULL;
}

int StarPuzzle::row(int index) {
	return (int)(index / m_col);
}

int StarPuzzle::column(int index) {
	return index % m_col;
}

void StarPuzzle::dumpStatus() {
	std::set<int>::iterator iter = m_selectedStarIndex.begin();
	std::string s;
	char buff[16];
	for (; iter != m_selectedStarIndex.end(); iter++) {
		int len = sprintf(buff, "%d, ", *iter);
		s.append(buff, len);
	}
	CCLog("selected: %s", s.c_str());
	s = "";
	std::map<int, int>::iterator it = m_changes.begin();
	for (; it != m_changes.end(); it++) {
        int len = sprintf(buff, "%d->%d, ", it->first, it->second);
        s.append(buff, len);
    }
	CCLog("changes: %s", s.c_str());
}
