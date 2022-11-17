#include "solver.h"

Solver::Solver(const std::vector<Border>& inBorders, const std::vector<Node>& inNodes, const std::vector<Material>& inParticles){
    borders = inBorders;
    particles = inParticles;
    nodes = inNodes;

    blen = borders.size();
    ilen = nodes.size();
}

void Solver::P2G(){
    plen = particles.size();

    //can use cuda to accelerate
    for(int p = 0; p < plen; ++p){
        particles[p].ConstitutiveModel();

        //get index of the bottom-left node
        int bottom_left_node = (X_GRID + 1) * static_cast<int>(particles[p].Xp[1] - Translation_xp[1])  + static_cast<int>(particles[p].Xp[0] - Translation_xp[0]);

        for(int y = bni; y < 3; ++y){
            for(int x = bni; x < 3; ++x){
                int node_id = bottom_left_node + x + (X_GRID + 1) * y;

                Vector2f distance = particles[p].Xp - nodes[node_id].Xi;
                double Wip = getWip(distance);
                Vector2f dWip = getdWip(distance);

                //APIC
                double inMi = Wip * particles[p].Mp;

                //momentum
                Vector2f inVi = Wip * particles[p].Mp * (particles[p].Vp + Dp_scal * H_INV * H_INV * particles[p].Bp * (-distance));
                Vector2f inFi = particles[p].Ap * dWip;
                
                nodes[node_id].Mi += inMi;
                nodes[node_id].Fi[0] += inFi[0];
                nodes[node_id].Fi[1] += inFi[1];
                nodes[node_id].Vi[0] += inVi[0];
                nodes[node_id].Vi[1] += inVi[1];  
            }
        }
    }
}

void Solver::UpdateNodes(){
    for(int i = 0; i < ilen; ++i){
        if(nodes[i].Mi > 0){
            nodes[i].Vi /= nodes[i].Mi;
            nodes[i].Fi = DT * (-nodes[i].Fi / nodes[i].Mi + G);
            nodes[i].Vi += nodes[i].Fi;
    		nodes[i].NodeCollision();
			#if FRICTION
			nodes[i].NodeFrictions();
			#else
			nodes[i].Vi_fri = nodes[i].Vi_col;
			#endif
        }    
    }
}

void Solver::G2P()				
{
	for (int p = 0; p < plen; p++)
	{		
		// Index of bottom-left node closest to the particle
		int node_base =
			(X_GRID + 1) * static_cast<int>(particles[p].Xp[1] - Translation_xp[1])
			+ static_cast<int>(particles[p].Xp[0] - Translation_xp[0]);

		// Set velocity and velocity field to 0 for sum update
		particles[p].Vp.setZeros();
		particles[p].Bp.setZeros();

		// Loop over all the close nodes (depend on interpolation through bni)
		for (int y = bni; y < 3; y++) {
			for (int x = bni; x < 3; x++){
				// Index of the node
				int node_id = node_base + x + (X_GRID + 1) * y;
				// Distance and weight
				Vector2f dist = particles[p].Xp - nodes[node_id].Xi;
                
				double Wip = getWip(dist);
				
				// Update velocity and velocity field (APIC)
				particles[p].Vp += Wip * nodes[node_id].Vi_fri;
				particles[p].Bp += Wip * (nodes[node_id].Vi_fri.outer_product(-dist));
			}
		}
	}
}

void Solver::UpdateParticles(){
    for(int p = 0; p < plen; ++p){
        int bottom_left_node = (X_GRID + 1) * static_cast<int>(particles[p].Xp[1] - Translation_xp[1]) + static_cast<int>(particles[p].Xp[0] - Translation_xp[0]);
        
        Vector2f Xp_temp = particles[p].Xp;
        particles[p].Xp.setZeros();
        Matrix2f T;

        for(int y = bni; y < 3; ++y){
            for(int x = bni; x < 3; ++x){
                int node_id = bottom_left_node + (X_GRID + 1) * y + x;
                Vector2f distance = Xp_temp - nodes[node_id].Xi;
                double Wip = getWip(distance);
                Vector2f dWip = getdWip(distance);

                particles[p].Xp += Wip * (nodes[node_id].Xi + DT * nodes[node_id].Vi_col);
                T += nodes[node_id].Vi_col.outer_product(dWip);
            }           
        }
        particles[p].updateDeformation(T);
    }
}


void Solver::ResetGrid(){
    for(int i = 0; i < ilen; ++i){
        if(nodes[i].Mi > 0){
            nodes[i].ResetNode();
        }
    }
}
