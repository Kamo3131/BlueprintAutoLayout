#include <json/json.h>
#include <fstream>
#include <map>
#include <SFML/Graphics.hpp>
struct Node {
    int id = 0;
    std::string name = "";
    float x = 0.f; float y = 0.f;
    std::string value = "";
    std::vector<int> to;
    std::vector<int> from;
};
class AutoLayout : public sf::Drawable{
    private:
        sf::Font m_font;
        std::map<int, Node> m_data;
        float m_size_x = 120.f, m_size_y = 80.f;
        float m_gap_x = 80.f, m_gap_y = 40.f;
    
        std::string valueOfName(std::string & name);
    public:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        AutoLayout() = default;
        AutoLayout(std::string file_path);
        ~AutoLayout() = default;
        void loadDataFromFile(std::string file_path);
        void loadFontFromFile(std::string file_path);
        void setAutoLayout(int id, float x, float y);
        void printDataContents() const;
};