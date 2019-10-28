#ifndef DEF_PLOT_HPP
#define DEF_PLOT_HPP

#include <vector>
#include <mutex>

#include <SFML/Graphics/VertexArray.hpp>

class Plot: public sf::Drawable {
public:
	Plot(sf::Vector2f const& pos, sf::Vector2f const& size);

	/*
	 * Add a new data point to the plot
	 */
	void append(float value);

	/*
	 * Clear the plot
	 */
	void clear();

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::VertexArray m_vbuffer;
	mutable std::mutex m_vbuffer_mutex;
	std::vector<float> m_data;
	float m_min;
	float m_max;
	sf::Vector2f m_pos;
	sf::Vector2f m_size;
};

#endif
