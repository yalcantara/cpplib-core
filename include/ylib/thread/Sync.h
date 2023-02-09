#pragma once
#include <mutex>

namespace ylib {
namespace tread {
class Sync {
private:
	std::mutex& mtx;

public:

	Sync(std::mutex& mtx) :
		mtx(mtx) {
		mtx.lock();
	}

	//Rule of five
	//=========================================================================
	//1. Copy Constructor
	//No copy constructor allowed
	Sync(const Sync&) = delete;

	//2. Copy Assignment
	//no copy assignment allowed
	Sync& operator=(const Sync& other) = delete;

	//3. Move Constructor
	//No move constructor allowed
	Sync(Sync&& other) = delete;

	//4. Move Assignment
	//No move assignment allowed
	Sync& operator=(Sync&& other) = delete;

	//5. Destructor
	virtual ~Sync() {
		mtx.unlock();
	}
	//=========================================================================
};
}
}