#ifndef DEF_APPLICATION_HPP
#define DEF_APPLICATION_HPP

#include <thread>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

#include "Plot.hpp"
#include "Renderer.hpp"
#include "Simulator.hpp"
#include "World.hpp"

class Application {
public:
	Application();
	~Application();

	void run();

private:
	void update(sf::Time const& dt);
	void display();
	void draw_gui();

	void retrieveEvents();
	bool processEvent(sf::Event const& e);
	void dispatchEvents();

	void close();

private:
	sf::RenderWindow m_window;
	sf::View m_view;
	sf::View m_defaultView;
	float m_zoom;

	Plot m_plot;

	World m_sim_world;     // world used to simulate the generations
	World m_display_world; // world used to display a generation

	std::thread m_sim_thread;

	Renderer  m_renderer;
	Simulator m_simulator;

	sf::Clock m_nextGenClock;

	std::vector<sf::Event> m_eventQueue;
	bool m_open;

	enum class State {
		Simulating,
		Editor_AddingFirstWall,
		Editor_AddingWalls,
		Editor_AddingCheckpoints_1,
		Editor_AddingCheckpoints_2
	} m_state;

	bool m_dragging_screen;
	sf::Vector2f m_dragging_pos;

	/*
	 * Editor stuff
	 */
	sf::Vector2f m_last_wall_pos;
	sf::Vector2f m_last_cp_pos;
	int m_last_checkpoint_id;
};

#endif
