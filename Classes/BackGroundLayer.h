/*
 * BackGroundLayer.h
 *
 *  Created on: 2013-7-14
 *      Author: maxing
 */

#ifndef BACKGROUNDLAYER_H_
#define BACKGROUNDLAYER_H_

#include <cocos2d.h>

class BackGroundLayer: public cocos2d::CCLayerColor {
public:
	BackGroundLayer();
	virtual ~BackGroundLayer();
	CREATE_FUNC(BackGroundLayer);
	virtual bool init();

	void fire(bool loop);
	void delayedFire(float delay, bool loop);
	void stopFire();
private:
	cocos2d::CCParticleSystem* createFireParticle();
	cocos2d::CCPoint randomPosition();
	void showFire(cocos2d::CCParticleSystem* fire, float delay);
	void resetFireSelector(cocos2d::CCNode* sender, cocos2d::CCParticleSystem* fire);
	cocos2d::CCParticleSystem* m_fire1;
	cocos2d::CCParticleSystem* m_fire2;
	cocos2d::CCParticleSystem* m_fire3;
	cocos2d::CCParticleSystem* m_fire4;
	cocos2d::CCParticleSystem* m_fire5;
	cocos2d::CCParticleSystem* m_fire6;
};

#endif /* BACKGROUNDLAYER_H_ */
