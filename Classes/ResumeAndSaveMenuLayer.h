/*
 * ResumeAndSaveMenuLayer.h
 *
 *  Created on: 2013-8-26
 *      Author: maxing
 */

#ifndef RESUMEANDSAVEMENULAYER_H_
#define RESUMEANDSAVEMENULAYER_H_

#include <cocos2d.h>

class ResumeAndSaveMenuLayer: public cocos2d::CCLayer {
public:
	ResumeAndSaveMenuLayer();
	virtual ~ResumeAndSaveMenuLayer();
	CREATE_FUNC(ResumeAndSaveMenuLayer);
	virtual bool init();
	virtual void onEnter();

	virtual void resumeCallback(cocos2d::CCObject* pSender);
	virtual void saveAndExitCallback(cocos2d::CCObject* pSender);

	void animate();
	void disappear(float duration);
	void registerWithTouchDispatcher();

private:
	void resumePuzzle();

	cocos2d::CCMenuItemSprite* m_resumeSprite;
	cocos2d::CCMenuItemSprite* m_saveAndExitSprite;

	cocos2d::CCPoint m_topCenter;
	cocos2d::CCPoint m_resumePosition;
	cocos2d::CCPoint m_saveAndExitPosition;
};

#endif /* RESUMEANDSAVEMENULAYER_H_ */
