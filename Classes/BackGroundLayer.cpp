/*
 * BackGroundLayer.cpp
 *
 *  Created on: 2013-7-14
 *      Author: maxing
 */

#include "BackGroundLayer.h"
#include "GameScene.h"

using namespace cocos2d;

BackGroundLayer::BackGroundLayer() {
}

BackGroundLayer::~BackGroundLayer() {
}

bool BackGroundLayer::init() {
	bool bRet = false;
	do {
		CC_BREAK_IF(! CCLayerColor::initWithColor( ccc4(0,0,0,255) ) );

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
		CCSprite* bg = CCSprite::createWithTexture(
				CCTextureCache::sharedTextureCache()->textureForKey("bg.png"));
		bg->setPosition(CCPoint(origin.x + 0, origin.y ));
		bg->setAnchorPoint(CCPoint(0, 0));
		this->addChild(bg);

		/*m_fire1 = createFireParticle();
		m_fire2 = createFireParticle();
		m_fire3 = createFireParticle();
		m_fire4 = createFireParticle();
		m_fire5 = createFireParticle();
		m_fire6 = createFireParticle();
		this->addChild(m_fire1);
		this->addChild(m_fire2);
		this->addChild(m_fire3);
		this->addChild(m_fire4);
		this->addChild(m_fire5);
		this->addChild(m_fire6);*/

		bRet = true;
	} while (0);

	return bRet;
}

void BackGroundLayer::fire(bool loop) {
	/*m_fire1->setPosition(randomPosition());
	m_fire2->setPosition(randomPosition());
	m_fire3->setPosition(randomPosition());
	m_fire4->setPosition(randomPosition());
	m_fire5->setPosition(randomPosition());
	m_fire6->setPosition(randomPosition());
	CCFiniteTimeAction *action = CCCallFunc::create(m_fire1,
			callfunc_selector(CCParticleSystem::resetSystem));
	this->runAction(action);
	float delayTime;
	delayTime = (rand() % 10)/10.0f+0.5;
	this->showFire(m_fire2, delayTime);
	delayTime += (rand() % 10)/10.0f;
	this->showFire(m_fire3, delayTime);
	delayTime += (rand() % 5)/10.0f+0.5;
	this->showFire(m_fire4, delayTime);
	delayTime += (rand() % 10)/10.0f+0.5;
	this->showFire(m_fire5, delayTime);
	delayTime += (rand() % 10)/10.0f+0.5;
	this->showFire(m_fire6, delayTime);
	if (loop) {
		delayTime += (rand() % 10)/10.0f+2;
		this->delayedFire(delayTime, true);
	}
	GameScene::instance()->playEffect("fire.mp3");*/
}

void BackGroundLayer::delayedFire(float delay, bool loop) {
	/*CCFiniteTimeAction *delayAction, *next, *seq;
	delayAction = CCDelayTime::create(delay);
	next = CCCallFuncND::create(this,
			callfuncND_selector(BackGroundLayer::fire), (void*)loop);
	seq = CCSequence::create(delayAction, next, NULL);
	this->runAction(seq);*/
}

void BackGroundLayer::stopFire() {
	//this->stopAllActions();
}

void BackGroundLayer::showFire(CCParticleSystem* fire, float delay) {
	/*CCFiniteTimeAction *delayAction, *action, *soundAction, *seq;
	delayAction = CCDelayTime::create(delay);
	action = CCCallFuncND::create(this,
			callfuncND_selector(BackGroundLayer::resetFireSelector), (void*)fire);
	seq = CCSequence::create(delayAction, action, NULL);
	this->runAction(seq);*/
}

void BackGroundLayer::resetFireSelector(CCNode* sender, CCParticleSystem* fire) {
	//fire->resetSystem();
}

CCParticleSystem* BackGroundLayer::createFireParticle() {
	/*CCParticleExplosion *emitter = CCParticleExplosion::createWithTotalParticles(200);
	emitter->setTexture(CCTextureCache::sharedTextureCache()->textureForKey("fire.png"));
	emitter->setGravity(CCPoint(0,-100));
	emitter->setLife(2.0f);
	emitter->setLifeVar(0.3);
	//emitter->setRadialAccel(50);
	//emitter->setRadialAccelVar(0);
	emitter->stopSystem();
	return emitter;*/
	return NULL;
}

CCPoint BackGroundLayer::randomPosition() {
	CCSize vs = GameScene::instance()->visibleSize();
	CCPoint origin = GameScene::instance()->visibleOrigin();
	return CCPoint(origin.x + rand() % (int)vs.width,
			origin.y + rand() % (int)(vs.height-240) + 240);
}
