#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <math.h>

#include "MyRio.h"
#include "I2C.h"
#include "Motor_Controller.h"
#include "Utils.h"

using namespace std;

extern NiFpga_Session myrio_session;

NiFpga_Status status;

int main(int argc, char **argv)
{
	status = MyRio_Open();
	if (MyRio_IsNotSuccess(status))
	{
		return status;
	}

	MyRio_I2c i2c;
	status = Utils::setupI2CB(&myrio_session, &i2c);

	Motor_Controller mc = Motor_Controller(&i2c);
	mc.controllerEnable(DC);
	mc.controllerEnable(SERVO);

	int volt = mc.readBatteryVoltage(1);
	printf("%d\n\n", volt);

	int speed = 300;
	int delay = 2;

	int leftCount = 0;
	int rigthCount = 0;

	for(int i=0; i<4; i++){
		leftCount += 360;
		rigthCount -= 360;
		mc.setMotorDegrees(DC, speed, leftCount, speed, rigthCount);
		Utils::waitFor(delay);

		rigthCount -= 450;
		mc.setMotorDegrees(DC, 0, leftCount, speed, rigthCount);
		Utils::waitFor(delay);
	}
	Utils::waitFor(2);


	mc.controllerReset(DC);
	mc.controllerReset(SERVO);

	status = MyRio_Close();

	return status;

	return 0;
}

