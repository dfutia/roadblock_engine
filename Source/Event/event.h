#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <vector>
#include <memory>
#include <mutex>

template<typename... Args>
class Event {
private:
	struct Connection {
		std::function<void(Args...)> m_callback;
		bool m_connected;

		Connection(std::function<void(Args...)> callback) : m_callback(std::move(callback)), m_connected(true) {}
	};
	std::vector<std::unique_ptr<Connection>> m_connections;
	mutable std::recursive_mutex mutex;
public:
	class ConnectionHandle {
	private:
		Event* m_event;
		typename std::vector<std::unique_ptr<Connection>>::iterator it;
	public:
		ConnectionHandle() : m_event(nullptr) {}

		ConnectionHandle(Event* event, typename std::vector<std::unique_ptr<Connection>>::iterator it)
			: m_event(event), it(it) {}

		void Disconnect() {
			if (m_event) {
				std::lock_guard<std::recursive_mutex> lock(m_event->mutex);
				(*it)->m_connected = false;
				m_event = nullptr;
			}
		}

		ConnectionHandle& operator=(const ConnectionHandle& other) {
			m_event = other.m_event;
			it = other.it;
			return *this;
		}
	};

	ConnectionHandle Connect(std::function<void(Args...)> callback) {
		std::lock_guard<std::recursive_mutex> lock(mutex);
		m_connections.push_back(std::make_unique<Connection>(std::move(callback)));
		return ConnectionHandle(this, std::prev(m_connections.end()));
	}

	void Fire(Args... args) {
		std::lock_guard<std::recursive_mutex> lock(mutex);
		for (auto it = m_connections.begin(); it != m_connections.end();) {
			if ((*it)->m_connected) {
				try {
					(*it)->m_callback(args...);
				}
				catch (...) {
					// Handle or rethrow exception
				}
				++it;
			}
			else {
				it = m_connections.erase(it);
			}
		}
	}
};

/*
void onEvent(int number, std::string text) {
	std::cout << "Listener 1: " << number << ", " << text << std::endl;
}

Event<int, std::string> myEvent;

auto myListener = myEvent.Connect(onEvent);

auto myOtherListener = myEvent.Connect([](int num, const std::string& str) {
	std::cout << "Listener 2: " << num << ", " << str << std::endl;
});

myEvent.Fire(21, "Dominic");

myListener.Disconnect();
myOtherListener.Disconnect();
*/

#endif 
