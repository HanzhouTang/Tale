#include"Tale.h"
int main() {
	Tale *app = new Tale(640,1024, GetModuleHandle(NULL));
	if (app->InitApp()) {
		if(app->InitDirectX())
		{
			wstring content = app->readFile(L"Resource\\xml\\test.xml");
			app->parse(content);
			if (app->initRootScene()) {
				if (app->ShowWindow(SW_SHOW)) {
					app->StartMessageLoop();
				}
			}
		}
		
	}
}