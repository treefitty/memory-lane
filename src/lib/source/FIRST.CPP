#define Uses_TApplication
#include <tv.h>

class TFirstApp : public TApplication
{
public :
     TFirstApp();
};

TFirstApp::TFirstApp() :
    TProgInit( &TFirstApp::initStatusLine,
	       &TFirstApp::initMenuBar,
	       &TFirstApp::initDeskTop
	     )
{
}
int main()
{
	TFirstApp demoApp;
	demoApp.run();
	return 0;
}