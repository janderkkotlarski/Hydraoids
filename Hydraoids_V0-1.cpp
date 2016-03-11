#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cassert>

#include <SFML/Graphics.hpp>

void loading(sf::Texture& texture, const std::string& image_name)
{
	
	assert(image_name != "");
	
	if (!texture.loadFromFile(image_name))
	{	
		std::cerr << image_name << " not found!\n";		
	}

}

float originate(sf::Sprite& sprite)
{
	
	const float mult{0.27f};
	assert(mult > 0.0f);
		
	const sf::FloatRect image_bounds{sprite.getLocalBounds()};		
	sprite.setOrigin(0.5f*image_bounds.width, 0.5f*image_bounds.height);
	return mult*(image_bounds.width + image_bounds.height);
	
}

void idle_wait(const float time_quant)
{
	
	sf::Clock clock;
	
	while (clock.getElapsedTime().asSeconds() < time_quant)
	{
				
	}
	
}

float abs_vector(const sf::Vector2f& vektor)
{
	return std::sqrt(vektor.x*vektor.x + vektor.y*vektor.y);
}

void unbounding(sf::Vector2f& position, const sf::Vector2f& window_xy)
{
	
	while (position.x > window_xy.x)
	{
		position.x -= window_xy.x;			
	}
	
	while (position.x < 0)
	{
		position.x += window_xy.x;			
	}
	
	while (position.y > window_xy.y)
	{
		position.y -= window_xy.y;			
	}
	
	while (position.y < 0)
	{
		position.y += window_xy.y;			
	}
	
}

sf::Vector2f direction_accel(sf::RenderWindow &window, const sf::Vector2f& position)
{
	
	const float mult{1.0f};
	assert(mult > 0.0f);
	
	const sf::Vector2f direction{static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - position};
	
	sf::Vector2f accel{0.0f, 0.0f};
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		accel = mult*direction/abs_vector(direction);
	}
	
	return accel;
	
}

class player
{
	 
	const std::string m_image_name{"Ball.png"};
		
	sf::Vector2f m_position{0.0f, 0.0f};
	sf::Vector2f m_speed{0.0f, 0.0f};
	sf::Vector2f m_accel{0.0f, 0.0f};
	
	const sf::Color m_light_blue{127, 127, 255};
	
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	
	float m_radius{0.0f};
	
	void load_image()
	{
		loading(m_texture, m_image_name);
	}
	
	void texture_in_sprite()
	{
		m_sprite.setTexture(m_texture);
	}
	
	void set_origin()
	{
		m_radius = originate(m_sprite);
	}
	
	void set_position()
	{
		m_sprite.setPosition(m_position);
	}
	
	void set_color()
	{
		m_sprite.setColor(m_light_blue);
	}
	
	public:
	
	void set_accel(sf::RenderWindow &window)
	{
		m_accel = direction_accel(window, m_position);
	}
	
	void change_speed(const float time_delta)
	{
		m_speed += time_delta*m_accel;
	}
	
	void move_player(const float time_delta)
	{
		m_position += time_delta*m_speed;
		set_position();
	}
	
	void edge_overlap(const sf::Vector2f& window_xy)
	{
		unbounding(m_position, window_xy);		
		set_position();
	}
	
	void show_object(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}
	
	player(const sf::Vector2f& position)
		: m_position(position), m_texture(), m_sprite()
	{
		
		load_image();
		texture_in_sprite();
		set_origin();
		set_position();
		set_color();
		
	}
	
	~player()
	{
		
	}
	
};

int main()
{
	
	const std::string program_name{"Hydraoids V0.1"};
	assert(program_name != "");
	
	const float window_x{768.0f};
	const float window_y{768.0f};
	
	const sf::Vector2f window_xy{window_x, window_y};
	
	const float time_quant{0.001f};
	const float time_delta{25.0f*time_quant};
	
	const sf::Color black{sf::Color(0, 0, 0)};
	
	player player_circle{0.5f*window_xy};
	
	sf::RenderWindow window{sf::VideoMode(window_x, window_y), program_name, sf::Style::Default};
	
	while (window.isOpen())
    {
		
		sf::Clock clock;
		
		sf::Event event;
		
		window.clear(black);
		
		player_circle.show_object(window);
		
		window.display();
		
		while (clock.getElapsedTime().asSeconds() < time_delta)
		{
			
			player_circle.set_accel(window);
			player_circle.change_speed(time_quant);
			player_circle.move_player(time_quant);
			player_circle.edge_overlap(window_xy);
			
			idle_wait(time_quant);
			
		}
		
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();                    
			return 1;
		}
		
		while (window.pollEvent(event))
		{
			
			if (event.type == sf::Event::Closed)
			{
				window.close();                
				return 2;
			}
		
		}
		
	}	
	
	return 0;

}
