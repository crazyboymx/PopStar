/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/
package com.crazyboy.popstar;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.os.Bundle;
import android.net.Uri;
import android.content.Intent;
import android.content.ActivityNotFoundException;
import android.app.Activity;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.ViewGroup;
import android.media.AudioManager;
import android.content.Context;
import android.widget.FrameLayout;
import android.widget.Toast;
import android.util.Log;

import com.adsmogo.adapters.AdsMogoAdapter;
import com.adsmogo.adapters.AdsMogoCustomEventPlatformEnum;
import com.adsmogo.adview.AdsMogoLayout;
import com.adsmogo.controller.listener.AdsMogoListener;
import com.adsmogo.util.AdsMogoUtil;

public class PopStar extends Cocos2dxActivity implements AdsMogoListener {

	private static final String id = "52d8a4678feb471e8432160d99e65dc6";
	private static Activity activity2;

	private static native void nativeInit(String id);
	private static native void nativeHit();
	
	protected AdsMogoLayout adsMogoLayout;
	protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		initAds();
		nativeInit(id);
		activity2 = this;
	}

    static {
        System.loadLibrary("cocos2dcpp");
    }
    
    protected void initAds() {
    	//com.adsmogo.util.L.debug=true;
    	adsMogoLayout = new AdsMogoLayout(this, id, true);
    	FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(
    	    FrameLayout.LayoutParams.FILL_PARENT,
    	    FrameLayout.LayoutParams.WRAP_CONTENT);

    	// 设置广告出现的位置(悬浮于顶部).
    	params.bottomMargin = 0;
    	adsMogoLayout.setAdsMogoListener(this);
    	adsMogoLayout.setFocusable(false);
    	params.gravity = Gravity.TOP;
    	addContentView(adsMogoLayout, params);
    }
    
    private void winPuzzle(int stage) {
    	if (stage % 8 == 0) {
    	}
    }

    private void gameOver(int score, int highScore) {

    }
    
    private void rate() {
    	Uri uri = Uri.parse("market://details?id=" + getContext().getPackageName());
    	Intent goToMarket = new Intent(Intent.ACTION_VIEW, uri);
		try {
			activity2.startActivity(goToMarket);
		} catch (ActivityNotFoundException e) {
		}
    }

    private void onStageCleared() {
    }

    @Override
    public boolean onKeyDown(final int keyCode, KeyEvent event) {
    	AudioManager mAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
    	int currentVolume = mAudioManager.getStreamVolume(AudioManager.STREAM_MUSIC);
    	switch (keyCode) {
    	case KeyEvent.KEYCODE_VOLUME_UP:
    		mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, currentVolume+1, 1);
    		return true;
    	case KeyEvent.KEYCODE_VOLUME_DOWN:
    		mAudioManager.setStreamVolume(AudioManager.STREAM_MUSIC, currentVolume-1, 1);
    		return true;
    	default:
    		return super.onKeyDown(keyCode, event);
    	}
    }
    
	public void onClickAd(String arg0) {
		//Log.d(AdsMogoUtil.ADMOGO, "-=onClickAd=-");
	}

	public boolean onCloseAd() {
		//Log.d(AdsMogoUtil.ADMOGO, "-=onCloseAd=-");
		return true;
	}
    
	public void onCloseMogoDialog() {
		//Log.d(AdsMogoUtil.ADMOGO, "-=onCloseMogoDialog=-");
	}
    
	public void onFailedReceiveAd() {
		//Log.d(AdsMogoUtil.ADMOGO, "-=onFailedReceiveAd=-");
	}
    
	public void onRealClickAd() {
		nativeHit();
	}
    
	public void onReceiveAd(ViewGroup arg0, String arg1) {
		//Log.d(AdsMogoUtil.ADMOGO, "-=onReceiveAd=-");
	}
    
	public void onRequestAd(String arg0) {
		//Log.d(AdsMogoUtil.ADMOGO, "-=onRequestAd=-");
	}
    
    @Override
	protected void onDestroy() {
		// 清除 adsMogoLayout 实例 所产生用于多线程缓冲机制的线程池 
		// 此方法请不要轻易调用，如果调用时间不当，会造成无法统计计数 
		if (adsMogoLayout != null) {
			adsMogoLayout.clearThread();
		}
		super.onDestroy();
	}
    
	public Class<? extends AdsMogoAdapter> getCustomEvemtPlatformAdapterClass(
			AdsMogoCustomEventPlatformEnum enumIndex) {
		return null;
	}
}
