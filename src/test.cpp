#include "test.h"
#include "log.h"

int addition(int a, int b)
{
	LOG_INFO("addition 100 + 200 = {}", 100 + 200);
	return a + b;
}