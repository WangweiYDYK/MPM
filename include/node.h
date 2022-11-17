#include "vector.h"
#include "border.h"

class Node{
public:
    double Mi;          //mass of the node
    
    Vector2f Xi;       //position of the node
    Vector2f Vi;       //velocity of the node
    Vector2f Vi_col;
    Vector2f Vi_fri;
    Vector2f Fi;


    std::vector<int> CollisionObjects;

    void NodeCollision();
    void NodeFriction();
    void DrawNode();
    void ResetNode();

    //constructor
    Node() {};
    Node(const Vector2f& inXi);
    ~Node() {};

	static std::vector<Border> borders;					// All the borders of the domain
	static size_t blen;

	static std::vector<Node> InitializeNodes()			// Initialize Nodes
	{
		std::vector<Node> outNodes;

		for (int y = 0; y <= Y_GRID; y++)
			for (int x = 0; x <= X_GRID; x++)
				outNodes.push_back(Node(Vector2f((double)x, (double)y)));

		return outNodes;
	}
};