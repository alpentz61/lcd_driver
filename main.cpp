
#include "Adafruit_ILI9341.h"

int main(int argc, char **argv)
{
	
	Adafruit_ILI9341 driver;
	
	if (false == driver.begin()){
		return -1;
	}
	
	driver.fillRect(10,10,10,10,ILI9341_DARKGREEN);
	
	driver.end();
	
    return 0;
}
