/*
 * ResumeAndSaveMenuLayer.cpp
 *
 *  Created on: 2013-8-26
 *      Author: maxing
 */

#include "ResumeAndSaveMenuLayer.h"
#include "GameScene.h"

using namespace cocos2d;

ResumeAndSaveMenuLayer::ResumeAndSaveMenuLayer() {
}

ResumeAndSaveMenuLayer::~ResumeAndSaveMenuLayer() {
}

bool ResumeAndSaveMenuLayer::init() {
	bool bRet = false;
	do {
		CC_BREAK_IF(! CCLayer::init() );

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
		int adHeight = GameScene::topAdHeight();
		m_topCenter = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height+100);
		CCSpriteFrameCache* cacher = CCSpriteFrameCache::sharedSpriteFrameCache();

		// resume 259*58
		CCSprite* normal = CCSprite::createWithSpriteFrameName("continue.png");
		CCSprite* selected = CCSprite::createWithSpriteFrameName("continue.png");
		selected->setScale(1.1);
		m_resumeSprite = CCMenuItemSprite::create(normal, selected,
				this, menu_selector(ResumeAndSaveMenuLayer::resumeCallback));
		m_resumePosition = CCPoint(origin.x + visibleSize.width/2, origin.y + 451);
		m_resumeSprite->setPosition(m_resumePosition);
		m_resumeSprite->setAnchorPoint(CCPoint(0.5f, 0.5f));
		// save 259*58
		normal = CCSprite::createWithSpriteFrameName("pause.png");
		selected = CCSprite::createWithSpriteFrameName("pause.png");
		selected->setScale(1.1);
		m_saveAndExitSprite = CCMenuItemSprite::create(normal, selected,
				this, menu_selector(ResumeAndSaveMenuLayer::saveAndExitCallback));
		m_saveAndExitPosition = CCPoint(origin.x + visibleSize.width/2, origin.y + 336);
		m_saveAndExitSprite->setPosition(m_saveAndExitPosition);
		m_saveAndExitSprite->setAnchorPoint(CCPoint(0.5f, 0.5f));

		CCArray* array = CCArray::create();
		array->addObject(m_resumeSprite);
		array->addObject(m_saveAndExitSprite);
		CCMenu* pMenu = CCMenu::createWithArray(array);
		pMenu->setPosition(0, 0);
		pMenu->setAnchorPoint(CCPoint(0, 0));
		this->addChild(pMenu);

		bRet = true;
	} while (0);

	return bRet;
}

void ResumeAndSaveMenuLayer::onEnter() {
	animate();
	CCLayer::onEnter();
}

void ResumeAndSaveMenuLayer::resumeCallback(CCObject* pSender) {
	float duration = 0.2f;
	disappear(duration);
	this->scheduleOnce(schedule_selector(ResumeAndSaveMenuLayer::resumePuzzle), duration);
	GameScene::instance()->playEffect("clickbutton.ogg");
}

void ResumeAndSaveMenuLayer::saveAndExitCallback(CCObject* pSender) {
	GameScene::instance()->saveAndExit();
	GameScene::instance()->playEffect("clickbutton.ogg");
}

void ResumeAndSaveMenuLayer::animate() {
	m_resumeSprite->setPosition(m_topCenter);
	m_saveAndExitSprite->setPosition(m_topCenter);

	m_resumeSprite->runAction(CCEaseElasticOut::create(CCMoveTo::create(0.5f, m_resumePosition), 0.5f));
	m_saveAndExitSprite->runAction(CCEaseElasticOut::create(CCMoveTo::create(0.5f, m_saveAndExitPosition), 0.5f));
}

void ResumeAndSaveMenuLayer::disappear(float duration) {
	m_resumeSprite->runAction(CCEaseElasticOut::create(CCMoveTo::create(0.5f, m_topCenter), 0.1f));
	m_saveAndExitSprite->runAction(CCEaseElasticOut::create(CCMoveTo::create(0.5f, m_topCenter), 0.1f));
}

void ResumeAndSaveMenuLayer::resumePuzzle() {
	GameScene::instance()->returnToPuzzle();
}

void ResumeAndSaveMenuLayer::registerWithTouchDispatcher() {
    //CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}
