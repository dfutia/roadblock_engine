#pragma once
#ifndef EVENT_H
#define EVENT_H

#include <functional>
#include <vector>
#include <memory>

template<typename... TArgs>
class Event {
private:
    struct Connection {
        std::function<void(TArgs...)> m_callback;
        bool m_connected;
        Connection(std::function<void(TArgs...)> callback) : m_callback(std::move(callback)), m_connected(true) {}
    };

    std::vector<std::weak_ptr<Connection>> m_connections;
public:
    class ConnectionHandle {
    private:
        std::shared_ptr<Connection> m_connection;
    public:
        ConnectionHandle() = default;

        explicit ConnectionHandle(std::shared_ptr<Connection> connection) : m_connection(std::move(connection)) {}

        void Disconnect() {
            if (m_connection) {
                m_connection->m_connected = false;
                m_connection.reset();
            }
        }
    };

    ConnectionHandle Connect(std::function<void(TArgs...)> callback) {
        auto connection = std::make_shared<Connection>(std::move(callback));
        m_connections.push_back(connection);
        return ConnectionHandle(connection);
    }

    void Fire(TArgs... args) {
        m_connections.erase(
            std::remove_if(m_connections.begin(), m_connections.end(),
                [&](const std::weak_ptr<Connection>& weak_conn) {
                    if (auto conn = weak_conn.lock()) {
                        if (conn->m_connected) {
                            try {
                                conn->m_callback(args...);
                            }
                            catch (...) {
                                // Handle or rethrow exception
                            }
                            return false;
                        }
                    }
                    return true;
                }
            ),
            m_connections.end()
        );
    }

    ~Event() {
        for (auto& weak_conn : m_connections) {
            if (auto conn = weak_conn.lock()) {
                conn->m_connected = false;
            }
        }
        m_connections.clear();
    }
};
/*
void onEvent(int number, std::string text) {
	std::cout << "Listener 1: " << number << ", " << text << std::endl;
}

// Creating event
Event<int, std::string> myEvent;

// Connecting to event
auto myListener = myEvent.Connect(onEvent);

Event<int, std::string>::ConnectionHandle myListener.Connect(onEvent);

auto myOtherListener = myEvent.Connect([](int num, const std::string& str) {
	std::cout << "Listener 2: " << num << ", " << str << std::endl;
});

myEvent.Fire(21, "Dominic");

myListener.Disconnect();
myOtherListener.Disconnect();
*/

#endif 