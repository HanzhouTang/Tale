#pragma once
#include<Windows.h>
#include<tchar.h>
#include<string>
#include<iostream>
#include <d2d1.h>
#include<wincodec.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include<DirectXMath.h>
#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }