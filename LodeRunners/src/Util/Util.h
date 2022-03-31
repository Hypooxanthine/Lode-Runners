#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

template<>
struct std::hash<sf::Vector2i>
{
	size_t operator()(const sf::Vector2i& v) const
	{
		return std::hash<std::string>()(std::to_string(v.x) + " " + std::to_string(v.y));
	}
};

namespace Data
{
	class Node;

	class Node
	{
	public:
		Node() = default;
		Node(const sf::Vector2i& pos);

		const sf::Vector2i& getPosition() const;
		const double& getGCost() const { return m_gCost; }
		const double& getHCost() const { return m_hCost; }
		const double& getFCost() const { return m_fCost; }

		const Node* getCameFrom() const;

		double heuristcDistTo(const Node& other) const;
		void computeCosts(const Node* fromNode, const Node* goal);

		std::vector<Node*>& getNeighbours();
		const std::vector<Node*>& getNeighbours() const;

		void addNeighbour(Node* neighbour);

		bool operator==(const Node& other) const;

	private:
		sf::Vector2i m_Pos;
		std::vector<Node*> m_Neighbours;
		const Node* m_CameFrom = nullptr;

		double m_gCost = 0.f, m_hCost = 0.f, m_fCost = 0.f;
		bool m_Computed = false;
	};

	class AStarGraph
	{
	public:
		AStarGraph() = default;
		AStarGraph(const AStarGraph& other);

		void addNode(const sf::Vector2i& pos);

		void addEdge(const sf::Vector2i& from, const sf::Vector2i& to);

		bool contains(const sf::Vector2i& pos);

		Node* getNode(const sf::Vector2i& pos);
		const Node* getNode(const sf::Vector2i& pos) const;

		std::vector<Node*>& getNeighbours(const sf::Vector2i& node);
		const std::vector<Node*>& getNeighbours(const sf::Vector2i& node) const;

	private:
		std::unordered_map<sf::Vector2i, Node> m_Nodes;
	};

}

namespace AI
{
	bool ComputeAStar(Data::AStarGraph graph, const sf::Vector2i& startPos, const sf::Vector2i& goalPos, std::vector<Data::Node>& path);
}