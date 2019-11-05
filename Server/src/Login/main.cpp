#include "stdafx.h"
void main()
{
	Dump dmp;
	INSTANCE(Application).Initialize();
	while (true)
	{
		INSTANCE(Application).Update();
		Threader::sleep(1);
	}
	INSTANCE(Application).Destroy();
}