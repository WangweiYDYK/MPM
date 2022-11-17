#include"node.h"

Node::Node(const Vector2f& inXi)
{
	Mi = 0.0;

	Xi = inXi;
	Vi.setZeros();
	Vi_col.setZeros();
	Vi_fri.setZeros();

	Fi.setZeros();
}

std::vector<Border> Node::borders = Border::InitializeBorders();
size_t Node::blen = Node::borders.size();

void Node::NodeCollision(){
    Vi_col = Vi;
    for(int b = 0; b < Node::blen; ++b){
        borders[b].Collision(Xi, Vi_col, CollisionObjects, b);
    }
}

void Node::NodeFriction(){
    Vi_fri = Vi_col;
    for(size_t i = 0, iLen = CollisionObjects.size(); i < iLen; ++i){
        borders[CollisionObjects[i]].Friction(Vi_fri, Vi_col, Vi);
    }
}

void Node::ResetNode()
{
	Mi = 0.0;
	Vi.setZeros();
	Fi.setZeros();
	CollisionObjects.clear();
}


void Node::DrawNode(){

}