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
    CCSize designSize = CCSizeMake(720, 1200);
    std::vector<std::string> searchPaths;

    searchPaths.push_back("particle");
    searchPaths.push_back("sounds");
    searchPaths.push_back("fonts");
    //CCLog("screenSize(%0.2f, %0.2f)", screenSize.width, screenSize.height);
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    CCEGLView::sharedOpenGLView()->setDesignResolutionSize(designSize.width, designSize.height, kResolutionShowAll);

    CCSpriteFrameCache* cacher = CCSpriteFrameCache::sharedSpriteFrameCache();
    cacher->addSpriteFramesWithFile("batch_cn_big.plist");
    CCTextureCache* texCacher = CCTextureCache::sharedTextureCache();
    texCacher->addImage("bg.png");
    texCacher->addImage("Star_1.png");
    texCacher->addImage("Star_2.png");
    texCacher->addImage("Star_3.png");
    texCacher->addImage("Star_4.png");
    texCacher->addImage("Star_5.png");

    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("clickbutton.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("cool.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("explode.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("fail.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("selectstar.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("stageclear.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("super.ogg");
    CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("win.ogg");

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
