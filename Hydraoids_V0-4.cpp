#include <iostream>
#include <sstream>
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
	
	const float mult{0.23f};
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

float vector_dot_vector(const sf::Vector2f& vektor_a, const sf::Vector2f& vektor_b)
{
	
	return vektor_a.x*vektor_b.x + vektor_a.y*vektor_b.y;
	
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

std::string score_to_string(const int skore)
{
    std::stringstream stringer;
    stringer << skore;
    return stringer.str();
}

class score
{
	
	const std::string m_file_name{"VeraMono-Bold.ttf"};
	
	const float m_mult{0.07f};	
	const float m_text_size{10.0f};
	
	const sf::Color m_creme{sf::Color(255, 224, 196, 255)};
	
	const std::string m_score_name{"Score: "};
	
	int m_score{0};
	
	sf::Font m_font;
	
	sf::Text m_text;
	
	void set_font()
	{
		
		assert(m_file_name != "");
		
		if (!m_font.loadFromFile(m_file_name))
		{
			std::cout << m_file_name << " not found!\n";
		}
		 
	}
		
	void set_text_font()
	{
		m_text.setFont(m_font);
	}
	
	void set_text_size()
	{
		assert(m_text_size > 0.0f);
		m_text.setCharacterSize(m_text_size);
	}
	
	void set_color()
	{
		m_text.setColor(m_creme);
	}
	
	void set_origin()
	{
		const sf::FloatRect rectangle{m_text.getLocalBounds()};
		m_text.setOrigin(0.5f*rectangle.width, 0.95f*rectangle.height);
	}
	
	void set_position(const sf::Vector2f& position)
	{
		m_text.setPosition(position);
	}
	
	public:
	
	void add_score()
	{
		++m_score;
	}
	
	void textify_score()
	{
		m_text.setString(m_score_name + score_to_string(m_score));
	}
	
	void show_score(sf::RenderWindow& window)
	{
		window.draw(m_text);
	}
	
	score(const sf::Vector2f& position)
		: m_text_size(m_mult*(position.x + position.y)), m_font(), m_text()
	{
		assert(m_score_name != "");
		assert(m_score == 0);
		
		set_font();
		set_text_font();
		set_text_size();
		set_color();
		textify_score();
		set_origin();
		set_position(position);
	}
	
	~score()
	{
		
	}
	
};

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
	
	bool m_shot{true};
	
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
	
	void set_color()
	{
		m_sprite.setColor(m_color);
	}
	
	void change_color()
	{
		m_color = m_light_red;
	}
	
	void change_shot()
	{
		m_shot = false;
	}
	
	void intro_spedition(sf::RenderWindow& window, player& player_object)
	{
		
		const float mult_posit{2.2f};
		assert(mult_posit > 2.0f);
		
		const float mult_speed{250.0f};
		assert(mult_speed > 0.0f);
		
		const sf::Vector2f unit_vect{unity_vector(mouse_vector(window, player_object.show_position()))};
		
		m_position = mult_posit*player_object.show_radius()*unit_vect + player_object.show_position();
		
		m_speed = mult_speed*unit_vect;
		
	}
	
	public:
	
	void is_shot(score& game_score)
	{
		
		if (m_shot)
		{
			change_color();
			set_color();
			change_shot();
			game_score.add_score();
			game_score.textify_score();
		}
		
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
	
	sf::Vector2f show_speed()
	{
		return m_speed;
	}
	
	void set_speed(const sf::Vector2f& speed)
	{
		m_speed = speed;
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
					   const sf::Vector2f& window_xy, int& hydra_count, bool& hydra_launched, int& hydra_countdown)
{
	
	const int  countdown{10};
	
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
		
		hydra_countdown = countdown;
	}
	
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
		hydra_launched && (hydra_countdown == 0))
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

void colloride_hydras(hydra& hydra_object_a, hydra& hydra_object_b, score& game_score)
{
		hydra_object_a.is_shot(game_score);
		hydra_object_b.is_shot(game_score);
}

void elastic_hydras(hydra& hydra_object_a, hydra& hydra_object_b)
{
	
	const sf::Vector2f hydra_speed_a{hydra_object_a.show_speed()};
	const sf::Vector2f hydra_speed_b{hydra_object_b.show_speed()};
	
	const sf::Vector2f delta_position{hydra_object_b.show_position() - hydra_object_a.show_position()};	
	const sf::Vector2f delta_speed{hydra_speed_b - hydra_speed_a};
	
	const float speed_dot_position{vector_dot_vector(delta_speed, delta_position)};	
	const float position_dot_position{vector_dot_vector(delta_position, delta_position)};
	
	hydra_object_a.set_speed(hydra_speed_a + (speed_dot_position/position_dot_position)*delta_position);
	hydra_object_b.set_speed(hydra_speed_b - (speed_dot_position/position_dot_position)*delta_position);
	
}

void collision_hydras(hydra& hydra_object_a, hydra& hydra_object_b, score& game_score)
{
	
	if (abs_vector(hydra_object_b.show_position() - hydra_object_a.show_position()) <
		(hydra_object_a.show_radius() + hydra_object_b.show_radius()))
	{
		colloride_hydras(hydra_object_a, hydra_object_b, game_score);
		elastic_hydras(hydra_object_a, hydra_object_b);
	}
	
}

void collide_hydras(std::vector <hydra>& hydra_objects, const int hydra_count, score& game_score)
{
	
	int count{0};

	while(count < hydra_count - 1)
	{
		
		int count_2{count + 1};
		
		while (count_2 < hydra_count)
		{
			
			collision_hydras(hydra_objects[count], hydra_objects[count_2], game_score);			
			++count_2;
			
		}
		
		++count;
		
	}

}

bool player_collide_hydra(player& player_object, hydra& hydra_object)
{
	
	if (abs_vector(player_object.show_position() - hydra_object.show_position()) <
		(player_object.show_radius() + hydra_object.show_radius()))
	{
		return true;
	}
	
	return false;
	
}

bool player_collide_hydras(player& player_object, std::vector <hydra>& hydra_objects, const int hydra_count)
{
	
	int count{0};
	
	bool collided{false};
	
	while(count < hydra_count)
	{
		if (player_collide_hydra(player_object, hydra_objects[count]))
		{
			collided = true;	
		}
		
		++count;
	}
	
	return collided;
	
}

int main()
{
	
	const std::string program_name{"Hydraoids V0.4"};
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
	
	sf::RenderWindow window{sf::VideoMode(window_x, window_y), program_name, sf::Style::Default};
	
	while (window.isOpen())
    {
		
		player player_object{0.5f*window_xy};
	
		std::vector <hydra> hydra_objects;
		int hydra_count{0};
		assert(hydra_count == 0);
		
		bool alive{true};
		
		score game_score{sf::Vector2f (0.48f*window_x, 0.035f*window_y)};
		
		bool hydra_launched{false};
		assert(!hydra_launched);
		
		int hydra_countdown{0};
		assert(hydra_countdown == 0);
		
		while (alive)
		{
			
			sf::Clock clock;
			
			sf::Event event;
			
			window.clear(black);
			
			if (hydra_count > 0)
			{
				show_hydras(window, hydra_objects, hydra_count);
			}
			
			player_object.show_object(window);
			
			game_score.show_score(window);
			
			window.display();
			
			while (clock.getElapsedTime().asSeconds() < time_delta)
			{
				
				sf::Clock timer;
				
				if (hydra_count > 0)
				{
					move_hydras(hydra_objects, window_xy, time_quant, hydra_count);
					collide_hydras(hydra_objects, hydra_count, game_score);
					alive = !player_collide_hydras(player_object, hydra_objects, hydra_count);
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
					
			if (hydra_countdown > 0)
			{
				--hydra_countdown;
			}
			else if (hydra_countdown < 0)
			{
				hydra_countdown = 0;
			}
			
			left_mouse_button(window, hydra_objects, player_object, window_xy, hydra_count, hydra_launched, hydra_countdown);
						
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
		
	}	
	
	return 0;

}
