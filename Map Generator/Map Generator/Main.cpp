#include <iostream>
#include "Application.h"

int main()
{
	std::srand(std::time(0));
	Application app;
	return app.Start();
}