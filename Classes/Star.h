/*
 * Star.h
 *
 *  Created on: 2013-7-10
 *      Author: maxing
 */

#ifndef STAR_H_
#define STAR_H_

#include "cocos2d.h"

using namespace cocos2d;

#define STAR_WIDTH  72
#define STAR_HEIGHT 72

class Star {
public:
	enum StarType {
		Yellow		= 1,
		Green		= 2,
		Red			= 3,
		Blue		= 4,
		Puple		= 5,
	};

	static StarType getType(int index) {
		switch (index) {
		case 1:  return Yellow; break;
		case 2:  return Green;  break;
		case 3:  return Red;    break;
		case 4:  return Blue;   break;
		case 5:  return Puple;  break;
		default: return Yellow; break;
		}
	}

	Star(StarType type, float scale = 1.0f);
	virtual ~Star();

	StarType type() const { return m_type; }
	CCPoint getPosition() const;
	void setPosition(CCPoint p); // �޶���
	void setPosition(float x, float y);
	void moveTo(CCPoint p); //��
	void moveTo(float x, float y) { moveTo(CCPoint(x, y)); }
	void move(float x, float y) { move(CCPoint(x, y)); } //��
	void move(CCPoint p) { moveTo(m_sprite->getPosition()+p); }

	void runAction(CCAction* action);
	CCParticleSystem* explode();

	void select();
	void unselect();
	void setScale(float scale);
	float scale() { return m_scale; }

	bool attach(CCNode* node);
	bool detach(CCNode* node = NULL);

private:
	StarType m_type;
	float m_scale;
	bool m_selected;

	CCSprite *m_sprite;
	CCSpriteFrame *m_normalFrame;
	CCSpriteFrame *m_selectedFrame;
	CCNode *m_attachedNode;
};

CCSprite* createStarSprite(Star::StarType type);

#endif /* STAR_H_ */
