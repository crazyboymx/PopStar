/*
 * MainMenuLayer.h
 *
 *  Created on: 2013-7-14
 *      Author: maxing
 */

#ifndef MAINMENULAYER_H_
#define MAINMENULAYER_H_

#include <Config.h>
#include <cocos2d.h>

class MainMenuLayer: public cocos2d::CCLayer {
public:
	MainMenuLayer();
	virtual ~MainMenuLayer();
	CREATE_FUNC(MainMenuLayer);
	virtual bool init();
	virtual void onEnter();
	virtual void keyBackClicked();
	void updateResumeMenu();

	virtual void newGameCallback(cocos2d::CCObject* pSender);
	virtual void challengeGameCallback(cocos2d::CCObject* pSender);
	virtual void resumeCallback(cocos2d::CCObject* pSender);
	virtual void learningCallback(cocos2d::CCObject* pSender);
	virtual void rateCallback(cocos2d::CCObject* pSender);

	void animate();
	void disappear(float duration);
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	void registerWithTouchDispatcher();
private:
	Config::GameMode m_selectMode;
	void startNewPuzzle();
	void resumePuzzle();
	void startLearning();
	void updateHighScore();

	cocos2d::CCLabelBMFont* m_bestLabel;
	cocos2d::CCLabelBMFont* m_bestScoreLable;
	cocos2d::CCSprite* m_sound;
	cocos2d::CCLabelBMFont* m_title1;
	cocos2d::CCLabelBMFont* m_title2;
	cocos2d::CCMenuItemSprite* m_newGame;
	cocos2d::CCMenuItemSprite* m_challengeGame;
	cocos2d::CCMenuItemSprite* m_resume;
	cocos2d::CCMenuItemSprite* m_learning;
	cocos2d::CCMenuItemSprite* m_rate;

	cocos2d::CCSprite* m_star1;
	cocos2d::CCSprite* m_star2;
	cocos2d::CCSprite* m_star3;
	cocos2d::CCSpriteFrame* m_muteFrame;
	cocos2d::CCSpriteFrame* m_soundFrame;
	cocos2d::CCSpriteFrame* m_resumeFrame;

	cocos2d::CCPoint m_topCenter;
	cocos2d::CCPoint m_newGamePosition;
	cocos2d::CCPoint m_challengePosition;
	cocos2d::CCPoint m_resumePosition;
	cocos2d::CCPoint m_learningPosition;
	cocos2d::CCPoint m_ratePosition;
};

#endif /* MAINMENULAYER_H_ */
