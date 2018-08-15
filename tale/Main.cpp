#include"Tale.h"
#include<gtest/gtest.h>



int main(int argc, char* argv[]) {

	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	Tale *app = new Tale(640,1024, GetModuleHandle(NULL));
	if (app->InitApp()) {
		if(app->InitDirectX())
		{
			
			if (app->initRootScene()) {
				if (app->ShowWindow(SW_SHOW)) {
					app->StartMessageLoop();
				}
			}
		}
		
	}
}