#include "Util.h"

#include <map>
#include <vector>
#include <algorithm>

#include "../Core/Base.h"

namespace Data
{
	/* Node class */

	Node::Node(const sf::Vector2i& pos)
		: m_Pos(pos)
	{}

	const sf::Vector2i& Node::getPosition() const
	{
		return m_Pos;
	}

	const Node* Node::getCameFrom() const
	{
		return m_CameFrom;
	}

	double Node::heuristcDistTo(const Node& other) const
	{
		return std::pow(other.m_Pos.x - m_Pos.x, 2) + std::pow(other.m_Pos.y - m_Pos.y, 2);
	}

	void Node::computeCosts(const Node* fromNode, const Node* goal)
	{
		if(!m_Computed)
		{
			m_gCost = fromNode->getGCost() + heuristcDistTo(*fromNode);
			m_hCost = heuristcDistTo(*goal);

			m_fCost = m_gCost + m_hCost;
			m_CameFrom = fromNode;
		}
		else
		{
			double temp_gCost = fromNode->getGCost() + heuristcDistTo(*fromNode);
			double temp_hCost = heuristcDistTo(*goal);

			double temp_fCost = m_gCost + m_hCost;

			if (temp_fCost < m_fCost)
			{
				m_gCost = temp_gCost;
				m_hCost = temp_hCost;
				m_fCost = temp_fCost;
				m_CameFrom = fromNode;
			}
		}
	}

	std::vector<Node*>& Node::getNeighbours()
	{
		return m_Neighbours;
	}

	const std::vector<Node*>& Node::getNeighbours() const
	{
		return m_Neighbours;
	}

	void Node::addNeighbour(Node* neighbour)
	{
		m_Neighbours.push_back(neighbour);
	}

	bool Node::operator==(const Node& other) const
	{
		return m_Pos.x == other.m_Pos.x && m_Pos.y == other.m_Pos.y;
	}

	/* AStarGraph class */

	AStarGraph::AStarGraph(const AStarGraph& other)
	{
		for (const auto& [id, node] : other.m_Nodes)
		{
			Node temp(node.getPosition());

			m_Nodes[id] = Node(node.getPosition());
		}

		for (const auto& [id, node] : other.m_Nodes)
		{
			for (const auto& neighbour : node.getNeighbours())
			{
				Node* correspondingNode = &m_Nodes.at(neighbour->getPosition());

				m_Nodes.at(id).addNeighbour(correspondingNode);
			}
		}
	}

	void AStarGraph::addNode(const sf::Vector2i& pos)
	{
		m_Nodes[pos] = Node(pos);
	}

	void AStarGraph::addEdge(const sf::Vector2i& from, const sf::Vector2i& to)
	{
		m_Nodes.at(from).addNeighbour(&m_Nodes.at(to));
	}

	bool AStarGraph::contains(const sf::Vector2i& pos)
	{
		return m_Nodes.contains(pos);
	}

	Node* AStarGraph::getNode(const sf::Vector2i& pos)
	{
		return &m_Nodes.at(pos);
	}

	const Node* AStarGraph::getNode(const sf::Vector2i& pos) const
	{
		return &m_Nodes.at(pos);
	}

	std::vector<Node*>& AStarGraph::getNeighbours(const sf::Vector2i& node)
	{
		return getNode(node)->getNeighbours();
	}

	const std::vector<Node*>& AStarGraph::getNeighbours(const sf::Vector2i& node) const
	{
		return getNode(node)->getNeighbours();
	}
}

namespace AI
{
	// "Best" node is vector.front().
	void sortOpenSet(std::vector<Data::Node*>& set)
	{
		using namespace Data;

		std::sort(set.begin(), set.end(),
			[](const Node* left, const Node* right) -> bool
			{
				if (left->getFCost() < right->getFCost()
					|| (left->getFCost() == right->getFCost() && left->getHCost() < right->getHCost()))
					return true;
				else
					return false;
			});
	}

	void reconstructPath(const Data::Node* current, std::vector<Data::Node>& path)
	{
		using namespace Data;

		path.clear();

		path.push_back(*current);

		while (current->getCameFrom() != nullptr)
		{
			path.push_back(*current->getCameFrom());
			current = current->getCameFrom();
		}

		std::reverse(path.begin(), path.end());
	}

	bool ComputeAStar(Data::AStarGraph graph, const sf::Vector2i& startPos, const sf::Vector2i& goalPos, std::vector<Data::Node>& path)
	{
		using namespace Data;
		
		Node* start = nullptr;
		Node* goal = nullptr;

		if (graph.contains(startPos))
			start = graph.getNode(startPos);
		else
			return false;

		if (graph.contains(goalPos))
			goal = graph.getNode(goalPos);
		else
			return false;

		std::vector<Node*> openSet;
		std::vector<Node*> closedSet;

		openSet.push_back(start);
		sortOpenSet(openSet);

		while (!openSet.empty())
		{
			Node* current = openSet.front();

			if (current == goal)
			{
				reconstructPath(current, path);
				return true;
			}

			openSet.erase(openSet.begin());
			closedSet.push_back(current);

			for (auto& neighbour : current->getNeighbours())
			{
				if(std::find(closedSet.begin(), closedSet.end(), neighbour) == closedSet.end())
				{
					neighbour->computeCosts(current, goal);

					if(std::find(openSet.begin(), openSet.end(), neighbour) == openSet.end())
						openSet.push_back(neighbour);
				}
			}

			sortOpenSet(openSet);
		}

		return false;
	}

}
