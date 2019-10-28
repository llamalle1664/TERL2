#include "Plot.hpp"

#include <limits>

#include <SFML/Graphics/RenderTarget.hpp>

#include <iostream>

Plot::Plot(sf::Vector2f const& pos, sf::Vector2f const& size):
	m_vbuffer(sf::LineStrip),
	m_min(std::numeric_limits<float>::max()),
	m_max(std::numeric_limits<float>::lowest()),
	m_pos(pos),
	m_size(size)
{
}

void Plot::append(float value) {
	std::lock_guard<std::mutex> lk(m_vbuffer_mutex);

	m_data.push_back(value);
	m_vbuffer.append({});

	const size_t size = m_data.size();
	std::vector<sf::Vertex> v(size);

	m_min = std::min(value, m_min);
	m_max = std::max(value, m_max);

	if (m_min == m_max)
		m_min += std::numeric_limits<float>::epsilon();

	for (size_t i = 0; i < size; i++) {
		const float x = m_size.x*static_cast<float>(i)/static_cast<float>(size);
		const float y = m_size.y - (m_data[i]- m_min)*(m_size.y)/(m_max - m_min);
		m_vbuffer[i].position = m_pos + sf::Vector2f(x,y);
		m_vbuffer[i].color = sf::Color::Green;
	}
}

void Plot::clear() {
	m_data.clear();
	m_vbuffer.clear();
}

void Plot::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	std::lock_guard<std::mutex> lk(m_vbuffer_mutex);
	target.draw(m_vbuffer, states);
}
