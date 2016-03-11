#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

#include <SFML/Graphics.hpp>

class player
{
	 
	const std::string m_image_name{"Ball.png"};
	
	float m_radius{0.0f};
	
	sf::Vector2f m_position{0.0f, 0.0f};
	
	sf::Vector2f m_speed{0.0f, 0.0f};
	
	sf::Vector2f m_accel{0.0f, 0.0f};
	
	const sf::Color m_light_blue{127, 127, 255};
	
	sf::Texture m_texture;
	
	sf::Sprite m_sprite;
	
	void load_image()
	{
	
		assert(m_image_name != "");
		
		if (!m_texture.loadFromFile(m_image_name))
		{
		
			std::cerr << m_image_name << " not found!\n";
			
		}
	
	}
	
	void texture_in_sprite()
	{
		
		m_sprite.setTexture(m_texture);
		
	}
	
	void set_origin()
	{
		
		const float mult{0.27f};
		assert(mult > 0.0f);
		
		const sf::FloatRect m_image_bounds{m_sprite.getLocalBounds()};		
		m_sprite.setOrigin(0.5f*m_image_bounds.width, 0.5f*m_image_bounds.height);
		m_radius = mult*(m_image_bounds.width + m_image_bounds.height);
		
	}
	
};

int main()
{
	
	const std::string program_name{"Hydraoids V0.1"};
	assert(program_name != "");
	
	
	
	
	
	
	return 0;

}
