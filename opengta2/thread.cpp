#include "opengta.h"

Thread_Manager Thread;
//FIXME: use glfw threads and mutexes

void Thread_Manager::Initialize() {

}

void Thread_Manager::Deinitialize() {

}

ThreadID Thread_Manager::Create(ThreadFunction* funcptr, void* param) {
	ThreadID th = new std::thread(funcptr,param);
	logWrite("Spawned thread [%p] (ThreadID[0x%x])",funcptr,th);
	return th;
}

ThreadID Thread_Manager::GetCurrentThreadID() {
    return nullptr;
}

void Thread_Manager::WaitForThread(ThreadID threadID) {
    threadID->join();
}

void Thread_Manager::Kill(ThreadID threadID) {
	logWrite("Killed thread (ThreadID[0x%x])",threadID);
    threadID->join(); // shouldn't actually KILL a thread!
}

void Thread_Manager::Sleep(float secs) {
    std::this_thread::sleep_for(std::chrono::seconds(static_cast<size_t>(secs)));
}



LockID Thread_Manager::EnterLock(int mutexID) {
    mutexLocks[mutexID].lock();
	return mutexID;
}

void Thread_Manager::LeaveLock(LockID lockID) {
    mutexLocks[lockID].unlock();
}

void Thread_Manager::WaitForLock(int mutexID) {
	Thread.LeaveLock(Thread.EnterLock(mutexID));
}
