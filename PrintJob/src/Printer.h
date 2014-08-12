#include "cinder/app/AppNative.h"
#include <winspool.h>
//#define DMPAPER_BOOTH_TWO_PHOTOS               515    /* Letter  5x15 type 2              */
#define DMPAPER_BOOTH_ONE_PHOTO  501  /* Letter  10x15(4x6'')         */
#define DMPAPER_BOOTH_TWO_PHOTOS  514  /* Letter  10x15(4x6'')         */

namespace printerWnd 
{
	class Printer
	{
		public:
			Printer(){};
			~Printer(){};

		
			static char * imagePath_c ;
			static short printerLetterSize;
			static short printerLetterOrientation;
		
			static int applySettings();
			static int print();

			static void setPathToPrintImage(LPWSTR value);

		private:
			static LPWSTR printerName;
			static LPWSTR imagePath;
	};
}