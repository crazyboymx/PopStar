/*
 * Config.cpp
 *
 *  Created on: 2013-7-25
 *      Author: maxing
 */

#include "Config.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include <map>

const char* CONFIG_KEY_MUTE				= "mute";
const char* CONFIG_KEY_HASSAVEDPUZZLE	= "hasSavedPuzzle";
const char* CONFIG_KEY_SAVEDPUZZLEMODE	= "savedPuzzleMode";
const char* CONFIG_KEY_HIGHSCORE		= "highScore";
const char* CONFIG_KEY_HIGHLEVEL		= "highLevel";
const char* CONFIG_KEY_SAVEDSCORE		= "savedScore";
const char* CONFIG_KEY_SAVEDTARGET		= "savedTarget";
const char* CONFIG_KEY_SAVEDLEVEL		= "savedLevel";
const char* CONFIG_KEY_SAVEDROW			= "savedRow";
const char* CONFIG_KEY_SAVEDCOL			= "savedCol";
const char* CONFIG_KEY_SAVEDPUZZLE		= "savedPuzzle";

using namespace cocos2d;

Config* Config::m_instance = NULL;

Config::Config()
: m_mute(false)
, m_hasSavedPuzzle(false)
, m_savedPuzzleMode(Normal)
, m_highScore(0)
, m_highLevel(0)
, m_savedScore(0)
, m_savedTarget(0)
, m_savedLevel(0)
, m_savedRow(0)
, m_savedCol(0){
}

Config::~Config() {
}

Config* Config::instance() {
	if (m_instance == NULL)
		m_instance = new Config;
	return m_instance;
}

void Config::load() {
	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	m_mute = ud->getBoolForKey(CONFIG_KEY_MUTE);
	m_hasSavedPuzzle = ud->getBoolForKey(CONFIG_KEY_HASSAVEDPUZZLE);
	m_highScore = ud->getIntegerForKey(CONFIG_KEY_HIGHSCORE);
	m_highLevel = ud->getIntegerForKey(CONFIG_KEY_HIGHLEVEL);
	if (m_hasSavedPuzzle) {
		m_savedScore = ud->getIntegerForKey(CONFIG_KEY_SAVEDSCORE);
		m_savedTarget = ud->getIntegerForKey(CONFIG_KEY_SAVEDTARGET);
		m_savedLevel = ud->getIntegerForKey(CONFIG_KEY_SAVEDLEVEL);
		m_savedRow = ud->getIntegerForKey(CONFIG_KEY_SAVEDROW);
		m_savedCol = ud->getIntegerForKey(CONFIG_KEY_SAVEDCOL);
		m_savedPuzzle = ud->getStringForKey(CONFIG_KEY_SAVEDPUZZLE);
	}
}

void Config::flush() {
	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	ud->setBoolForKey(CONFIG_KEY_MUTE, m_mute);
	ud->setBoolForKey(CONFIG_KEY_HASSAVEDPUZZLE, m_hasSavedPuzzle);
	ud->setIntegerForKey(CONFIG_KEY_HIGHSCORE, m_highScore);
	ud->setIntegerForKey(CONFIG_KEY_HIGHLEVEL, m_highLevel);
	ud->setIntegerForKey(CONFIG_KEY_SAVEDSCORE, m_savedScore);
	ud->setIntegerForKey(CONFIG_KEY_SAVEDTARGET, m_savedTarget);
	ud->setIntegerForKey(CONFIG_KEY_SAVEDLEVEL, m_savedLevel);
	ud->setIntegerForKey(CONFIG_KEY_SAVEDROW, m_savedRow);
	ud->setIntegerForKey(CONFIG_KEY_SAVEDCOL, m_savedCol);
	ud->setStringForKey(CONFIG_KEY_SAVEDPUZZLE, m_savedPuzzle);
	ud->flush();
}

void Config::setMute(bool mute) {
	m_mute = mute;
	CCUserDefault* ud = CCUserDefault::sharedUserDefault();
	ud->setBoolForKey(CONFIG_KEY_MUTE, m_mute);
	if (m_mute)
		CocosDenshion::SimpleAudioEngine::sharedEngine()->stopAllEffects();
}

void Config::clearSavedPuzzle() {
	m_hasSavedPuzzle = false;
	m_savedRow = 0;
	m_savedCol = 0;
	m_savedLevel = 0;
	m_savedScore = 0;
	m_savedTarget = 0;
	m_savedPuzzle = "";
}
void Config::savePuzzle(std::string puzzle, int row, int col, int level, int score, int target) {
	m_savedRow = row;
	m_savedCol = col;
	m_savedLevel = level;
	m_savedScore = score;
	m_savedTarget = target;
	m_savedPuzzle = puzzle;
	m_hasSavedPuzzle = true;
	m_savedPuzzleMode = Normal;
}
