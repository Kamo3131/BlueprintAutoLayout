#include "AutoLayout.hpp"
#include <iostream>
#include <functional>
#include <cmath>

AutoLayout::AutoLayout(std::string file_path) {
    loadDataFromFile(file_path+"/input.json");
    loadFontFromFile(file_path+"/roboto/Roboto-Black.ttf");
}
void AutoLayout::loadDataFromFile(std::string file_path) {
    std::ifstream file(file_path, std::ifstream::binary);
    if(!file.is_open()) {
        std::cout << "File on path " << file_path << " did no open!";
        exit(1);
    }
    Json::Value root;
    file >> root;
    const Json::Value nodes = root["nodes"];
    const Json::Value edges = root["edges"];
    if (nodes.isArray()) {
        for (const auto& node : nodes) {
            int id = node["id"].asInt();
            Node node_data;
            node_data.id = id;
            std::string name = node["name"].asString();
            std::string value = valueOfName(name);
            node_data.name = name;
            node_data.value = value;
            node_data.x = node["x"].asInt();
            node_data.y = node["y"].asInt();
            m_data[id] = node_data;
        }
        for(const auto& edge : edges) {
            int from_id = edge["from"].asInt();
            int to_id = edge["to"].asInt();

            if (m_data.count(from_id)) {
                m_data[from_id].to.push_back(to_id);
                m_data[to_id].from.push_back(from_id);
            }
        }
    }
    file.close();
}

std::string AutoLayout::valueOfName(std::string & name) {
    const std::size_t op_paren = name.find("(");
    if(std::string::npos == op_paren) {
        return "";
    }
    const std::size_t cl_paren = name.find(")");
    if(std::string::npos == cl_paren) {
        return "";
    }
    std::string value = name.substr(op_paren+1, cl_paren - op_paren - 1);
    name.erase(op_paren, std::string::npos);
    return value;
}
void AutoLayout::loadFontFromFile(std::string file_path) {
    if (!m_font.openFromFile(file_path)) {
        std::cout << "Font on path " << file_path << " not found!\n";
    }
}
void AutoLayout::printDataContents() const {
    for (const auto& [id, node] : m_data) {
        std::cout << "ID [" << id << "]: " << node.name;
        if(!node.value.empty()){
            std::cout << "(" << node.value << ")";
        }
        std::cout << " x: " << node.x << " y: " << node.y;
        std::vector<int> from = node.from;
        if(from.size() > 0) {
            std::cout << "; from: ";
            for(const int& j : from) {
                std::cout << j << " ";     
            }
        }
        std::vector<int> to = node.to;
        if(to.size() > 0) {
            std::cout << "; to: ";
            for(const int& j : to) {
                std::cout << j << " ";     
            }
        }
        std::cout << std::endl;
    }
}

void AutoLayout::setAutoLayout(int id, float x, float y) {
    std::vector<int> con = m_data[id].to;
    if(x > m_data[id].x) {
        m_data[id].x = x;
    }
    m_data[id].y = y;
    float size = (con.size() - 1) * (m_size_y + m_gap_y);
    float new_x = x + m_size_x + m_gap_x; 
    float new_y = y - 0.5 * size; 
    for(const int c : con) {
        setAutoLayout(c, new_x, new_y);
        new_y += m_size_y+m_gap_y;
    }
}

void AutoLayout::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for(const auto& [id, node] : m_data) {
        const float x = node.x;
        const float y = node.y;
        sf::RectangleShape rectangle({m_size_x, m_size_y});
        rectangle.setPosition(sf::Vector2f{x - static_cast<float>(0.5 * m_size_x), y - static_cast<float>(0.5 * m_size_y)});
        rectangle.setFillColor(sf::Color(216, 211, 101));
        target.draw(rectangle);
        

        bool isBranch = (node.to.size() > 1);
        for(const auto& con : node.to) {
            const auto& targetNode = m_data.at(con);
            const float to_x = targetNode.x - 0.5 * m_size_x;
            const float to_y = targetNode.y;

            float start_x = x + 0.5f * m_size_x;
            float start_y = y;

            float dx = to_x - (x + 0.5f * m_size_x);
            float dy = to_y - y;
            float line_width = sqrtf(dx*dx + dy*dy);

            sf::RectangleShape line({line_width, 3.f});
            sf::Angle line_angle = sf::radians(atan2f(dy, dx));
            line.setPosition(sf::Vector2f{x + static_cast<float>(0.5 * m_size_x), y});
            line.setRotation(line_angle);
            line.setFillColor(sf::Color(230, 240, 130));
            target.draw(line);
            
            if (isBranch && !targetNode.value.empty()) {
                sf::Text branchText(m_font);
                branchText.setString(targetNode.value);
                branchText.setCharacterSize(12);
                branchText.setFillColor(sf::Color(250, 100, 100));

                float mid_x = start_x + dx * 0.5f;
                float mid_y = start_y + dy * 0.5f;

                sf::FloatRect bBounds = branchText.getLocalBounds();
                branchText.setOrigin({
                    bBounds.position.x + bBounds.size.x / 2.0f,
                    bBounds.position.y + bBounds.size.y / 2.0f
                });
                
                branchText.setPosition({mid_x, mid_y - 10.f});
                target.draw(branchText);
            }
        }

        sf::Text text(m_font);
        text.setString(node.name);
        text.setCharacterSize(10);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f{x, y});
        sf::FloatRect textBounds = text.getLocalBounds();
        text.setOrigin({
            textBounds.position.x + textBounds.size.x / 2.0f,
            textBounds.position.y + textBounds.size.y / 2.0f
        });
        text.setFillColor(sf::Color(69, 64, 64));
        target.draw(text);
    }
}