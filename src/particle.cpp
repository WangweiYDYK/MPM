#include <windows.h>
#include"particle.h"

Particle::Particle(const double inVp0, const double inMp, const Vector2f& inXp, const Vector2f& inVp, const Matrix2f& inBp){
    Mp = inMp;
    Vp0 = inVp0;
    Xp = inXp;
    Vp = inVp;
    Bp = inBp;
}

//water
Water::Water(const double inVp0, const double inMp, const Vector2f& inXp, const Vector2f& inVp, const Matrix2f& inBp):
    Particle(inVp0, inMp, inXp, inVp, inBp)
{
    Ap = 0.0;
	Jp = 1.0;
}

Water::Water(Particle p)
	: Particle(p.Vp0, p.Mp, p.Xp, p.Vp, p.Bp)
{
	Ap = 0.0;
	Jp = 1.0;
}

void Water::ConstitutiveModel(){
    double dJp = -K_water*(1.0 / pow(Jp, GAMMA_water) - 1.0);
    Ap = dJp * Vp0 * Jp;
}

void Water::updateDeformation(const Matrix2f& T){
    Jp = (1 + DT * T.trace()) * Jp;
}


