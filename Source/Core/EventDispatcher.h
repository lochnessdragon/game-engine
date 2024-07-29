#pragma once

#include <vector>
#include <functional>

template<typename T>
class EventDispatcher {
private:
	typedef bool CallbackFunc(const T&);
	std::vector<std::function<CallbackFunc>> Callbacks;
public:

	void Dispatch(const T& eventData) {
		bool handled = false;
		size_t idx = 0;
		while (!handled && idx < this->Callbacks.size()) {
			auto listener = this->Callbacks[idx];
			handled = listener(eventData);
			idx++;
		}
	}

	void AddListener(std::function<CallbackFunc> callback) { Callbacks.push_back(callback); };
};