#include "SocialForm.h"
#include <ctype.h>
#include <boost/algorithm/string.hpp>

using namespace ci;
using namespace ci::app;
using namespace std;

Awesomium::WebCore*		 SocialForm::mWebCorePtr;
Awesomium::WebView*		 SocialForm::mWebViewPtr;
Awesomium::WebSession*   SocialForm::session;

void SocialForm::setup( ci::app::WindowRef window)
{
	mainWindow = window;	

	// set Awesomium logging to verbose
	Awesomium::WebConfig cnf;
	cnf.log_level = Awesomium::kLogLevel_None;
	Awesomium::WebPreferences pref;	
	// initialize the Awesomium web engine
	mWebCorePtr		= Awesomium::WebCore::Initialize( cnf );	
	session			= mWebCorePtr->CreateWebSession(Awesomium::WSLit("soc"), pref);
	mWebViewPtr		= mWebCorePtr->CreateWebView( getWindowWidth(), getWindowHeight(), session );	

	_vkontakteOffset			= Vec2f(0.0f, 0);
	vkontaktePopupAvailableArea = Area(0,  0, getWindowWidth(), getWindowHeight());
	facebookPopupAvailableArea  = Area(0,  0, getWindowWidth(), getWindowHeight());

	_facebookWindowHeight = 500;	
	_facebookWindowWidth  = 700;

	photoURLs.clear();

	for (int i = 1; i < 4; i++)
	{
		string num_s = to_string(i);

		string photo_url = "";
		string path = (getAppPath()/"data").string();	
	
		path = path + "\\PHOTO_"+ num_s+ ".jpg";		
		
		for (size_t i = 0; i < path.size(); i++)
		{
			photo_url.push_back( path[i]);
			if (path[i] == '\\' ) photo_url.push_back( path[i]);
		}

		photoURLs.push_back(photo_url);		
	}
}

void SocialForm::show(int mode)
{
	popupMode = mode;
	popupAnimationState = POPUP_ANIMATION_STATE;
	socialServerStatus  = SERVER_STATUS_NONE;	
	session->ClearCookies();
	
	initHandlers();

	if (popupMode == SOCIAL_VKONTAKTE)
	{		
		social = new Vkontakte();		
		social->clear_token();	
		socialServerSignalCon = social->serverHandler.connect( 
			boost::bind(&SocialForm::socialServerSignal, this) 
		);		

		postingWaitingText = "Отправляем фотографии во ВКонтакте..";
	
		if( mWebViewPtr )
			mWebViewPtr->Resize(getWindowWidth(), getWindowHeight() );
	
		console()<<" auth url :: "<<social->getAuthUrl()<<std::endl;
		mWebViewPtr->LoadURL( Awesomium::WebURL( Awesomium::WSLit( social->getAuthUrl()) ) );
		mWebViewPtr->Focus();
	}
	else if (popupMode == SOCIAL_FACEBOOK)
	{		
		social = new Facebook();	
		social->clear_token();
		socialServerSignalCon = social->serverHandler.connect( 
			boost::bind(&SocialForm::socialServerSignal, this) 
		);
	
		postingWaitingText = "Отправляем фотографии в Facebook..";
	
		if( mWebViewPtr )
			mWebViewPtr->Resize(getWindowWidth(), getWindowHeight() );

		_facebookOffset = Vec2f(0, 0);
	
		mWebViewPtr->LoadURL( Awesomium::WebURL( Awesomium::WSLit( social->getAuthUrl()) ) );
		mWebViewPtr->Focus();
	}
	else if (popupMode == SOCIAL_TWITTER)
	{	
		social = new Twitter();			
		social->clear_token();
		socialServerSignalCon = social->serverHandler.connect( 
			boost::bind(&SocialForm::socialServerSignal, this) 
		);

		/*string login = "login", password = "pass";
		string message = "твитератест1";
		social->setLoginPassword(login, password);
		//social->postStatus(message);
		string url = "PHOTO_1.jpg";
		social->postPhoto(url, message);
		*/	
	

	}
}
void SocialForm::hide()
{
	closedHandler();
}

void SocialForm::socialServerSignal()
{
	if (social->getResponse() == "OK")
		socialServerStatus = SERVER_STATUS_POST_READY;
	else 
		socialServerStatus = SERVER_STATUS_POST_ERROR;
}


void SocialForm::initHandlers()
{
	popupAnimationState = POPUP_READY_STATE;
	
	MouseDownCon   = mainWindow->getSignalMouseDown().connect( std::bind( &SocialForm::MouseDown, this, std::_1 ) );
	MouseUpCon	   = mainWindow->getSignalMouseUp().connect(   std::bind( &SocialForm::MouseUp,   this, std::_1 ) );
	KeyDownCon	   = mainWindow->getSignalKeyDown().connect(   std::bind( &SocialForm::KeyDown,	  this, std::_1 ) );		
}

void SocialForm::removeHandlers()
{
	socialServerSignalCon.disconnect();
	delete social;

	MouseUpCon.disconnect( );
	MouseDownCon.disconnect();
	KeyDownCon.disconnect();
}

void SocialForm::closedHandler()
{
	formClosed();
}

void SocialForm::MouseDown( MouseEvent &event )
{
	if (SERVER_STATUS_POSTING == socialServerStatus)
	{
		return;
	}
	else if (popupMode ==  SOCIAL_VKONTAKTE && 
			 vkontaktePopupAvailableArea.contains(event.getPos()))
	{			
		MouseEvent mEvent = inititateMouseEvent(event.getPos() -_vkontakteOffset);
		ph::awesomium::handleMouseDown( mWebViewPtr, mEvent );		
	}
	else if (popupMode ==  SOCIAL_FACEBOOK && 
			 facebookPopupAvailableArea.contains(event.getPos()))
	{	
		MouseEvent mEvent = inititateMouseEvent(event.getPos() -_facebookOffset);
		ph::awesomium::handleMouseDown( mWebViewPtr, mEvent );	
	}
}

void SocialForm::MouseUp( MouseEvent &event )
{
	if (popupMode ==  SOCIAL_VKONTAKTE && 
		vkontaktePopupAvailableArea.contains(event.getPos()))
	{					
		MouseEvent mEvent = inititateMouseEvent(event.getPos() -_vkontakteOffset);
		ph::awesomium::handleMouseUp( mWebViewPtr, mEvent );	
	}
	else if (popupMode ==  SOCIAL_FACEBOOK && 
			facebookPopupAvailableArea.contains(event.getPos()))
	{		
		MouseEvent mEvent = inititateMouseEvent(event.getPos() -_facebookOffset);
		ph::awesomium::handleMouseUp( mWebViewPtr, mEvent );		
	}
}
void SocialForm::KeyDown( KeyEvent event )
{	
	ph::awesomium::handleKeyDown( mWebViewPtr, event );
}

void SocialForm::update()
{
	if (socialServerStatus == SERVER_STATUS_USER_REJECT) return;

	// update the Awesomium engine
	if(mWebCorePtr)	mWebCorePtr->Update();

	// create or update our OpenGL Texture from the webview
	if(mWebViewPtr && ph::awesomium::isDirty( mWebViewPtr ) ) 
	{
		try {
			// set texture filter to NEAREST if you don't intend to transform (scale, rotate) it
			gl::Texture::Format fmt; 
			fmt.setMagFilter( GL_NEAREST );

			// get the texture using a handy conversion function
			mWebTexture = ph::awesomium::toTexture( mWebViewPtr, fmt );
			//console()<<"UPDATE TEXTURE!!"<<std::endl;
		}
		catch( const std::exception &e ) {
			console() << e.what() << std::endl;
		}

		// update the window title to reflect the loaded content
		char title[1024];
		mWebViewPtr->title().ToUTF8( title, 1024 );
	}

	if(mWebViewPtr)
	{
		if (popupMode ==  SOCIAL_FACEBOOK)
		{		
			char anchr[1024];
			mWebViewPtr->url().anchor().ToUTF8( anchr, 1024 );
			std::string anchString( anchr );
			std::size_t pos = anchString.find("access_token"); 			

			if (pos == 0 && socialServerStatus == SERVER_STATUS_NONE) 	
			{	
				std::string delimiter		= "&";
				std::string token			= anchString.substr(0, anchString.find(delimiter)); 
				social->access_token		=  token.substr(13);				
				socialServerStatus			= SERVER_STATUS_POSTING;		
			
				//social->postPhoto(photoURLs, "#testtest");
				social->postStatus( "#testtest");
			}	
			else
			{
				char query[1024];
				mWebViewPtr->url().query().ToUTF8( query, 1024 );
				std::string queryString( query );
				std::size_t pos_denied = queryString.find("error_reason=user_denied"); 
				if (pos_denied <1000)
				{
					socialServerStatus = SERVER_STATUS_USER_REJECT;
					removeHandlers();
					hide();				
				}
			}			
		}	
		else if (popupMode ==  SOCIAL_VKONTAKTE)
		{
			char anchr[1024];
			mWebViewPtr->url().anchor().ToUTF8( anchr, 1024 );
			std::string anchString( anchr );
			std::size_t pos = anchString.find("access"); 
			
			if (pos==0 && socialServerStatus == SERVER_STATUS_NONE) 	
			{	
				std::string delimiter = "&";
				std::string token = anchString.substr(0, anchString.find(delimiter));
				social->access_token = token.substr(13);
				socialServerStatus  = SERVER_STATUS_POSTING;

				//social->postStatus( "#тест");
				social->postPhoto(photoURLs, "#test");
			}
			else 
			{
				std::size_t pos_denied = anchString.find("User denied your request"); 
				if (pos_denied <1000)
				{
					socialServerStatus = SERVER_STATUS_USER_REJECT;
					removeHandlers();
					hide();
				}			
			}
		}	
	}
}

void SocialForm::draw()
{
	if (SERVER_STATUS_POSTING == socialServerStatus)
	{		
		//PopupMail::draw("SERVER_LOADING", postingWaitingText);
		return;
	}
	else if (SERVER_STATUS_POST_READY == socialServerStatus)
	{
		//PopupMail::draw("SERVER_OK");
		return;
	}
	else if (SERVER_STATUS_POST_ERROR == socialServerStatus)
	{
		//PopupMail::draw("SERVER_ERROR");
		return;
	}	

	if( popupAnimationState == POPUP_READY_STATE && mWebTexture && mWebViewPtr->IsLoading() == false )
	{		
		gl::pushMatrices();		
			gl::draw( mWebTexture );
		gl::popMatrices();		
	}
	else
	{
		gl::pushMatrices();			
			gl::translate(950.0f, 674.0f+150.0f);
			//PreloaderCircle::draw();
		gl::popMatrices();
	}
}


MouseEvent SocialForm::inititateMouseEvent(Vec2f _vec)
{
	unsigned int k = 1;
	float r = 1.0f;
	uint32_t t = 1;
	return MouseEvent(getWindow(), 1, _vec.x  , _vec.y , k,  r,  t);
}