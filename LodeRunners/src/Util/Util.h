#pragma once

#include <vector>
#include <unordered_map>
#include <string>

namespace Data
{
	template <typename T>
	struct Vertex
	{
		Vertex() = default;
		Vertex(const std::string& id) : id(id) {}

		using Neighbour = std::pair<T, Vertex*>;

		std::string id;
		std::vector<Neighbour> neighbours;
	};

	template <typename T>
	class Graph
	{
	public:
		Graph() = default;

		Graph(const Graph& other)
		{
			for (const auto& pair : other.m_Vertices)
				this->addVertex(pair.second.id);

			for (const auto& pair : other.m_Vertices)
			{
				const auto& from = pair.second.id;

				for (const auto& neighbour : pair.second.neighbours)
					this->addEdge(from, neighbour.second->id, neighbour.first);
			}
		}

		bool addVertex(const std::string& id)
		{
			if (m_Vertices.contains(id)) return false;

			m_Vertices[id] = Vertex<T>(id);
			return true;
		}

		bool addEdge(const std::string& from, const std::string& to, const T& cost)
		{
			if (!m_Vertices.contains(from) || !m_Vertices.contains(to)) return false;

			auto& neighbours = m_Vertices[from].neighbours;
			Vertex<T>* vTo = &m_Vertices[to];

			if (std::find_if(neighbours.begin(), neighbours.end(),
				[&](const Vertex<T>::Neighbour& n) -> bool
				{
					return n.second->id == to;
				}) != neighbours.end()) return false;

			neighbours.push_back(Vertex<T>::Neighbour(cost, vTo));

			return true;
		}

		const Vertex<T>& getVertex(const std::string& id) const
		{
			return m_Vertices.at(id);
		}

	private:
		std::unordered_map<std::string, Vertex<T>> m_Vertices;
	};

}

namespace AI
{
	//ComputeAStar()
}