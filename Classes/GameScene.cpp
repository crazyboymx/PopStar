#include "GameScene.h"
#include "MainMenuLayer.h"
#include "ResumeAndSaveMenuLayer.h"
#include "BackGroundLayer.h"
#include "SimpleAudioEngine.h"
#include "StarPuzzle.h"
#include "Config.h"
#include "bridge.h"

using namespace cocos2d;

GameScene* GameScene::m_instance = NULL;

#define PUZZLE_SIZE 10

GameScene::GameScene() {
	m_scene = CCScene::create();
	m_scene->retain();
	m_backgroundLayer = BackGroundLayer::create();
	m_scene->addChild(m_backgroundLayer, -1000);
	m_mainMenuLayer = MainMenuLayer::create();
	m_mainMenuLayer->retain();
	m_gameLayer = GameLayer::create();
	m_gameLayer->retain();
	m_resumeAndSaveMenuLayer = ResumeAndSaveMenuLayer::create();
	m_resumeAndSaveMenuLayer->retain();

    m_visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    m_origin = CCDirector::sharedDirector()->getVisibleOrigin();
}

GameScene::~GameScene() {
	m_scene->release();
	m_mainMenuLayer->release();
	m_gameLayer->release();
	m_resumeAndSaveMenuLayer->release();
}

GameScene* GameScene::instance() {
	if (m_instance == NULL)
		m_instance = new GameScene();
	return m_instance;
}

void GameScene::showMainMenu() {
	m_scene->addChild(m_mainMenuLayer, 1000);
	m_scene->removeChild(m_gameLayer);
	m_backgroundLayer->delayedFire(2.0f, true);
}

void GameScene::showResumeMenu() {
	m_gameLayer->addChild(m_resumeAndSaveMenuLayer, 999);
	m_gameLayer->disableInput();
}

void GameScene::startNewPuzzle() {
	Config::instance()->clearSavedPuzzle();
	m_backgroundLayer->stopFire();
	m_scene->removeChild(m_mainMenuLayer);
	m_scene->addChild(m_gameLayer);
	m_gameLayer->setGameMode(Config::Normal);
	m_gameLayer->disableInput();
	m_gameLayer->resetStatus();
	m_gameLayer->nextStage();
}

void GameScene::resumePuzzle() {
	m_backgroundLayer->stopFire();
	m_scene->removeChild(m_mainMenuLayer);
	m_scene->addChild(m_gameLayer);
	Config* conf = Config::instance();
	std::string puzzle = conf->puzzle();
	int row = conf->row();
	int col = conf->col();
	int stage = conf->level();
	int score = conf->score();
	int target = conf->target();
	m_gameLayer->setGameMode(conf->savedPuzzleMode());
	m_gameLayer->resumePuzzle(puzzle, row, col, stage, score, target);
}

void GameScene::startLearning() {
	m_backgroundLayer->stopFire();
	m_scene->removeChild(m_mainMenuLayer);
	m_scene->addChild(m_gameLayer);
	m_gameLayer->setGameMode(Config::Learning);
	m_gameLayer->disableInput();
	m_gameLayer->resetStatus();
	m_gameLayer->nextStage();
}

void GameScene::returnToPuzzle() {
	m_gameLayer->removeChild(m_resumeAndSaveMenuLayer);
	m_gameLayer->enableInput();
}

void GameScene::saveAndExit() {
	m_gameLayer->savePuzzle();
	returnToPuzzle();
	CCFiniteTimeAction* callBack = CCCallFunc::create(m_gameLayer,
			callfunc_selector(GameLayer::returnToMainMenu));
	m_gameLayer->runAction(CCSequence::create(
			CCFadeOutTRTiles::create(2.0f, CCSize(16, 12)),
			CCStopGrid::create(), callBack, NULL));
}

void GameScene::playEffect(const char *file) {
	if (Config::instance()->mute() == false)
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(file);
}

void GameScene::fire() {
	m_backgroundLayer->fire(false);
}

void GameScene::rate() {
	::rate();
}

void GameScene::autoHitTest()
{
	if(m_gameLayer->getParent() == m_scene)
		m_gameLayer->autoHitTest();
}

GameLayer::GameLayer() {
	m_puzzle = 0;
	m_mode = Config::Normal;
	m_targetScoreArray[0] = 0;
	m_targetScoreArray[1] = 1000;
	m_targetScoreArray[2] = 3000;
	m_targetScoreArray[3] = 6000;
	m_targetScoreArray[4] = 8000;
	m_targetScoreArray[5] = 10000;
	m_targetScoreArray[6] = 12000;
	m_targetScoreArray[7] = 15000;
	m_targetScoreArray[8] = 17000;
	m_targetScoreArray[9] = 20000;
	for (int i = 10; i <= MAX_STAGE; i++)
	    m_targetScoreArray[i] = 20000 + 4000 * (i - 9);
}

GameLayer::~GameLayer() {
	if (m_puzzle)
		delete m_puzzle;
}

bool GameLayer::init() {
	bool bRet = false;
	do {
		CCLayer::init();
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        CCLog("visibleSize %0.2f, %0.2f, origin %0.2f %0.2f", visibleSize.width,
        		visibleSize.height, origin.x, origin.y);
        int adHeight = GameScene::topAdHeight();
        m_topCenter = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height);
        m_rightCenter = CCPoint(origin.x + visibleSize.width, origin.y + visibleSize.height/2);
        CCSpriteFrameCache* cacher = CCSpriteFrameCache::sharedSpriteFrameCache();

        m_status.mute = Config::instance()->mute();
        m_status.highScore = Config::instance()->highScore();

        // best 165*28
		m_bestSprite = CCSprite::createWithSpriteFrameName("best.png");
		m_bestSprite->setPosition(CCPoint(origin.x + 0, origin.y + visibleSize.height - adHeight - 28));
		m_bestSprite->setAnchorPoint(CCPoint(0, 0));
		this->addChild(m_bestSprite);
		// bestbg 258*28
		m_bestbgSprite = CCSprite::createWithSpriteFrameName("bestbg.png");
		m_bestbgSprite->setPosition(CCPoint(origin.x + 165, origin.y + visibleSize.height - adHeight - 28));
		m_bestbgSprite->setAnchorPoint(CCPoint(0, 0));
		this->addChild(m_bestbgSprite);
		// bestScoreLable
		m_bestScoreLabel = CCLabelBMFont::create(
				CCString::createWithFormat("%d", m_status.highScore)->getCString(),"highscore.fnt");
		m_bestScoreLabel->setPosition(CCPoint(origin.x + 294, origin.y + visibleSize.height - adHeight - 28));
		m_bestScoreLabel->setAnchorPoint(CCPoint(0.5, 0));
		this->addChild(m_bestScoreLabel);
		// pause 36*28
		/*m_pauseSprite = CCSprite::createWithSpriteFrameName("pause.png");
		m_pauseSprite->setPosition(CCPoint(origin.x + 433, origin.y + visibleSize.height - adHeight - 28));
		m_pauseSprite->setAnchorPoint(CCPoint(0, 0));
		this->addChild(m_pauseSprite);*/
		// stage 90*28
		m_stageSprite = CCSprite::createWithSpriteFrameName("stage.png");
		m_stageSprite->setPosition(CCPoint(origin.x + 0, origin.y + visibleSize.height - adHeight - 56));
		m_stageSprite->setAnchorPoint(CCPoint(0, 0));
		this->addChild(m_stageSprite);
		// stagebg 50*28
		m_stagebgSprite = CCSprite::createWithSpriteFrameName("stagebg.png");
		m_stagebgSprite->setPosition(CCPoint(origin.x + 90, origin.y + visibleSize.height - adHeight - 56));
		m_stagebgSprite->setAnchorPoint(CCPoint(0, 0));
		this->addChild(m_stagebgSprite);
		// stageLable
		m_stageLabel = CCLabelBMFont::create(
				CCString::createWithFormat("%d", m_status.stage)->getCString(),"highscore.fnt");
		m_stageLabel->setPosition(CCPoint(origin.x + 115, origin.y + visibleSize.height - adHeight - 56));
		m_stageLabel->setAnchorPoint(CCPoint(0.5, 0));
		this->addChild(m_stageLabel);
		// target 110*28
		m_targetSprite = CCSprite::createWithSpriteFrameName("target.png");
		m_targetSprite->setPosition(CCPoint(origin.x + 140, origin.y + visibleSize.height - adHeight - 56));
		m_targetSprite->setAnchorPoint(CCPoint(0, 0));
		this->addChild(m_targetSprite);
		// targetbg 234*28
		m_targetbgSprite = CCSprite::createWithSpriteFrameName("targetbg.png");
		m_targetbgSprite->setPosition(CCPoint(origin.x + 250, origin.y + visibleSize.height - adHeight - 56));
		m_targetbgSprite->setAnchorPoint(CCPoint(0, 0));
		this->addChild(m_targetbgSprite);
		// targetScoreTopLable
		m_targetScoreTopLabel = CCLabelBMFont::create(
				CCString::createWithFormat("%d", 0)->getCString(),"highscore.fnt");
		m_targetScoreTopLabel->setPosition(CCPoint(origin.x + 367, origin.y + visibleSize.height - adHeight - 56));
		m_targetScoreTopLabel->setAnchorPoint(CCPoint(0.5, 0));
		this->addChild(m_targetScoreTopLabel);
		// score 95*28
		m_scoreSprite = CCSprite::createWithSpriteFrameName("score.png");
		m_scoreSprite->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height - adHeight - 112));
		m_scoreSprite->setAnchorPoint(CCPoint(0.5, 0));
		this->addChild(m_scoreSprite);
		// scorebg 235*28
		m_scorebgSprite = CCSprite::createWithSpriteFrameName("scorebg.png");
		m_scorebgSprite->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height - adHeight - 140));
		m_scorebgSprite->setAnchorPoint(CCPoint(0.5, 0));
		this->addChild(m_scorebgSprite);
		// scoreLable x*28
		m_scoreLabel = CCLabelBMFont::create(
				CCString::createWithFormat("%d", m_status.score)->getCString(),"highscore.fnt");
		m_scoreLabel->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height - adHeight - 140));
		m_scoreLabel->setAnchorPoint(CCPoint(0.5, 0));
		this->addChild(m_scoreLabel);

		// scoreHintLable x*28
		m_scoreHintLabel = CCLabelBMFont::create(
				CCString::createWithFormat("%d blocks %d points", 0, 0)->getCString(),"highscore.fnt");
		m_scoreHintLabel->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height - adHeight - 168));
		m_scoreHintLabel->setAnchorPoint(CCPoint(0.5, 0));
		m_scoreHintLabel->setVisible(false);
		this->addChild(m_scoreHintLabel);
		// bonusHintLabel x*28
		m_bonusHintLabel = CCLabelBMFont::create(
				CCString::createWithFormat("%d blocks %d points", 0, 0)->getCString(),"highscore.fnt");
		m_bonusHintLabel->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
		m_bonusHintLabel->setAnchorPoint(CCPoint(0.5, 0));
		m_bonusHintLabel->setVisible(false);
		this->addChild(m_bonusHintLabel);

		// new stage info labels
		m_newStageInfoLabel = CCLabelBMFont::create(
				CCString::createWithFormat("level %d", 0)->getCString(),"highscore.fnt");
		m_newStageInfoLabelInitPosition = CCPoint(origin.x + visibleSize.width+80, origin.y + visibleSize.height/2+28);
		m_newStageInfoLabelDispPosition = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2+28);
		m_newStageInfoLabel->setPosition(m_newStageInfoLabelInitPosition);
		m_newStageInfoLabel->setAnchorPoint(CCPoint(0.5, 0.5));
		m_newStageInfoLabel->setVisible(false);
		this->addChild(m_newStageInfoLabel, 999);
		m_newStageTargetInfoLabel = CCLabelBMFont::create(
				CCString::createWithFormat("target score is")->getCString(),"highscore.fnt");
		m_newStageTargetInfoLabelInitPosition = CCPoint(origin.x + visibleSize.width+80, origin.y + visibleSize.height/2);
		m_newStageTargetInfoLabelDispPosition = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
		m_newStageTargetInfoLabel->setPosition(m_newStageTargetInfoLabelInitPosition);
		m_newStageTargetInfoLabel->setAnchorPoint(CCPoint(0.5, 0.5));
		m_newStageTargetInfoLabel->setVisible(false);
		this->addChild(m_newStageTargetInfoLabel, 999);
		m_newStageTargetScoreInfoLabel = CCLabelBMFont::create(
				CCString::createWithFormat("%d", m_status.target)->getCString(),"highscore.fnt");
		m_newStageTargetScoreInfoLabelInitPosition = CCPoint(origin.x + visibleSize.width+80, origin.y + visibleSize.height/2-28);
		m_newStageTargetScoreInfoLabelDispPosition = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2-28);
		m_newStageTargetScoreInfoLabel->setPosition(m_newStageTargetScoreInfoLabelInitPosition);
		m_newStageTargetScoreInfoLabel->setAnchorPoint(CCPoint(0.5, 0.5));
		m_newStageTargetScoreInfoLabel->setVisible(false);
		//m_newStageTargetScoreInfoLabel->setScale(1.2);
		this->addChild(m_newStageTargetScoreInfoLabel, 999);

		// goodSprite 186*120
		m_goodSprite = CCSprite::createWithSpriteFrameName("good.png");
		m_goodSprite->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + PUZZLE_SIZE * STAR_HEIGHT + 60));
		m_goodSprite->setAnchorPoint(CCPoint(0.5, 0.5));
		this->addChild(m_goodSprite);
		m_goodSprite->setVisible(false);
		// coolSprite 186*120
		m_coolSprite = CCSprite::createWithSpriteFrameName("cool.png");
		m_coolSprite->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + PUZZLE_SIZE * STAR_HEIGHT + 60));
		m_coolSprite->setAnchorPoint(CCPoint(0.5, 0.5));
		this->addChild(m_coolSprite);
		m_coolSprite->setVisible(false);
		// excellentSprite 310*120
		m_excellentSprite = CCSprite::createWithSpriteFrameName("excellent.png");
		m_excellentSprite->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + PUZZLE_SIZE * STAR_HEIGHT + 60));
		m_excellentSprite->setAnchorPoint(CCPoint(0.5, 0.5));
		this->addChild(m_excellentSprite);
		m_excellentSprite->setVisible(false);
		// fantasticSprite 294, 104
		m_fantasticSprite = CCSprite::createWithSpriteFrameName("fantastic.png");
		m_fantasticSprite->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + PUZZLE_SIZE * STAR_HEIGHT + 52));
		m_fantasticSprite->setAnchorPoint(CCPoint(0.5, 0.5));
		this->addChild(m_fantasticSprite);
		m_fantasticSprite->setVisible(false);

		// clearSprite 200*143
		m_clearSprite = CCSprite::createWithSpriteFrameName("clear.png");
		m_clearSprite->setPosition(CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
		m_clearSprite->setAnchorPoint(CCPoint(0.5, 0.5));
		this->addChild(m_clearSprite, 999);
		m_clearSprite->setVisible(false);
		m_clearSpriteInitPosition = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
		m_clearSpriteDisplayScale = 0.25f;
		m_clearSpriteDisplayPosition = CCPoint(
				origin.x + 200*0.5*m_clearSpriteDisplayScale+10,
				origin.y + visibleSize.height - adHeight - 56 - 143*0.5*m_clearSpriteDisplayScale);

		// winSprite 200*78
		m_winFailSpriteInitPosition = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height+40);
		m_winFailSpriteDisplayPosition = CCPoint(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2);
		m_winSprite = CCSprite::createWithSpriteFrameName("win.png");
		m_winSprite->setPosition(m_winFailSpriteInitPosition);
		m_winSprite->setAnchorPoint(CCPoint(0.5, 0.5));
		this->addChild(m_winSprite, 999);
		// gameoverSprite 335*75
		m_gameoverSprite = CCSprite::createWithSpriteFrameName("gameover.png");
		m_gameoverSprite->setPosition(m_winFailSpriteInitPosition);
		m_gameoverSprite->setAnchorPoint(CCPoint(0.5, 0.5));
		this->addChild(m_gameoverSprite, 999);

		this->setTouchEnabled(true);
		bRet = true;
	} while (0);

	return bRet;
}

void GameLayer::returnToMainMenu() {
	GameScene::instance()->showMainMenu();
}

void GameLayer::resumePuzzle(std::string puzzle, int row, int col, int stage, int score, int target) {
	if (m_puzzle)
		delete m_puzzle;
	disableInput();
	m_puzzle = new StarPuzzle(row, col);
	m_puzzle->setTop(m_topCenter.y);
	m_puzzle->setLeft(GameScene::instance()->visibleOrigin().x);
	m_puzzle->setBottom(GameScene::instance()->visibleOrigin().y);
	m_puzzle->setRight(m_rightCenter.x);
	m_puzzle->loadPuzzle(puzzle);
	m_puzzle->attach(this);
	m_status.stage = stage;
	m_status.score = score;
	m_status.target = target;
	resetSpritesAndStatus();
	scheduleAnimation(NewStageInfo);
}

void GameLayer::createPuzzle(int row, int col) {
	if (m_puzzle)
		delete m_puzzle;

	m_puzzle = new StarPuzzle(row, col);
	m_puzzle->setTop(m_topCenter.y);
	m_puzzle->setLeft(GameScene::instance()->visibleOrigin().x);
	m_puzzle->setBottom(GameScene::instance()->visibleOrigin().y);
	m_puzzle->setRight(m_rightCenter.x);
	m_puzzle->initPuzzle();
	m_puzzle->attach(this);
}

int GameLayer::target(int stage) {
	if (stage <= 9)
		return m_targetScoreArray[stage];
	return m_targetScoreArray[9] + (stage - 9) * 4000;
}

void GameLayer::checkPuzzleSolved() {
	if (m_puzzle->isSolved() == false)
		return;

	disableInput();
	int bonus = m_puzzle->bonus();
	float delay = showBonusAnimation();
	if (bonus > 0) {
		addScoreAnimation(bonus, m_bonusHintLabel->getPosition(), delay, 0.5f);
		delay += 0.5f;
	}
	delay += 0.5f;
	scheduleAnimation(PuzzleEnd, delay);
}

void GameLayer::updateStatus(int cnt, int score, const CCPoint& location) {
	float roundTime = 0.4;
	float interv = 0.1f;
	int roundScore = score/cnt;
	for (int i = 0; i < cnt; i++) {
		addScoreAnimation(roundScore, location, interv * i, roundTime);
	}
}

void GameLayer::addScore(CCNode* sender, int score) {
	m_status.score += score;
	m_scoreLabel->setCString(
			CCString::createWithFormat("%d", m_status.score)->getCString());
}

void GameLayer::updateHighScore(int score) {
	if (score > m_status.highScore) {
		m_status.highScore = score;
		m_bestScoreLabel->setCString(
			CCString::createWithFormat("%d", m_status.highScore)->getCString());
	}
}

void GameLayer::updateBonusHintLabel(CCNode* sender, int score) {
	m_bonusHintLabel->setCString(
			CCString::createWithFormat("bonus %d", score)->getCString());
}

bool GameLayer::isStageClear(int score) {
	return m_status.target <= score;
}

void GameLayer::newStage() {
	createPuzzle(PUZZLE_SIZE, PUZZLE_SIZE);
	if (m_mode == Config::Normal)
		m_status.target = target(m_status.stage);
	else if (m_mode == Config::Challenge) {
		int strength = std::min(100, 80 + m_status.stage);
		m_status.target = m_status.lastTarget + m_puzzle->evaluateMaxScore() * strength/100;
	}
	else if (m_mode == Config::Learning) {
		m_status.target = m_puzzle->evaluateMaxScore();
		m_status.score = 0;
		m_status.stage = 1;
	}
	resetSpritesAndStatus();
	m_stageLabel->setCString(CCString::createWithFormat("%d", m_status.stage)->getCString());
	m_targetScoreTopLabel->setCString(CCString::createWithFormat("%d", m_status.target)->getCString());
	scheduleAnimation(NewStageInfo);
}

void GameLayer::nextStage() {
	m_status.stage ++;
	newStage();
}

void GameLayer::resetStatus() {
	m_status.stage = 0;
	m_status.score = 0;
	m_status.lastTarget = 0;
}

void GameLayer::savePuzzle() {
	if (m_puzzle == NULL || m_mode == Config::Learning)
		return;
	std::string puzzle = m_puzzle->serialization();
	Config::instance()->savePuzzle(puzzle, PUZZLE_SIZE, PUZZLE_SIZE, m_status.stage, m_status.score, m_status.target);
	Config::instance()->setSavedPuzzleMode(Config::Normal);
	Config::instance()->flush();
}

void GameLayer::autoHitTest() {
	if (!isInputEnabled()) {
		return;
	}
	HitResult hr = m_puzzle->autoSelect();
	handleHitResult(hr);
}

void GameLayer::ccTouchesEnded(CCSet* touches, CCEvent* event) {
	CCTouch* touch = (CCTouch*)( touches->anyObject() );
	CCPoint location = touch->getLocation();

	HitResult result;
	if (m_mode == Config::Learning)
		result = m_puzzle->hitTestAuto();
	else
		result = m_puzzle->hitTest(CCPoint(location.x, location.y));
	handleHitResult(result);
}

void GameLayer::keyBackClicked() {
	GameScene::instance()->showResumeMenu();
}

void GameLayer::handleHitResult(HitResult& hr) {
	hideScoreHintLabel();
	if (hr.type == HitResult::HIT_STAR) {
		GameScene::instance()->playEffect("select.mp3");
		scoreHintAnimation(hr.data, m_puzzle->score(hr.data));
	}
	else if (hr.type == HitResult::HIT_POPSTAR) {
		m_puzzle->popStar();
		m_puzzle->applyChanges();
		goodGameAnimation(hr.data);
		updateStatus(hr.data, m_puzzle->score(hr.data), hr.location);
		if (!m_stageCleared && isStageClear(m_status.score)) {
			clearPuzzleAnimation();
			m_stageCleared = true;
		}
		checkPuzzleSolved();
	}
}

void GameLayer::enableInput() {
	enableTouchEvent();
	enableKeypad();
}

void GameLayer::disableInput() {
	disableTouchEvent();
	disableKeypad();
}

bool GameLayer::isInputEnabled() {
	return isKeypadEnabled() && isTouchEnabled();
}

void GameLayer::enableTouchEvent() {
	this->setTouchEnabled(true);
    CCDirector::sharedDirector()->getTouchDispatcher()->addStandardDelegate(this,0);
}

void GameLayer::disableTouchEvent() {
	this->setTouchEnabled(false);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void GameLayer::enableKeypad() {
	this->setKeypadEnabled(true);
}

void GameLayer::disableKeypad() {
	this->setKeypadEnabled(false);
}

void GameLayer::resetSpritesAndStatus() {
	m_stageCleared = false;
	m_clearSprite->setVisible(false);
	m_winSprite->setPosition(m_winFailSpriteInitPosition);
	m_gameoverSprite->setPosition(m_winFailSpriteInitPosition);

	m_bestSprite->setVisible(false);
	m_bestbgSprite->setVisible(false);
	m_bestScoreLabel->setCString(
				CCString::createWithFormat("%d", m_status.highScore)->getCString());
	m_bestScoreLabel->setVisible(false);
	m_stageSprite->setVisible(false);
	m_stagebgSprite->setVisible(false);
	m_stageLabel->setCString(
				CCString::createWithFormat("%d", m_status.stage)->getCString());
	m_stageLabel->setVisible(false);
	m_targetSprite->setVisible(false);
	m_targetbgSprite->setVisible(false);
	m_targetScoreTopLabel->setCString(
				CCString::createWithFormat("%d", m_status.target)->getCString());
	m_targetScoreTopLabel->setVisible(false);
	m_scoreSprite->setVisible(false);
	m_scorebgSprite->setVisible(false);
	m_scoreLabel->setCString(
				CCString::createWithFormat("%d", m_status.score)->getCString());
	m_scoreLabel->setVisible(false);
	hideScoreHintLabel();
	hideBonusHintLabel();
}

void GameLayer::scheduleAnimation(Animation anim, float delay) {
	CCFiniteTimeAction* delayAction, *animAction = NULL;
	delayAction = CCDelayTime::create(delay);
	switch (anim) {
	case NewStageInfo:
		animAction = CCCallFunc::create(this,
				callfunc_selector(GameLayer::newStageInfoAnimation));
		break;
	case NewStageStatus:
		animAction = CCCallFunc::create(this,
				callfunc_selector(GameLayer::newStageStatusAnimation));
		break;
	case GoodGame:
	case ClearPuzzle:
		break;
	case GameOver:
		animAction = CCCallFunc::create(this,
				callfunc_selector(GameLayer::gameOverAnimation));
		break;
	case WinPuzzle:
		animAction = CCCallFunc::create(this,
				callfunc_selector(GameLayer::winPuzzleAnimation));
		break;
	case NextStage:
		animAction = CCCallFunc::create(this,
				callfunc_selector(GameLayer::nextStage));
		break;
	case MainMenu:
		animAction = CCCallFunc::create(this,
				callfunc_selector(GameLayer::returnToMainMenu));
		break;
	case ScoreHint:
		break;
	case PuzzleEnd:
		animAction = CCCallFunc::create(this,
				callfunc_selector(GameLayer::puzzleEnd));
		break;
	case ExplodeTailStar:
		animAction = CCCallFunc::create(this,
				callfunc_selector(GameLayer::explodeTailStarAnimation));
		break;
	case ExplodeRemainStars:
		animAction = CCCallFunc::create(this,
				callfunc_selector(GameLayer::explodeRemainStarAnimation));
		break;
	default:
		break;
	}
	this->runAction(CCSequence::create(delayAction, animAction, NULL));
}

void GameLayer::newStageInfoAnimation() {
	float actionTime = 0.5f;
	float delayTime = 0.3f;
	CCFiniteTimeAction* action, *delay, *seq;

	m_newStageInfoLabel->setCString(
			CCString::createWithFormat("level %d", m_status.stage)->getCString());
	m_newStageInfoLabel->setVisible(true);
	m_newStageInfoLabel->setPosition(m_newStageInfoLabelInitPosition);
	action = CCMoveTo::create(actionTime, m_newStageInfoLabelDispPosition);
	m_newStageInfoLabel->runAction(action);

	m_newStageTargetInfoLabel->setVisible(true);
	m_newStageTargetInfoLabel->setPosition(m_newStageTargetInfoLabelInitPosition);
	delay = CCDelayTime::create(delayTime);
	action = CCMoveTo::create(actionTime, m_newStageTargetInfoLabelDispPosition);
	seq = CCSequence::create(delay, action, NULL);
	m_newStageTargetInfoLabel->runAction(seq);

	m_newStageTargetScoreInfoLabel->setCString(
			CCString::createWithFormat("%d", m_status.target)->getCString());
	m_newStageTargetScoreInfoLabel->setVisible(true);
	m_newStageTargetScoreInfoLabel->setPosition(m_newStageTargetScoreInfoLabelInitPosition);
	delay = CCDelayTime::create(delayTime*2);
	action = CCMoveTo::create(actionTime, m_newStageTargetScoreInfoLabelDispPosition);
	seq = CCSequence::create(delay, action, NULL);
	m_newStageTargetScoreInfoLabel->runAction(seq);
	scheduleAnimation(NewStageStatus, delayTime*2 + actionTime + 1.0f);
}

void GameLayer::newStageStatusAnimation() {
	hideNewStageInfoLables();
	float delay = m_puzzle->puzzleStartAnimation();
	CCFiniteTimeAction* delayAction;
	CCFiniteTimeAction* action;
	CCAction* enableInputAction = CCCallFunc::create(this, callfunc_selector(GameLayer::enableInput));
	delayAction = CCDelayTime::create(delay);
	float actionTime = 0.5f;
	float interv = 0.4f;
	action = CCFadeIn::create(actionTime);
	m_bestSprite->setVisible(true);
	m_bestSprite->setOpacity(0);
	m_bestSprite->runAction(CCSequence::create(delayAction, enableInputAction, action, NULL));
	delayAction = CCDelayTime::create(delay);
	action = CCFadeIn::create(actionTime);
	m_bestbgSprite->setVisible(true);
	m_bestbgSprite->setOpacity(0);
	m_bestbgSprite->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(delay);
	action = CCFadeIn::create(actionTime);
	m_bestScoreLabel->setVisible(true);
	m_bestScoreLabel->setOpacity(0);
	m_bestScoreLabel->runAction(CCSequence::create(delayAction, action, NULL));

	delayAction = CCDelayTime::create(delay+interv);
	action = CCFadeIn::create(actionTime);
	m_stageSprite->setVisible(true);
	m_stageSprite->setOpacity(0);
	m_stageSprite->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(delay+interv);
	action = CCFadeIn::create(actionTime);
	m_stagebgSprite->setVisible(true);
	m_stagebgSprite->setOpacity(0);
	m_stagebgSprite->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(delay+interv);
	action = CCFadeIn::create(actionTime);
	m_stageLabel->setVisible(true);
	m_stageLabel->setOpacity(0);
	m_stageLabel->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(delay+interv);
	action = CCFadeIn::create(actionTime);
	m_targetSprite->setVisible(true);
	m_targetSprite->setOpacity(0);
	m_targetSprite->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(delay+interv);
	action = CCFadeIn::create(actionTime);
	m_targetbgSprite->setVisible(true);
	m_targetbgSprite->setOpacity(0);
	m_targetbgSprite->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(delay+interv);
	action = CCFadeIn::create(actionTime);
	m_targetScoreTopLabel->setVisible(true);
	m_targetScoreTopLabel->setOpacity(0);
	m_targetScoreTopLabel->runAction(CCSequence::create(delayAction, action, NULL));

	delayAction = CCDelayTime::create(delay+interv*2);
	action = CCFadeIn::create(actionTime);
	m_scoreSprite->setVisible(true);
	m_scoreSprite->setOpacity(0);
	m_scoreSprite->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(delay+interv*2);
	action = CCFadeIn::create(actionTime);
	m_scorebgSprite->setVisible(true);
	m_scorebgSprite->setOpacity(0);
	m_scorebgSprite->runAction(CCSequence::create(delayAction, action, NULL));
	delayAction = CCDelayTime::create(delay+interv*2);
	action = CCFadeIn::create(actionTime);
	m_scoreLabel->setVisible(true);
	m_scoreLabel->setOpacity(0);
	m_scoreLabel->runAction(CCSequence::create(delayAction, action, NULL));
}

void GameLayer::goodGameAnimation(int count) {
	if (count < 6)
		return;

	CCFiniteTimeAction* delay;
	CCFiniteTimeAction* action1, *action2, *action3;
	CCFiniteTimeAction* final;
	action1 = CCFadeIn::create(0.01f);
	action2 = CCScaleTo::create(0.2f, 1.0f);
	delay = CCDelayTime::create(1.0f);
	action3 = CCFadeOut::create(0.2f);
	final = CCSequence::create(action1, action2, delay, action3, NULL);

	if (count < 8) {
		m_goodSprite->setScale(0.1f);
		m_goodSprite->setVisible(true);
		m_goodSprite->runAction(final);
	}
	else if (count < 10) {
		m_coolSprite->setScale(0.1f);
		m_coolSprite->setVisible(true);
		m_coolSprite->runAction(final);
	}
	else if (count < 12) {
		m_excellentSprite->setScale(0.1f);
		m_excellentSprite->setVisible(true);
		m_excellentSprite->runAction(final);
	}
	else {
		m_fantasticSprite->setScale(0.1f);
		m_fantasticSprite->setVisible(true);
		m_fantasticSprite->runAction(final);
		GameScene::instance()->fire();
	}
}

void GameLayer::clearPuzzleAnimation() {
	CCFiniteTimeAction *delay;
	CCFiniteTimeAction *action1, *action2, *action3, *action4;
	CCFiniteTimeAction *seq1, *seq2, *spawn;

	action1 = CCFadeIn::create(0.01f);
	action2 = CCScaleTo::create(0.2f, 1.0f);
	delay = CCDelayTime::create(1.0f);
	seq1 = CCSequence::create(action1, action2, delay, NULL);
	action3 = CCScaleTo::create(0.5f, m_clearSpriteDisplayScale);
	action4 = CCMoveTo::create(0.5f, m_clearSpriteDisplayPosition);
	spawn = CCSpawn::create(action3, action4, NULL);
	seq2 = CCSequence::create(seq1, spawn, NULL);

	m_clearSprite->setPosition(m_clearSpriteInitPosition);
	m_clearSprite->setScale(0.1f);
	m_clearSprite->setVisible(true);
	m_clearSprite->runAction(seq2);
	GameScene::instance()->playEffect("stageclear.mp3");
}

void GameLayer::addScoreAnimation(int score, CCPoint location, float delay, float flyTime) {
	CCFiniteTimeAction *delayAction = CCDelayTime::create(delay);
	CCFiniteTimeAction *standAction1 = CCDelayTime::create(0.2f);
	CCFiniteTimeAction *standAction2 = CCDelayTime::create(0.3f);
	CCFiniteTimeAction *showAction = CCShow::create();
	CCFiniteTimeAction *callBack = CCCallFuncND::create(this,
			callfuncND_selector(GameLayer::addScore), (void*)(score));
	cocos2d::CCLabelBMFont* label = CCLabelBMFont::create(
			CCString::createWithFormat("%d", score)->getCString(),"highscore.fnt");
	label->setPosition(location);
	label->setAnchorPoint(CCPoint(0.5, 0));
	label->setVisible(false);
	this->addChild(label);
	CCFiniteTimeAction* flyAction =
			CCEaseOut::create(CCMoveTo::create(flyTime, m_scoreLabel->getPosition()), 0.5f);
	CCFiniteTimeAction* removeAction = CCRemoveSelf::create(true);
	label->runAction(CCSequence::create(delayAction, showAction, standAction1,
			flyAction, callBack, standAction2, removeAction, NULL));
}

float GameLayer::showBonusAnimation() {
	float delay = 1.5f;
	float interv = 0.45f;
	int clearedCnt = 0;
	int remain = m_puzzle->remainStars();
	while (remain > 0) {
		if (clearedCnt >= 9) {
			scheduleAnimation(ExplodeRemainStars, delay);
			clearedCnt++;
			remain = 0;
		}
		else {
			scheduleAnimation(ExplodeTailStar, delay);
			clearedCnt++;
			remain--;
		}
		bonusHintAnimation(m_puzzle->bonus(clearedCnt), delay);
		delay += interv;
	}
	return delay;
}

void GameLayer::puzzleEnd() {
	if (isStageClear(m_status.score) == false) {
		Config::instance()->clearSavedPuzzle();
		Config::instance()->updateHighScore(m_status.score);
		Config::instance()->updateHighLevel(m_status.stage);
		Config::instance()->flush();
		onGameOver(m_status.score, m_status.highScore);
		updateHighScore(m_status.score);
		m_status.lastTarget = 0;
		scheduleAnimation(GameOver);
	}
	else {
		m_status.lastTarget = m_status.target;
		onWinPazzule(m_status.stage);
		scheduleAnimation(WinPuzzle);
	}
}

void GameLayer::explodeTailStarAnimation() {
	m_puzzle->removeAndPopTailStar();
}

void GameLayer::explodeRemainStarAnimation() {
	m_puzzle->removeAndPopAllRemainStars();
}

void GameLayer::gameOverAnimation() {
	CCFiniteTimeAction *action1, *action2, *action3, *seq1;
	action1 = CCEaseSineOut::create(CCMoveTo::create(2, m_winFailSpriteDisplayPosition));
	action2 = CCDelayTime::create(1);
	action3 = CCCallFunc::create(this, callfunc_selector(GameLayer::returnToMainMenu));
	seq1 = CCSequence::create(action1, action2, action3, NULL);
	m_gameoverSprite->runAction(seq1);
	GameScene::instance()->playEffect("gameover.mp3");
}

void GameLayer::winPuzzleAnimation() {
	m_winSprite->setVisible(true);
	CCFiniteTimeAction *action1, *action2, *action3, *seq1;
	CCAction* next;
	action1 = CCEaseSineOut::create(CCMoveTo::create(2, m_winFailSpriteDisplayPosition));
	action2 = CCDelayTime::create(1.0f);
	action3 = CCEaseOut::create(CCMoveTo::create(0.5, m_winFailSpriteInitPosition), 1.0);
	seq1 = CCSequence::create(action1, action2, action3, NULL);
	m_winSprite->runAction(seq1);
	GameScene::instance()->playEffect("win.mp3");
	scheduleAnimation(NextStage, 3.5f);
}

void GameLayer::scoreHintAnimation(int count, int score) {
	m_scoreHintLabel->setCString(
			CCString::createWithFormat("%d blocks %d points", count, score)->getCString());
	m_scoreHintLabel->setVisible(true);
	CCFiniteTimeAction *action1, *action2, *action3, *action4, *seq;
	action1 = CCScaleTo::create(0.06f, 1.4f);
	action2 = CCScaleTo::create(0.06f, 0.9f);
	action3 = CCScaleTo::create(0.06f, 1.2f);
	action4 = CCScaleTo::create(0.06f, 1.0f);
	seq = CCSequence::create(action1, action2, action3, action4, NULL);
	m_scoreHintLabel->runAction(seq);
}

void GameLayer::hideScoreHintLabel() {
	m_scoreHintLabel->setVisible(false);
}

void GameLayer::bonusHintAnimation(int bonus, float delay) {
	CCFiniteTimeAction *delayAction, *updateAction;
	CCFiniteTimeAction *showAction, *initScaleAction;
	CCFiniteTimeAction *scaleAction, *standAction, *hideAction, *seq;
	delayAction = CCDelayTime::create(delay);
	updateAction = CCCallFuncND::create(this, callfuncND_selector(
			GameLayer::updateBonusHintLabel), (void*)bonus);
	showAction = CCShow::create();
	initScaleAction = CCCallFunc::create(this,
			callfunc_selector(GameLayer::scaleBonusHintLabelToZero));
	scaleAction = CCScaleTo::create(0.1f, 1.0f);
	standAction = CCDelayTime::create(0.3f);
	hideAction = CCHide::create();
	seq = CCSequence::create(delayAction, updateAction, initScaleAction,
			showAction, scaleAction, standAction, hideAction, NULL);
	m_bonusHintLabel->runAction(seq);
}

void GameLayer::showBonusHintLabel() {
	m_bonusHintLabel->setVisible(true);
}

void GameLayer::hideBonusHintLabel() {
	m_bonusHintLabel->setVisible(false);
}

void GameLayer::scaleBonusHintLabelToZero() {
	m_bonusHintLabel->setScale(0.0f);
}

void GameLayer::hideNewStageInfoLables() {
	m_newStageInfoLabel->setVisible(false);
	m_newStageTargetInfoLabel->setVisible(false);
	m_newStageTargetScoreInfoLabel->setVisible(false);
}
