#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include "Editor/editorpanel.h"
#include "Event/engineevents.h"

#include <imgui.h>

#include <string>

enum class MessageType {
    Info,
    Warning,
    Error
};

struct ConsoleMessage {
    std::string message;
    MessageType type;
    ConsoleMessage(const std::string& msg, MessageType t) : message(msg), type(t) {}
};

class Console : public EditorPanel {
public:
    Console() : EditorPanel(true, "Console") {
        luaMessageConnection = EngineEvents::LuaMessageEvent.Connect([this](const std::string& message, LuaMessageType type) {
            addMessage(message, type);
        });
    }

    ~Console() {
        luaMessageConnection.Disconnect();
    }

    void render() override {
        ImGui::Begin("Console", &m_open);

        // Add a clear button
        if (ImGui::Button("Clear")) {
            messages.clear();
        }
        ImGui::SameLine();

        // Add a filter
        filter.Draw("Filter", -100.0f);

        ImGui::Separator();

        ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        for (const auto& msg : messages) {
            if (!filter.PassFilter(msg.message.c_str())) {
                continue;
            }

            ImVec4 color;
            switch (msg.type) {
            case MessageType::Info:    color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); break;
            case MessageType::Warning: color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); break;
            case MessageType::Error:   color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); break;
            }

            ImGui::PushStyleColor(ImGuiCol_Text, color);
            ImGui::TextUnformatted(msg.message.c_str());
            ImGui::PopStyleColor();
        }

        ImGui::EndChild();
        ImGui::End();
    }
private:
    std::vector<ConsoleMessage> messages;
    ImGuiTextFilter filter;
    Event<const std::string&, LuaMessageType>::ConnectionHandle luaMessageConnection;

    void addMessage(const std::string& message, LuaMessageType type) {
        MessageType consoleType;
        switch (type) {
        case LuaMessageType::Info:
            consoleType = MessageType::Info;
            break;
        case LuaMessageType::Warning:
            consoleType = MessageType::Warning;
            break;
        case LuaMessageType::Error:
            consoleType = MessageType::Error;
            break;
        }
        messages.emplace_back(message, consoleType);
    }
};

#endif 