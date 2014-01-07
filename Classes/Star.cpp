/*
 * Star.cpp
 *
 *  Created on: 2013-7-10
 *      Author: maxing
 */

#include "Star.h"
#include <string>

Star::Star(StarType type, float scale)
    : m_type(type)
    , m_scale(scale)
    , m_attachedNode(0)
    , m_selected(false)
{
	/*CCString normal, selected;
	normal.initWithFormat("star%da.png", type);
	selected.initWithFormat("star%db.png", type);*/
	const char *normal, *selected;
	if (type == Yellow) {
		normal = "star1.png";
		selected = "star1_select.png";
	}
	else if (type == Green) {
		normal = "star2.png";
		selected = "star2_select.png";
	}
	else if (type == Red) {
		normal = "star3.png";
		selected = "star3_select.png";
	}
	else if (type == Blue) {
		normal = "star4.png";
		selected = "star4_select.png";
	}
	else if (type == Puple) {
		normal = "star5.png";
		selected = "star5_select.png";
	}
	m_sprite = CCSprite::createWithSpriteFrameName(normal);
	m_sprite->retain();
	m_sprite->setAnchorPoint(CCPoint(0, 0));
	m_normalFrame = m_sprite->displayFrame();
	m_normalFrame->retain();
	CCSpriteFrameCache* cacher = CCSpriteFrameCache::sharedSpriteFrameCache();
	m_selectedFrame = cacher->spriteFrameByName(selected);
	m_selectedFrame->retain();
}

Star::~Star() {
	detach();
	m_sprite->release();
	m_normalFrame->release();
	m_selectedFrame->release();
}

CCPoint Star::getPosition() const {
	return m_sprite->getPosition();
}

void Star::setPosition(CCPoint p) {
	m_sprite->setPosition(p);
}

void Star::setPosition(float x, float y) {
	m_sprite->setPosition(CCPoint(x, y));
}

void Star::moveTo(CCPoint p) {
	m_sprite->runAction(CCEaseBackInOut::create(CCMoveTo::create(0.3f, p)));
}

void Star::runAction(CCAction* action) {
	m_sprite->runAction(action);
}

CCParticleSystem* Star::explode() {
	CCParticleExplosion *emitter = CCParticleExplosion::createWithTotalParticles(5);
	CCString key;
	key.initWithFormat("Star_%d.png", m_type);
	emitter->setTexture(CCTextureCache::sharedTextureCache()->textureForKey(key.getCString()));
	emitter->setPosition(m_sprite->getPosition()+CCPoint(STAR_WIDTH/2, STAR_HEIGHT/2));
	emitter->setGravity(CCPoint(0,-240));
	emitter->setRadialAccel(50);
	emitter->setRadialAccelVar(0);
	emitter->setStartColor(ccc4f(1.0f, 1.0f, 1.0f, 1.0f));
	emitter->setStartColorVar(ccc4f(0.0f, 0.0f, 0.0f, 0.0f));
	emitter->setEndColor(ccc4f(1.0f, 1.0f, 1.0f, 1.0f));
	emitter->setEndColorVar(ccc4f(0.0f, 0.0f, 0.0f, 0.0f));
	if (m_attachedNode)
		m_attachedNode->addChild(emitter, 999);
	return emitter;
}

void Star::select() {
	if (m_selected)
		return;
	m_selected = true;
	m_sprite->setDisplayFrame(m_selectedFrame);
}

void Star::unselect() {
	if (m_selected == false)
		return;
	m_selected = false;
	m_sprite->setDisplayFrame(m_normalFrame);
}

bool Star::attach(CCNode* node) {
	if (node == NULL)
		return false;

	detach();
	node->addChild(m_sprite);
	m_attachedNode = node;
	return true;
}

bool Star::detach(CCNode* node) {
	if (m_attachedNode == NULL)
		return false;

	if (node == NULL || node == m_attachedNode) {
		m_attachedNode->removeChild(m_sprite);
		m_attachedNode = NULL;
		return true;
	}

	return false;
}

CCSprite* createStarSprite(Star::StarType type) {
	std::string resource = "star";
	resource.append(1, (char)('0'+type));
	resource.append("a.png");
	CCSprite* sprite = CCSprite::createWithSpriteFrameName(resource.c_str());
	sprite->setAnchorPoint(CCPoint(0, 0));
	return sprite;
}
