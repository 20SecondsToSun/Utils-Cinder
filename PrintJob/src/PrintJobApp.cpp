#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "Printer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PrintJobApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	std::string twiceSymbol(std::string __string, char __symbol);
};

void PrintJobApp::setup()
{
	printerWnd::Printer::printerLetterSize = DMPAPER_BOOTH_TWO_PHOTOS;
	int status							   = printerWnd::Printer::applySettings();
	
	string path							  = getAppPath().string()  + "\\template.bmp";		
	string photo_url					  = twiceSymbol(path, '\\') ;

	char* cstr = _strdup(photo_url.c_str());
	printerWnd::Printer::imagePath_c = cstr;

	status	  = printerWnd::Printer::print();
}

void PrintJobApp::mouseDown( MouseEvent event )
{
}

void PrintJobApp::update()
{
}

void PrintJobApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

std::string PrintJobApp::twiceSymbol(std::string __string, char __symbol)
{
	string twiceString = "";
		for (size_t  i = 0; i < __string.size(); i++)
	{
		twiceString.push_back( __string[i]);
		if (__string[i] == __symbol ) twiceString.push_back( __string[i]);
	}

	return twiceString;
}

CINDER_APP_NATIVE( PrintJobApp, RendererGl )