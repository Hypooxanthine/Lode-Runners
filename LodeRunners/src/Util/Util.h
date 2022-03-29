#pragma once

#include <vector>
#include <unordered_map>
#include <string>

namespace Data
{
	class Node;

	struct NodePosition
	{
		int x = 0, y = 0;
	};

	class Node
	{
	public:
		Node() = default;
		Node(const NodePosition& pos);

		const NodePosition& getPosition() const;
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
		NodePosition m_Pos;
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

		void addNode(const NodePosition& pos);

		void addEdge(const NodePosition& from, const NodePosition& to);

		bool contains(const NodePosition& pos);

		Node* getNode(const NodePosition& pos);
		const Node* getNode(const NodePosition& pos) const;

		std::vector<Node*>& getNeighbours(const NodePosition& node);
		const std::vector<Node*>& getNeighbours(const NodePosition& node) const;

	private:
		std::string nodePosToStr(const NodePosition& pos) const;

	private:
		std::unordered_map<std::string, Node> m_Nodes;
	};

}

namespace AI
{
	bool ComputeAStar(Data::AStarGraph graph, const Data::NodePosition& startPos, const Data::NodePosition& goalPos, std::vector<Data::Node>& path);
}