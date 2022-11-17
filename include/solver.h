#include "particle.h"
#include "node.h"

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
class Solver{
public:
    std::vector<Border> borders;
    std::vector<Material> particles;
    std::vector<Node> nodes;

    size_t ilen, plen, blen;
    Solver() {};
    Solver(const std::vector<Border>& inBorders, const std::vector<Node>& inNodes, const std::vector<Material>& inParticles);
    ~Solver(){};


    void P2G();
    void UpdateNodes();
    void G2P();
    void UpdateParticles();
    void ResetGrid();

    void Draw();

    static double Bspline(double x){
        double W;
        x = fabs(x);

        if(x < 1){
            W = (x*x*x / 2.0 - x * x + 2 / 3.0);
        }
        else if(x < 2){
            W = (2 - x)*(2 - x)*(2 - x) / 6.0;
        }
        else
            W = 0;

        return W;
    }
    
    static double dBspline(double x){
        double dW;
        double x_fabs = fabs(x);
        if(x_fabs < 1){
            dW = x * x_fabs * 1.5 - 2.0 * x;
        }
        else if(x_fabs < 2.0){
            dW = -x * x_fabs / 2.0 + 2 * x - 2 * x / x_fabs;
        }
        else
            dW = 0;

        return dW;
    }

    static double getWip(const Vector2f& distance){
        return Bspline(distance[0]) * Bspline(distance[1]);
    }

	static Vector2f getdWip(const Vector2f& dist)	// 2D weight gradient
	{
		return Vector2f(
			dBspline(dist[0]) * Bspline(dist[1]),
			Bspline(dist[0]) * dBspline(dist[1]));
	}
};