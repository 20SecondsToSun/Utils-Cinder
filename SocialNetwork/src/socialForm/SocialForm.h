#pragma once

#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/ImageIO.h"
#include "cinder/Timeline.h"

/*
#include "Popup.h"
#include "Params.h"
#include "Utils.h"
#include "AssetsManager.h"

#include "VirtualKeyboard.h"*/

#include "Button.h"
#include "CinderAwesomium.h"

#include "SocialShare.h"
#include "Vkontakte.h"
#include "Facebook.h"
#include "Twitter.h"


#define   POPUP_ANIMATION_STATE 1
#define   POPUP_READY_STATE		2
#define   POPUP_INIT_STATE		3

#define   SERVER_STATUS_NONE 1
#define   SERVER_STATUS_POSTING	2
#define   SERVER_STATUS_POST_READY	3
#define   SERVER_STATUS_POST_ERROR	4
#define   SERVER_STATUS_USER_REJECT 5




#define concat(first, second) first second

class SocialForm
{
	public:
		
		void								setup( ci::app::WindowRef window);			
		void								draw( );
		void								update();
		void								show(int mode);	
		void								hide();	

		boost::signals2::signal<void(void)> formClosed;

	private:

		ci::app::WindowRef					mainWindow;
		
		ci::Anim<ci::ColorA>				errorAlpha, bgColor;

	
		void								KeyDown( ci::app::KeyEvent event  );
		void								MouseDown( ci::app::MouseEvent &event );
		void								MouseUp( ci::app::MouseEvent &event );

		void								initHandlers();
		void								removeHandlers();
		void								closedHandler();
		void								keyboardTouchSignal();
	
		ci::signals::connection				KeyDownCon, MouseDownCon, MouseUpCon;

		ci::gl::Texture						closeEmailTex;		
		Button								closePopup;	

		static Awesomium::WebCore*			mWebCorePtr;
		static Awesomium::WebView*			mWebViewPtr;
		static Awesomium::WebSession*		session;
		ci::gl::Texture						mWebTexture;

		int									popupMode;
	
		ci::Vec2f							_vkontakteOffset;
		ci::Vec2f							_facebookOffset;

		int									popupAnimationState;		
		int									socialServerStatus;

		std::string							postingWaitingText;

		int									_facebookWindowHeight, _facebookWindowWidth;

		Area								facebookPopupAvailableArea, vkontaktePopupAvailableArea;

		SocShare							*social;
		void								socialServerSignal();
		ci::signals::connection				socialServerSignalCon;

		std::vector<std::string>			photoURLs;

		//	static ci::app::KeyEvent   imitate_ENTER_KEY_EVENT();
		//static ci::app::KeyEvent   imitate_BACKSPACE_KEY_EVENT();
		static ci::app::MouseEvent inititateMouseEvent(ci::Vec2f _vec);
};