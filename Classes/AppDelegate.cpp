#include "AppDelegate.h"
#include "GameScene.h"
#include "BackGroundLayer.h"
#include "MainMenuLayer.h"
#include "SimpleAudioEngine.h"
#include "Config.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
}

AppDelegate::~AppDelegate() {
}

bool AppDelegate::applicationDidFinishLaunching() {
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    CCSize screenSize = CCEGLView::sharedOpenGLView()->getFrameSize();
    CCSize designSize = CCSizeMake(480, 800);
    std::vector<std::string> searchPaths;

    searchPaths.push_back("particle");
    searchPaths.push_back("sounds");
    searchPaths.push_back("fonts");
    CCLog("screenSize(%0.2f, %0.2f)", screenSize.width, screenSize.height);
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);

    CCSpriteFrameCache* cacher = CCSpriteFrameCache::sharedSpriteFrameCache();
    cacher->addSpriteFramesWithFile("batch.plist");
    CCTextureCache* texCacher = CCTextureCache::sharedTextureCache();
    texCacher->addImage("Star_1.png");
    texCacher->addImage("Star_2.png");
    texCacher->addImage("Star_3.png");
    texCacher->addImage("Star_4.png");
    texCacher->addImage("Star_5.png");
    texCacher->addImage("fire.png");
    texCacher->addImage("learning.png");
    cacher->addSpriteFrame(CCSpriteFrame::create("learning.png", CCRect(0, 0, 258, 59)), "learning.png");

    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("broken.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("click.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("fire.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("gameover.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("select.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("stageclear.mp3");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("win.mp3");

    Config::instance()->load();

    pDirector->setDisplayStats(false);
    pDirector->setAnimationInterval(1.0 / 60);

    GameScene::instance()->showMainMenu();
    pDirector->runWithScene(GameScene::scene());

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
