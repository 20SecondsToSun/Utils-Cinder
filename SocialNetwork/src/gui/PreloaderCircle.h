#pragma once
#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/gl.h"
#include "cinder/Text.h"

using namespace std;

class PreloaderCircle
{
	public:		
		static void draw();
		static void loadTexture(ci::gl::Texture tex);	
		static ci::gl::Texture preloader;	
		static void setup();
};