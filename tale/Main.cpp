#include"Tale.h"
int main() {
	Tale *app = new Tale(640,1024, GetModuleHandle(NULL));
	if (app->InitApp()) {
		if (app->InitD2d())
		{
			if (app->initRootScene()) {
				if (app->ShowWindow(SW_SHOW)) {
					app->StartMessageLoop();
				}
			}
		}
		
	}
}