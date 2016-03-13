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
	
	const float mult{0.25f};
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

sf::Vector2f unity_vector(const sf::Vector2f& vektor)
{
	
	if ((vektor.x != 0.0f) || (vektor.y != 0.0f))
	{
		return vektor/abs_vector(vektor);
	}
	
	return vektor;
	
}

sf::Vector2f mouse_position(sf::RenderWindow& window)
{
	return static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
}

sf::Vector2f mouse_vector(sf::RenderWindow& window, const sf::Vector2f& position)
{
	return mouse_position(window) - position;
}

sf::Vector2f direction_accel(sf::RenderWindow &window, const sf::Vector2f& window_xy, const sf::Vector2f& position)
{
	
	const float mult{100.0f};
	assert(mult > 0.0f);
	
	const sf::Vector2f mouse_posit{mouse_position(window)};
	
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right) &&
		(mouse_posit.x > 0.0f) && (mouse_posit.x < window_xy.x) &&
		(mouse_posit.y > 0.0f) && (mouse_posit.y < window_xy.y))
	{
		return mult*unity_vector(mouse_vector(window, position));
	}
	
	return sf::Vector2f (0.0f, 0.0f);
	
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
	
	void set_accel(sf::RenderWindow& window, const sf::Vector2f& window_xy)
	{
		m_accel = direction_accel(window, window_xy, m_position);
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
	
	public:
	
	void show_object(sf::RenderWindow& window)
	{
		window.draw(m_sprite);
	}
	
	float show_radius()
	{
		return m_radius;
	}
	
	sf::Vector2f show_position()
	{
		return m_position;
	}
	
	void move_player_object(sf::RenderWindow& window, const sf::Vector2f& window_xy, const float time_quant)
	{
		set_accel(window, window_xy);
		change_speed(time_quant);
		move_player(time_quant);
		edge_overlap(window_xy);
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

class hydra
{
	
	const std::string m_image_name{"Ball.png"};
		
	sf::Vector2f m_position{0.0f, 0.0f};
	sf::Vector2f m_speed{0.0f, 0.0f};
	
	const sf::Color m_light_orange{255, 195, 127};
	const sf::Color m_light_red{255, 127, 127};
	
	sf::Color m_color{m_light_orange};
	
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
	
	void intro_spedition(sf::RenderWindow& window, player& player_object)
	{
		
		const float mult_posit{2.2f};
		assert(mult_posit > 2.0f);
		
		const float mult_speed{100.0f};
		assert(mult_speed > 0.0f);
		
		const sf::Vector2f unit_vect{unity_vector(mouse_vector(window, player_object.show_position()))};
		
		m_position = mult_posit*player_object.show_radius()*unit_vect + player_object.show_position();
		
		m_speed = mult_speed*unit_vect;
		
	}
	
	public:
	
	void set_color()
	{
		m_sprite.setColor(m_color);
	}
	
	void change_color()
	{
		m_color = m_light_red;
	}
		
	void move_hydra(const float time_delta)
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
	
	void confirm_sprite()
	{
		load_image();
		texture_in_sprite();		
	}
	
	float show_radius()
	{
		return m_radius;
	}
	
	sf::Vector2f show_position()
	{
		return m_position;
	}
	
	hydra(sf::RenderWindow& window, player& player_object)
		: m_texture(), m_sprite()
	{
		
		intro_spedition(window, player_object);
		confirm_sprite();
		set_origin();
		set_position();
		set_color();
		
	}
	
	~hydra()
	{
		
	}
	
};

void add_hydra(std::vector <hydra>& hydra_objects, sf::RenderWindow& window, player& player_object)
{
	
	hydra_objects.push_back(hydra (window, player_object));
	
}

void confirm_hydras(std::vector <hydra>& hydra_objects, const int hydra_count)
{
	
	int count{0};
	
	while (count < hydra_count)
	{
		
		hydra_objects[count].confirm_sprite();
		++count;
		
	}
	
}

void left_mouse_button(sf::RenderWindow& window, std::vector <hydra>& hydra_objects, player& player_object,
					   const sf::Vector2f& window_xy, int& hydra_count, bool& hydra_launched)
{
	
	const sf::Vector2f mouse_posit{mouse_position(window)};
		
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		!hydra_launched &&
		(mouse_posit.x > 0.0f) && (mouse_posit.x < window_xy.x) &&
		(mouse_posit.y > 0.0f) && (mouse_posit.y < window_xy.y))
	{
		add_hydra(hydra_objects, window, player_object);
		++hydra_count;
		
		confirm_hydras(hydra_objects, hydra_count);
		
		hydra_launched = true;
	}
	
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		hydra_launched)
	{
		hydra_launched = false;
	}
	
}

void show_hydras(sf::RenderWindow& window, std::vector <hydra>& hydra_objects, const int hydra_count)
{
	
	if (hydra_count > 0)
	{
		int count{0};
		
		while(count < hydra_count)
		{
			hydra_objects[count].show_object(window);
			++count;
		}
	}
	
}

void move_hydras(std::vector <hydra>& hydra_objects, const sf::Vector2f& window_xy,
				const float time_quant, const int hydra_count)
{
	
	int count{0};
	
	while(count < hydra_count)
	{
		hydra_objects[count].move_hydra(time_quant);
		hydra_objects[count].edge_overlap(window_xy);
		++count;
	}
	
}

void colloride_hydras(hydra& hydra_object_a, hydra& hydra_object_b)
{
	
	if (abs_vector(hydra_object_b.show_position() - hydra_object_a.show_position()) <
		(hydra_object_a.show_radius() + hydra_object_b.show_radius()))
	{
		hydra_object_a.change_color();
		hydra_object_a.set_color();
		
		hydra_object_b.change_color();
		hydra_object_b.set_color();
	}
	
}

void collide_hydras(std::vector <hydra>& hydra_objects, const int hydra_count)
{
	
	int count{0};

	while(count < hydra_count - 1)
	{
		
		int count_2{count + 1};
		
		while (count_2 < hydra_count)
		{
			
			colloride_hydras(hydra_objects[count], hydra_objects[count_2]);			
			++count_2;
			
		}
		
		++count;
		
	}

}

int main()
{
	
	const std::string program_name{"Hydraoids V0.3"};
	assert(program_name != "");
	
	const float window_x{704.0f};
	assert(window_x > 0.0f);
	
	const float window_y{704.0f};
	assert(window_y > 0.0f);
	
	const sf::Vector2f window_xy{window_x, window_y};
	
	const float time_quant{0.001f};
	assert(time_quant > 0.0f);
	
	const float time_delta{25.0f*time_quant};
	assert(time_delta > time_quant);
	
	const sf::Color black{sf::Color(0, 0, 0)};
	
	player player_object{0.5f*window_xy};
	
	std::vector <hydra> hydra_objects;
	int hydra_count{0};
	assert(hydra_count == 0);
	
	bool hydra_launched{false};
	
	sf::RenderWindow window{sf::VideoMode(window_x, window_y), program_name, sf::Style::Default};
	
	while (window.isOpen())
    {
		
		sf::Clock clock;
		
		sf::Event event;
		
		window.clear(black);
		
		if (hydra_count > 0)
		{
			show_hydras(window, hydra_objects, hydra_count);
		}
		
		player_object.show_object(window);
		
		window.display();
		
		while (clock.getElapsedTime().asSeconds() < time_delta)
		{
			
			sf::Clock timer;
			
			if (hydra_count > 0)
			{
				move_hydras(hydra_objects, window_xy, time_quant, hydra_count);
				collide_hydras(hydra_objects, hydra_count);
			}
			
			player_object.move_player_object(window, window_xy, time_quant);
			
			// std::cout << timer.getElapsedTime().asSeconds() << "!\n";
			
			while (timer.getElapsedTime().asSeconds() < time_quant)
			{
				// std::cout << timer.getElapsedTime().asSeconds() << "\n";
			}
			
			// std::cout << timer.getElapsedTime().asSeconds() << "?\n";
			
			// idle_wait(time_quant);
			
		}
		
		// window.close();
		// return 3;
		
		left_mouse_button(window, hydra_objects, player_object, window_xy, hydra_count, hydra_launched);
		
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
