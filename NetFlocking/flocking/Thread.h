#ifndef _THREAD_H_
#define _THREAD_H_

// Certificate of Authenticity
//
// EGP-405-01 Networking for Online Games
// Project 3
// 8-27-2018
//
// Vedant Chaudhari, 1532077
// John Malvey, 1005854
//
// We certify that this work is entirely our own.The assessor of this project may reproduce this project 
// and provide copies to other academic staff, and/or communicate a copy of this project to a plagiarism 
// - checking service, which may retain a copy of the project on its database.

#ifdef __cplusplus
extern "C" {
#else	//__cplusplus
// struct foo
// foo bar; // C++ instance
// struct foo bar // C instance
typedef struct egpThread egpThread;
#endif	//__cplusplus

// Our definition of an acceptable function
// Any function that returns int and accepts any ptr param
typedef int(*egpThreadFunc)(void*);

struct egpThread {
	void* handle;
	unsigned int ID;

	int result;
	int isRunning;

	egpThreadFunc func;
	void* args;
};

int egpThreadCreate(egpThread* pThread_out, egpThreadFunc func, void* args);
int egpThreadTerminate(egpThread* pThread);

#ifdef __cplusplus
}
#else	//__cplusplus
#endif	//__cplusplus

#endif	// !_THREAD_H_