#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "SocialForm.h"
#include "SocialShare.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SocialNetworkApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

private:
	void formClosed();
	SocialForm			socialForm;
};

void SocialNetworkApp::setup()
{
	setWindowSize(1000, 800);

	socialForm.setup(getWindow());

	socialForm.formClosed.connect( 
		boost::bind(&SocialNetworkApp::formClosed, this) 
		);

	socialForm.show(SOCIAL_FACEBOOK);//SOCIAL_FACEBOOK//SOCIAL_TWITTER

}

void SocialNetworkApp::mouseDown( MouseEvent event )
{
}

void SocialNetworkApp::update()
{
	socialForm.update();
}

void SocialNetworkApp::draw()
{	
	gl::clear( Color( 0, 0, 0 ) ); 
	socialForm.draw();
}

void SocialNetworkApp::formClosed(  )
{

}

CINDER_APP_NATIVE( SocialNetworkApp, RendererGl )