#include "stdafx.h"


void main()
{
	bool fType = true;
	Dump dump;
	srand(time(NULL));
	INSTANCE(ConfigManager).reloadConfig();

	INSTANCE(RobotManager);
	INSTANCE(SocketHandler);

	double time = DateTime::GetNowAppUS();
	double delay = 0.0;
	double pingTime = 0.0;

	AES aes(sKey);

	char bytes[] = {
		0x00, 0x00, 0x00, 0x04, 
		0x00, 0x00, 0x00, 0x01, 
		0x00, 0x05, 0x75, 0x73,  
		0x65, 0x72, 0x31, 0x00, 
		0x00
	};
	aes.Cipher(bytes, 17);
	aes.InvCipher(bytes, 17);

	while (1)
	{
		delay = DateTime::GetNowAppUS() - time;
		time += delay;
		time = DateTime::GetNowAppUS();
		pingTime += delay;

		INSTANCE(Network).update(0.f, 0.f);
		INSTANCE(RobotManager).update(time, delay);
		Threader::sleep(1);
	}
}