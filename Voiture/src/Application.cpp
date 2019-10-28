#include "Application.hpp"

#include "ImGui/imgui.h"
#include "ImGui/imgui-SFML.h"

#include "utils.hpp"

Application::Application():
	m_window(sf::VideoMode(800, 600), "Voiture"),
	m_view(sf::Vector2f(0,0), sf::Vector2f(800,600)),
	m_defaultView(sf::Vector2f(400, 300), sf::Vector2f(800, 600)),
	m_zoom(1.f),
	m_plot(sf::Vector2f(0,100), sf::Vector2f(200, 100)),
	m_renderer(m_display_world),
	m_simulator(m_plot, m_sim_world, {agent_sensor_count, 6, 1}),
	m_open(true),
	m_state(State::Simulating),
	m_dragging_screen(false),
	m_last_checkpoint_id(0)
{
	m_window.setFramerateLimit(60);
	ImGui::SFML::Init(m_window);

	m_display_world.load("world.data");
	m_sim_world.load("world.data");
	m_renderer.updateCircuit(m_display_world);
}

Application::~Application() {
	m_simulator.stop();
	if (m_sim_thread.joinable())
		m_sim_thread.join();
}

void Application::run() {
	sf::Clock clock;
	while (m_open) {
		const sf::Time tick = clock.restart();

		retrieveEvents();
		ImGui::SFML::Update(m_window, tick);
		dispatchEvents();
		draw_gui();

		update(tick);

		display();
	}

	ImGui::SFML::Shutdown();
}

void Application::update(sf::Time const& dt) {
	switch (m_state) {
		case State::Simulating:
			m_renderer.updateAgents(m_display_world.step(dt.asSeconds()));
			if (m_simulator.isRunning() && m_nextGenClock.getElapsedTime() > sf::seconds(autorefresh_seconds)) {
				m_nextGenClock.restart();
				auto const& gen = m_simulator.getGeneration(-1);
				m_display_world.resetAgents(gen);
				m_renderer.setAgentCount(gen.size());
			}
			break;
		default:
			break;
	}
}

void Application::display() {
		m_window.setView(m_view);

		m_window.clear();
		m_renderer.render(m_window);
		m_window.setView(m_defaultView);
		m_window.draw(m_plot);
		ImGui::SFML::Render(m_window);
		m_window.display();
}

void Application::draw_gui() {
	static bool is_sim_running = false;
	ImGui::Begin("Control Panel");

	/*
	 *  SIMULATOR CONTROL
	 */
	ImGui::Separator();
	if (ImGui::Button("Run") && !is_sim_running) {
		m_sim_thread = std::thread(&Simulator::run, &m_simulator);
		is_sim_running = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Stop") && is_sim_running) {
		is_sim_running = false;
		m_simulator.stop();
		if (m_sim_thread.joinable()) {
			m_sim_thread.join();
		}
	}
	ImGui::SameLine();
	if (is_sim_running)
		ImGui::Text("Running");
	else
		ImGui::Text("Stopped");

	ImGui::Text((std::string("History size: ") + std::to_string(m_simulator.getHistorySize())).c_str());

	/*
	 *  VIEW CONTROL
	 */
	ImGui::Separator();
	if (ImGui::Button("View last gen")) {
		auto const& gen = m_simulator.getGeneration(-1);
		m_display_world.resetAgents(gen);
		m_renderer.setAgentCount(gen.size());
	}
	else if (ImGui::Button("Clear preview"))
		m_renderer.setAgentCount(0);

	/*
	 * EDITOR CONTROL
	 */
	ImGui::Separator();
	if (ImGui::Button("Add Walls"))
		m_state = State::Editor_AddingFirstWall;
	else if (ImGui::Button("Remove last wall")) {
		m_display_world.removeLastWall();
		m_sim_world.removeLastWall();
		m_renderer.updateCircuit(m_display_world);
	}
	else if (ImGui::Button("Add Checkpoints"))
		m_state = State::Editor_AddingCheckpoints_1;
	else if (ImGui::Button("Done"))
		m_state = State::Simulating;

	ImGui::Separator();
	if (ImGui::Button("Load")) {
		if (m_display_world.load("world.data")) {
			m_sim_world.load("world.data");
			m_renderer.updateCircuit(m_display_world);
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("Save")) {
		m_display_world.save("world.data");
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear")) {
		m_display_world.clear();
		m_sim_world.clear();
		m_renderer.updateCircuit(m_display_world);
	}

	ImGui::End();
}

void Application::retrieveEvents() {
	sf::Event event;
	m_eventQueue.clear();
	while (m_window.pollEvent(event)) {
		ImGui::SFML::ProcessEvent(event);
		m_eventQueue.push_back(event);
		if (event.type == sf::Event::Closed)
			close();
		else if (event.type == sf::Event::Resized)
			processEvent(event);
	}
}

bool Application::processEvent(sf::Event const& event) {
	sf::Vector2f mpos = 
		m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window), m_view);
	switch (event.type) {
		case sf::Event::Resized:
			{
				sf::Vector2f s(event.size.width, event.size.height);
				auto center = m_view.getCenter();
				m_view.setSize(s);
				m_view.setCenter(center);

				m_defaultView.setSize(s);
				m_defaultView.setCenter(s/2.f);
			}
			return true;
		case sf::Event::MouseButtonPressed:
			switch (event.mouseButton.button) {
				case sf::Mouse::Left:
					{
						switch (m_state) {
							case State::Editor_AddingFirstWall:
								m_last_wall_pos = mpos;
								m_state = State::Editor_AddingWalls;
								return true;
							case State::Editor_AddingWalls:
								m_sim_world.addWall(Wall(m_last_wall_pos, mpos));
								m_display_world.addWall(Wall(m_last_wall_pos, mpos));
								m_last_wall_pos = mpos;
								m_renderer.updateCircuit(m_display_world);
								return true;
							case State::Editor_AddingCheckpoints_1:
								m_last_cp_pos = mpos;
								m_state = State::Editor_AddingCheckpoints_2;
								return true;
								break;
							case State::Editor_AddingCheckpoints_2:
								m_sim_world.addCheckpoint(Checkpoint(m_last_checkpoint_id,
											Wall(m_last_cp_pos, mpos)));
								m_display_world.addCheckpoint(Checkpoint(m_last_checkpoint_id,
											Wall(m_last_cp_pos, mpos)));
								m_last_checkpoint_id++;
								m_renderer.updateCircuit(m_display_world);
								m_state = State::Editor_AddingCheckpoints_1;
								return true;
								break;
							default:
								break;
						}
					}
					break;
				case sf::Mouse::Middle:
					m_dragging_screen = true;
					m_dragging_pos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
					return true;
					break;
				default:
					break;
			}
			break;
		case sf::Event::MouseButtonReleased:
			switch (event.mouseButton.button) {
				case sf::Mouse::Middle:
					m_dragging_screen = false;
					return true;
					break;
				default:
					break;
			}
			break;
		case sf::Event::MouseMoved:
			if (m_dragging_screen) {
				sf::Vector2f pos(event.mouseMove.x, event.mouseMove.y);
				m_view.move((m_dragging_pos - pos)*m_zoom);
				m_dragging_pos = pos;
				return true;
			}
			break;
		case sf::Event::MouseWheelScrolled:
			if (event.mouseWheelScroll.delta < 0) {
				m_view.zoom(1.1);
				m_zoom *= 1.1f;
			}
			else {
				m_view.zoom(0.9);
				m_zoom *= 0.9f;
			}
			break;
		default:
			break;
	}

	return false;
}

void Application::dispatchEvents() {
	if (!(ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)) {
		for (auto const& e: m_eventQueue) {
			if (processEvent(e))
				continue;
			// dispatch here
		}
	}
}

void Application::close() {
	m_open = false;
	m_window.close();
}
