PopStar
======

编译步骤：
1. 下载cocos2d-x源码，放在PopStar同级目录，并且命名为cocos2d-x，完成后目录结构应该如下：
some_where
 |
 |----cocos2d-x
 |      |
 |      |----cocos2dx
 |      |----CocosDenshion
 |      |----....
 |
 |----PopStar

2. 进入PopStar/proj.android目录，执行: ./build_native.sh
3. 执行: android update project -n PopStar -p . -t android-10
4. 执行: ant debug
