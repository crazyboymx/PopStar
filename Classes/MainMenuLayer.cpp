/*
 * MainMenuLayer.cpp
 *
 *  Created on: 2013-7-14
 *      Author: maxing
 */

#include "MainMenuLayer.h"
#include "GameScene.h"
#include "Config.h"

using namespace cocos2d;

MainMenuLayer::MainMenuLayer() {
}

MainMenuLayer::~MainMenuLayer() {
}

bool MainMenuLayer::init() {
	bool bRet = false;
	do {
		CC_BREAK_IF(! CCLayer::init() );

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
		int adHeight = GameScene::topAdHeight();
		m_topCenter = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height+100);
		CCSpriteFrameCache* cacher = CCSpriteFrameCache::sharedSpriteFrameCache();

		// best 165*28
		m_best = CCSprite::createWithSpriteFrameName("best.png");
		m_best->setPosition(CCPoint(origin.x + 0, origin.y + visibleSize.height - adHeight - 28));
		m_best->setAnchorPoint(CCPoint(0, 0));
		this->addChild(m_best);
		// bestbg 258*28
		m_bestbg = CCSprite::createWithSpriteFrameName("bestbg.png");
		m_bestbg->setPosition(CCPoint(origin.x + 165, origin.y + visibleSize.height - adHeight - 28));
		m_bestbg->setAnchorPoint(CCPoint(0, 0));
		this->addChild(m_bestbg);
		// bestScoreLable
		m_bestScoreLable = CCLabelBMFont::create(
				CCString::createWithFormat("%d", Config::instance()->highScore())->getCString(), "highscore.fnt");
		m_bestScoreLable->setPosition(CCPoint(origin.x + 294, origin.y + visibleSize.height - adHeight - 28));
		m_bestScoreLable->setAnchorPoint(CCPoint(0.5, 0));
		this->addChild(m_bestScoreLable);
		// sound 36*28
		m_sound = CCSprite::createWithSpriteFrameName("sound.png");
		m_sound->setPosition(CCPoint(origin.x + 433, origin.y + visibleSize.height - adHeight - 28));
		m_sound->setAnchorPoint(CCPoint(0, 0));
		m_soundFrame = m_sound->displayFrame();
		m_soundFrame->retain();
		m_muteFrame = cacher->spriteFrameByName("sound2.png");
		m_muteFrame->retain();
		if (Config::instance()->mute())
			m_sound->setDisplayFrame(m_muteFrame);
		this->addChild(m_sound);
		// title1 422*288
		m_title = CCSprite::createWithSpriteFrameName("title1.png");
		m_title->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height - adHeight - 172));
		m_title->setAnchorPoint(CCPoint(0.5f, 0.5f));
		this->addChild(m_title);
		// new game 259*59
		CCSprite* normal = CCSprite::createWithSpriteFrameName("new.png");
		CCSprite* selected = CCSprite::createWithSpriteFrameName("new.png");
		selected->setScale(1.1);
		m_newGame = CCMenuItemSprite::create(normal, selected,
				this, menu_selector(MainMenuLayer::newGameCallback));
		m_newGamePosition = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height - adHeight - 356);
		m_newGame->setPosition(m_newGamePosition);
		m_newGame->setAnchorPoint(CCPoint(0.5f, 0.5f));
		// resume 259*58
		normal = CCSprite::createWithSpriteFrameName("resume.png");
		selected = CCSprite::createWithSpriteFrameName("resume.png");
		selected->setScale(1.1);
		CCSprite* disabled = CCSprite::createWithSpriteFrameName("resume2.png");
		m_resume = CCMenuItemSprite::create(normal, selected, disabled,
				this, menu_selector(MainMenuLayer::resumeCallback));
		m_resumePosition = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height - adHeight - 442);
		m_resume->setPosition(m_resumePosition);
		m_resume->setAnchorPoint(CCPoint(0.5f, 0.5f));
		updateResumeMenu();
		// learning 259*58
		normal = CCSprite::createWithSpriteFrameName("learning.png");
		selected = CCSprite::createWithSpriteFrameName("learning.png");
		selected->setScale(1.1);
		m_learning = CCMenuItemSprite::create(normal, selected,
				this, menu_selector(MainMenuLayer::learningCallback));
		m_learningPosition = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height - adHeight - 528);
		m_learning->setPosition(m_learningPosition);
		m_learning->setAnchorPoint(CCPoint(0.5f, 0.5f));
		// rate 259*58
		normal = CCSprite::createWithSpriteFrameName("rate.png");
		selected = CCSprite::createWithSpriteFrameName("rate.png");
		selected->setScale(1.1);
		m_rate = CCMenuItemSprite::create(normal, selected,
				this, menu_selector(MainMenuLayer::rateCallback));
		m_ratePosition = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height - adHeight - 614);
		m_rate->setPosition(m_ratePosition);
		m_rate->setAnchorPoint(CCPoint(0.5f, 0.5f));

		CCArray* array = CCArray::create();
		array->addObject(m_newGame);
		array->addObject(m_resume);
		array->addObject(m_learning);
		array->addObject(m_rate);
		CCMenu* pMenu = CCMenu::createWithArray(array);
		pMenu->setPosition(0, 0);
		pMenu->setAnchorPoint(CCPoint(0, 0));
		this->addChild(pMenu);
		// light 100*100

		this->setTouchEnabled(true);
		this->setKeypadEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void MainMenuLayer::onEnter() {
	updateHighScore();
	updateResumeMenu();
	animate();
	CCLayer::onEnter();
}

void MainMenuLayer::keyBackClicked() {
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

void MainMenuLayer::updateResumeMenu() {
	m_resume->setEnabled(Config::instance()->hasSavedPuzzle());
}

void MainMenuLayer::newGameCallback(CCObject* pSender) {
	float duration = 2.0f;
	disappear(duration);
	this->scheduleOnce(schedule_selector(MainMenuLayer::startNewPuzzle), duration);
	GameScene::instance()->playEffect("click.mp3");
}

void MainMenuLayer::resumeCallback(CCObject* pSender) {
	this->setTouchEnabled(false);
	float duration = 2.0f;
	disappear(duration);
	this->scheduleOnce(schedule_selector(MainMenuLayer::resumePuzzle), duration);
	GameScene::instance()->playEffect("click.mp3");
}

void MainMenuLayer::learningCallback(CCObject* pSender) {
	float duration = 2.0f;
	disappear(duration);
	this->scheduleOnce(schedule_selector(MainMenuLayer::startLearning), duration);
	GameScene::instance()->playEffect("click.mp3");
}

void MainMenuLayer::rateCallback(CCObject* pSender) {
	GameScene::instance()->rate();
	GameScene::instance()->playEffect("click.mp3");
}

void MainMenuLayer::animate() {
	m_newGame->setPosition(m_topCenter);
	m_resume->setPosition(m_topCenter);
	m_learning->setPosition(m_topCenter);
	m_rate->setPosition(m_topCenter);
	m_best->setOpacity(0.0);
	m_bestbg->setOpacity(0.0);
	m_bestScoreLable->setOpacity(0.0);
	m_sound->setOpacity(0.0);

	CCFiniteTimeAction* delayAction = CCDelayTime::create(0.2f);
	CCFiniteTimeAction* action = CCEaseElasticOut::create(CCMoveTo::create(0.5f, m_ratePosition), 0.5f);
	m_rate->runAction(CCSequence::create(delayAction, action, NULL));

	delayAction = CCDelayTime::create(0.4f);
	action = CCEaseElasticOut::create(CCMoveTo::create(0.5f, m_learningPosition), 0.5f);
	m_learning->runAction(CCSequence::create(delayAction, action, NULL));

	delayAction = CCDelayTime::create(0.6f);
	action = CCEaseElasticOut::create(CCMoveTo::create(0.5f, m_resumePosition), 0.5f);
	m_resume->runAction(CCSequence::create(delayAction, action, NULL));

	delayAction = CCDelayTime::create(0.8f);
	action = CCEaseElasticOut::create(CCMoveTo::create(0.5f, m_newGamePosition), 0.5f);
	m_newGame->runAction(CCSequence::create(delayAction, action, NULL));

	delayAction = CCDelayTime::create(1.0f);
	action = CCFadeIn::create(2.0);
	m_best->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(1.0f);
	action = CCFadeIn::create(2.0);
	m_bestbg->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(1.0f);
	action = CCFadeIn::create(2.0);
	m_bestScoreLable->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(1.0f);
	action = CCFadeIn::create(2.0);
	m_sound->runAction(CCSequence::create(delayAction, action, NULL));
}

void MainMenuLayer::disappear(float duration) {
	this->runAction(CCSequence::create(
			CCFadeOutTRTiles::create(duration, CCSize(16, 12)),
			CCStopGrid::create(), NULL));
}

void MainMenuLayer::startNewPuzzle() {
	GameScene::instance()->startNewPuzzle();
}

void MainMenuLayer::resumePuzzle() {
	GameScene::instance()->resumePuzzle();
}

void MainMenuLayer::startLearning() {
	GameScene::instance()->startLearning();
}

void MainMenuLayer::updateHighScore() {
	m_bestScoreLable->setCString(CCString::createWithFormat("%d",
			Config::instance()->highScore())->getCString());
}

void MainMenuLayer::ccTouchesEnded(CCSet* touches, CCEvent* event) {
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

	if (!m_sound->boundingBox().containsPoint(location))
		return;
	bool mute = !Config::instance()->mute();
	Config::instance()->setMute(mute);
	if (mute)
		m_sound->setDisplayFrame(m_muteFrame);
	else
		m_sound->setDisplayFrame(m_soundFrame);
}

void MainMenuLayer::registerWithTouchDispatcher() {
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
