#pragma once
#include <windows.h>
#include <vector>
#include "constants.h"
#include "vector.h"
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

//basic attribution of particle
class Particle{
public:

    //attribution
    double Mp;           //mass of particle
    double Vp0;          //volume of particle
    
    Vector2f Xp;       //position of particle
    Vector2f Vp;       //velocity of particle
    Matrix2f Bp;

    //constructor
    Particle();
    Particle(const double inVp0, const double inMp, const Vector2f& inXp, const Vector2f& inVp, const Matrix2f& inBp);
    ~Particle() {};
};


//water
class Water : public Particle{
public:
    double Jp;                                              //deformation gradient
    double Ap;
    
    //constructors
    Water() : Particle() {};
    Water(const double inVp0, const double inMp, const Vector2f& inXp, const Vector2f& inVp, const Matrix2f& inBp);
    Water(Particle p);
    ~Water() {};

    //function
    void ConstitutiveModel();
    void updateDeformation(const Matrix2f& T);

    void drawParticle();


    //static functions
    static std::vector<Water> InitializeParticles(){
        std::vector<Water> outParticles;
        return outParticles;
    }

	static std::vector<Water> AddParticles()				// Add particle mid-simulation
	{
		std::vector<Water> outParticles;
		Vector2f v = Vector2f(30, 0);						// Initial velocity
		Matrix2f a = Matrix2f(0);

		for (int p = 0; p < 8; p++)
		{
			double r = ((double)rand() / (RAND_MAX));			// random number

			Vector2f pos = Vector2f((double)CUB, (double)(Y_GRID - 2 * CUB - 0.5*p - r));		// new positions
			outParticles.push_back(Particle(1.14, 0.0005, pos, v, a));	
		}

		return outParticles;
	}

};