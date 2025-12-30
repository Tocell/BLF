#ifndef TEST_H
#define TEST_H

#ifdef _WIN32
	#ifdef CANCONTROL_EXPORTS
		#define CANCONTROL_API __declspec(dllexport)
	#else
		#define CANCONTROL_API __declspec(dllimport)
	#endif
#else
	#define CANCONTROL_API __attribute__((visibility("default")))
#endif

CANCONTROL_API int addition(int a, int b);

#endif //TEST_H
