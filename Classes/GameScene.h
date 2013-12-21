#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "Status.h"
#include "StarPuzzle.h"
#include "cocos2d.h"
#include "Config.h"

class StarPuzzle;

#define AD_HEIGHT 80
#define MAX_STAGE 30

class GameLayer;
class BackGroundLayer;
class MainMenuLayer;
class ResumeAndSaveMenuLayer;

class GameScene {
public:
	virtual ~GameScene();
	static GameScene* instance();
	static cocos2d::CCScene* scene() { return instance()->m_scene; }

	void showMainMenu();
	void showResumeMenu();
	void startNewPuzzle(Config::GameMode mode);
	void resumePuzzle();
	void startLearning();
	void returnToPuzzle();
	void saveAndExit();
	void playEffect(const char* file);
	void fire();
	void rate();
	void autoHitTest();
	static int topAdHeight() { return AD_HEIGHT; }
	cocos2d::CCSize visibleSize() { return m_visibleSize; }
	cocos2d::CCPoint visibleOrigin() { return m_origin; }

private:
	GameScene();
	cocos2d::CCScene* m_scene;
	BackGroundLayer*  m_backgroundLayer;
	GameLayer*        m_gameLayer;
	MainMenuLayer*    m_mainMenuLayer;
	ResumeAndSaveMenuLayer* m_resumeAndSaveMenuLayer;
	static GameScene* m_instance;

	cocos2d::CCSize m_visibleSize;
	cocos2d::CCPoint m_origin;
};

class GameLayer : public cocos2d::CCLayer
{
public:
	GameLayer();
	~GameLayer();
	CREATE_FUNC(GameLayer);
	virtual bool init();

	void returnToMainMenu();
	void resumePuzzle(std::string puzzle, int row, int col, int stage, int score, int target);
	void createPuzzle(int row, int col);
	void newStage();
	void nextStage();
	void setGameMode(Config::GameMode mode) { m_mode = mode; }
	void resetStatus();
	void savePuzzle();
	void enableInput();
	void disableInput();
	bool isInputEnabled();
	void enableTouchEvent();
	void disableTouchEvent();
	void enableKeypad();
	void disableKeypad();
	void autoHitTest();
	void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
	virtual void keyBackClicked();
	void handleHitResult(HitResult& hr);

	void newStageInfoAnimation();
	void newStageStatusAnimation();
	void addScoreAnimation(int score, cocos2d::CCPoint pos, float delay, float flyTime);
	float showBonusAnimation();
	// count: 6,7->good, 8,9->cool, 10,11->excellent, 12,13...->fantastic
	void goodGameAnimation(int count);
	void clearPuzzleAnimation();
	void puzzleEnd();
	void explodeTailStarAnimation();
	void explodeRemainStarAnimation();
	void gameOverAnimation();
	void winPuzzleAnimation();
	void scoreHintAnimation(int count, int score);
	void hideScoreHintLabel();
	void bonusHintAnimation(int bonus, float delay);
	void showBonusHintLabel();
	void hideBonusHintLabel();
	void scaleBonusHintLabelToZero();
	void hideNewStageInfoLables();
private:
	enum Animation {
		NewStageInfo,
		NewStageStatus,
		GoodGame,
		ClearPuzzle,
		GameOver,
		WinPuzzle,
		NextStage,
		MainMenu,
		ScoreHint,
		PuzzleEnd,
		ExplodeTailStar,
		ExplodeRemainStars,
	};
	void scheduleAnimation(Animation anim, float delay = 0.0f);
	void resetSpritesAndStatus();
	int target(int stage);
	void checkPuzzleSolved();
	void updateStatus(int cnt, int score, const cocos2d::CCPoint& location);
	void addScore(cocos2d::CCNode* sender, int score);
	void updateHighScore(int score);
	void updateBonusHintLabel(cocos2d::CCNode* sender, int score);
	bool isStageClear(int score);

	StarPuzzle* m_puzzle;
	Status m_status;
	Config::GameMode m_mode;
	int m_targetScoreArray[MAX_STAGE+1];
	bool m_stageCleared;

	cocos2d::CCLabelBMFont*    m_levelLabel;
	cocos2d::CCLabelBMFont*    m_levelNumLabel;
	cocos2d::CCLabelBMFont*    m_targetLabel;
	cocos2d::CCLabelBMFont*    m_targetScoreLabel;
	cocos2d::CCLabelBMFont*    m_bestLabel;
	cocos2d::CCLabelBMFont*    m_bestScoreLabel;
	cocos2d::CCLabelBMFont*    m_stageLabel;
	cocos2d::CCLabelBMFont*    m_stageNumLabel;
	cocos2d::CCLabelBMFont*    m_scoreLabel;
	cocos2d::CCLabelBMFont*    m_scoreNumLabel;
	cocos2d::CCMenuItemSprite* m_resumeSprite;
	cocos2d::CCMenuItemSprite* m_saveSprite;
	cocos2d::CCLabelBMFont*    m_scoreHintLabel;
	cocos2d::CCLabelBMFont*    m_bonusHintLabel;

	cocos2d::CCLabelBMFont*    m_newStageInfoLabel;
	cocos2d::CCLabelBMFont*    m_newStageTargetInfoLabel;

	cocos2d::CCLabelBMFont*    m_goodSprite;
	cocos2d::CCLabelBMFont*    m_coolSprite;
	cocos2d::CCLabelBMFont*    m_excellentSprite;
	cocos2d::CCLabelBMFont*    m_fantasticSprite;
	cocos2d::CCSprite*         m_clearSprite;
	cocos2d::CCSprite*         m_winSprite;
	cocos2d::CCSprite*         m_gameoverSprite;

	cocos2d::CCSprite* m_sound;
	cocos2d::CCSpriteFrame* m_muteFrame;
	cocos2d::CCSpriteFrame* m_soundFrame;
	cocos2d::CCPoint  m_resumeSpritePosition;
	cocos2d::CCPoint  m_saveSpritePosition;
	cocos2d::CCPoint  m_clearSpriteInitPosition;
	cocos2d::CCPoint  m_clearSpriteDisplayPosition;
	float             m_clearSpriteDisplayScale;
	cocos2d::CCPoint  m_winFailSpriteInitPosition;
	cocos2d::CCPoint  m_winFailSpriteDisplayPosition;
	cocos2d::CCPoint  m_newStageInfoLabelInitPosition;
	cocos2d::CCPoint  m_newStageInfoLabelDispPosition;
	cocos2d::CCPoint  m_newStageTargetInfoLabelInitPosition;
	cocos2d::CCPoint  m_newStageTargetInfoLabelDispPosition;
	cocos2d::CCPoint  m_newStageTargetScoreInfoLabelInitPosition;
	cocos2d::CCPoint  m_newStageTargetScoreInfoLabelDispPosition;

	cocos2d::CCPoint  m_topCenter;
	cocos2d::CCPoint  m_rightCenter;
};

#endif  // __GAMELAYER_H__
