#include "stdafx.h"

int main(){
	Dump dmp;
	sApp.Initialize();
	while (sApp.GetRun())
	{
		sApp.Update();
		Threader::sleep(1);
	}
	sApp.Destroy();
	return 0;
}