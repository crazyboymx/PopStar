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

		// best
		m_bestLabel = CCLabelBMFont::create("最高分", "zh.fnt");
		m_bestLabel->setPosition(CCPoint(origin.x+340, origin.y + 1000));
		m_bestLabel->setAnchorPoint(CCPoint(0.5, 0));
		this->addChild(m_bestLabel);
		// bestScoreLable
		m_bestScoreLable = CCLabelBMFont::create(
				CCString::createWithFormat("%d", Config::instance()->highScore())->getCString(), "zh.fnt");
		m_bestScoreLable->setPosition(CCPoint(origin.x+340, origin.y + 920));
		m_bestScoreLable->setAnchorPoint(CCPoint(0.5, 0));
		this->addChild(m_bestScoreLable);
		// sound 128*128
		m_sound = CCSprite::createWithSpriteFrameName("sound.png");
		m_sound->setPosition(CCPoint(origin.x + visibleSize.width-60, origin.y + visibleSize.height - adHeight - 80));
		m_sound->setAnchorPoint(CCPoint(0.5, 0.5));
		m_sound->setScale(0.7);
		m_soundFrame = m_sound->displayFrame();
		m_soundFrame->retain();
		m_muteFrame = cacher->spriteFrameByName("mute.png");
		m_muteFrame->retain();
		if (Config::instance()->mute())
			m_sound->setDisplayFrame(m_muteFrame);
		this->addChild(m_sound);
		// title1
		m_title1 = CCLabelBMFont::create("消灭星星", "zh.fnt");
		m_title1->setPosition(CCPoint(origin.x + 40, origin.y + 740));
		m_title1->setAnchorPoint(CCPoint(0.0f, 0.0f));
		m_title1->setScale(1.5625);
		this->addChild(m_title1);
		// title2
		m_title2 = CCLabelBMFont::create("高分挑战", "zh.fnt");
		m_title2->setPosition(CCPoint(origin.x + 280, origin.y + 630));
		m_title2->setAnchorPoint(CCPoint(0.0f, 0.0f));
		m_title2->setScale(1.5625);
		this->addChild(m_title2);
		// new game 450*72
		CCSprite* normal = CCSprite::createWithSpriteFrameName("start.png");
		normal->setAnchorPoint(CCPoint(0.5f, 0.5f));
		CCSprite* selected = CCSprite::createWithSpriteFrameName("start.png");
		selected->setAnchorPoint(CCPoint(0.5f, 0.5f));
		selected->setScale(1.1);
		m_newGame = CCMenuItemSprite::create(normal, selected,
				this, menu_selector(MainMenuLayer::newGameCallback));
		m_newGamePosition = CCPoint(origin.x + visibleSize.width/2, origin.y + 480);
		m_newGame->setPosition(m_newGamePosition);
		m_newGame->setAnchorPoint(CCPoint(0.5f, 0.5f));
		// challenge 450*72
		normal = CCSprite::createWithSpriteFrameName("challenge.png");
		normal->setAnchorPoint(CCPoint(0.5f, 0.5f));
		selected = CCSprite::createWithSpriteFrameName("challenge.png");
		selected->setAnchorPoint(CCPoint(0.5f, 0.5f));
		selected->setScale(1.1);
		m_challengeGame = CCMenuItemSprite::create(normal, selected,
				this, menu_selector(MainMenuLayer::challengeGameCallback));
		m_challengePosition = CCPoint(origin.x + visibleSize.width/2, origin.y + 380);
		m_challengeGame->setPosition(m_challengePosition);
		m_challengeGame->setAnchorPoint(CCPoint(0.5f, 0.5f));
		// resume 450*72
		normal = CCSprite::createWithSpriteFrameName("continue.png");
		normal->setAnchorPoint(CCPoint(0.5f, 0.5f));
		selected = CCSprite::createWithSpriteFrameName("continue.png");
		selected->setAnchorPoint(CCPoint(0.5f, 0.5f));
		selected->setScale(1.1);
		CCSprite* disabled = CCSprite::createWithSpriteFrameName("continue_gray.png");
		m_resume = CCMenuItemSprite::create(normal, selected, disabled,
				this, menu_selector(MainMenuLayer::resumeCallback));
		m_resumePosition = CCPoint(origin.x + visibleSize.width/2, origin.y + 580);
		m_resume->setPosition(m_resumePosition);
		m_resume->setAnchorPoint(CCPoint(0.5f, 0.5f));
		updateResumeMenu();
		// learning 450*72
		normal = CCSprite::createWithSpriteFrameName("highscore.png");
		normal->setAnchorPoint(CCPoint(0.5f, 0.5f));
		selected = CCSprite::createWithSpriteFrameName("highscore.png");
		selected->setAnchorPoint(CCPoint(0.5f, 0.5f));
		selected->setScale(1.1);
		m_learning = CCMenuItemSprite::create(normal, selected,
				this, menu_selector(MainMenuLayer::learningCallback));
		m_learningPosition = CCPoint(origin.x + visibleSize.width/2, origin.y + 280);
		m_learning->setPosition(m_learningPosition);
		m_learning->setAnchorPoint(CCPoint(0.5f, 0.5f));
		// rate 450*72
		normal = CCSprite::createWithSpriteFrameName("rate.png");
		normal->setAnchorPoint(CCPoint(0.5f, 0.5f));
		selected = CCSprite::createWithSpriteFrameName("rate.png");
		selected->setAnchorPoint(CCPoint(0.5f, 0.5f));
		selected->setScale(1.1);
		m_rate = CCMenuItemSprite::create(normal, selected,
				this, menu_selector(MainMenuLayer::rateCallback));
		m_ratePosition = CCPoint(origin.x + visibleSize.width/2, origin.y + 180);
		m_rate->setPosition(m_ratePosition);
		m_rate->setAnchorPoint(CCPoint(0.5f, 0.5f));

		CCArray* array = CCArray::create();
		array->addObject(m_newGame);
		array->addObject(m_challengeGame);
		array->addObject(m_resume);
		array->addObject(m_learning);
		array->addObject(m_rate);
		CCMenu* pMenu = CCMenu::createWithArray(array);
		pMenu->setPosition(0, 0);
		pMenu->setAnchorPoint(CCPoint(0, 0));
		this->addChild(pMenu);
		// star 128*128
		m_star1 = CCSprite::createWithSpriteFrameName("star.png");
		m_star1->setAnchorPoint(CCPoint(0.5f, 0.5f));
		m_star1->setPosition(CCPoint(origin.x + 160, origin.y + 900));
		m_star1->setRotation(-20);
		this->addChild(m_star1);
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
	m_selectMode = Config::Normal;
	this->scheduleOnce(schedule_selector(MainMenuLayer::startNewPuzzle), duration);
	GameScene::instance()->playEffect("clickbutton.ogg");
}

void MainMenuLayer::challengeGameCallback(CCObject* pSender) {
	float duration = 2.0f;
	disappear(duration);
	m_selectMode = Config::Challenge;
	this->scheduleOnce(schedule_selector(MainMenuLayer::startNewPuzzle), duration);
	GameScene::instance()->playEffect("clickbutton.ogg");
}

void MainMenuLayer::resumeCallback(CCObject* pSender) {
	this->setTouchEnabled(false);
	float duration = 2.0f;
	disappear(duration);
	this->scheduleOnce(schedule_selector(MainMenuLayer::resumePuzzle), duration);
	GameScene::instance()->playEffect("clickbutton.ogg");
}

void MainMenuLayer::learningCallback(CCObject* pSender) {
	float duration = 2.0f;
	disappear(duration);
	this->scheduleOnce(schedule_selector(MainMenuLayer::startLearning), duration);
	GameScene::instance()->playEffect("clickbutton.ogg");
}

void MainMenuLayer::rateCallback(CCObject* pSender) {
	GameScene::instance()->rate();
	GameScene::instance()->playEffect("clickbutton.ogg");
}

void MainMenuLayer::animate() {
	/*m_newGame->setPosition(m_topCenter);
	m_resume->setPosition(m_topCenter);
	m_learning->setPosition(m_topCenter);
	m_rate->setPosition(m_topCenter);
	m_bestLabel->setOpacity(0.0);
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
	m_newGame->runAction(CCSequence::create(delayAction, action, NULL));*/
}

void MainMenuLayer::disappear(float duration) {
	this->runAction(CCSequence::create(
			CCFadeOutTRTiles::create(duration, CCSize(16, 12)),
			CCStopGrid::create(), NULL));
}

void MainMenuLayer::startNewPuzzle() {
	GameScene::instance()->startNewPuzzle(m_selectMode);
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
