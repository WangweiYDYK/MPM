1.P2G

对于格点i在时刻n：
$$m_i^{\alpha,n}=\Sigma_pw_{ip}^{\alpha,n}m_p^\alpha$$
$$\mathbf{v}_{i}^{\alpha, n}=\frac{1}{m_{i}^{\alpha, n}} \sum_{n} w_{i p}^{\alpha, n} m_{p}^{\alpha}\left(\mathbf{v}_{p}^{\alpha, n}+\mathrm{C}_{p}^{\alpha, n}\left(\mathbf{x}_{i}^{\alpha, n}-\mathbf{x}_{p}^{\alpha, n}\right)\right),$$


$\hat{\mathbf{x}_{i}^{w}}$是网格节点在原始位置$\mathbf{x}_{i}^{w}$在接受动量$\Delta tv_i^{\alpha,n+1}$后形变后的位置，因此：
$$\hat{\mathbf{x}_{i}^{w}}=\mathbf{x}_{i}^{w}+\Delta t \mathbf{v}_i^{\alpha,n+1}$$
$$\mathbf{f}_{i}^{w}\left(\hat{\mathbf{x}}^{w}\right)=-\frac{\partial \psi^{w}}{\partial \hat{\mathbf{x}}_{i}^{w}}=-\sum_{p} V_{p}^{0}\left(\frac{\partial \psi^{w}}{\partial J^{w}}\left(J^{w}\left(\hat{\mathbf{x}}^{w}\right)\right)\right) J^{w, n} \nabla w_{i p}^{w, n}$$

对于液体其能量可以表示为：
$$\psi^{w}(J^w)=-k(\frac{(J^w)^{1-\gamma}}{1-\gamma}-J^w)$$
则其偏导有：
$$p^w=-\frac{\partial\psi^w}{\partial J^w}(J^w)=k(\frac{1}{J^{w\gamma}}-1)$$

不妨令$A_p =  V_{p}^{0}p^w J^{w, n}$
```
void Water::ConstitutiveModel(){
    double dJp = -K_water*(1.0 / pow(Jp, GAMMA_water) - 1.0);
    Ap = dJp * Vp0 * Jp;
}
```
```
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
```
2.update nodes

mpm作为一种伽辽金方法，其边界条件必须在网格上进行
一般有sticky, slip, seperate三种边界条件

sticky边界条件下，粒子会在边界粘滞，速度为0
$$V_i^{n+1}=BC_{sticky}(\hat{V}_i^{n+1})=0$$
slip边界条件下，粒子在边界上法线方向上的速度归零，切向速度不变，粒子会在边界上沿着边界光滑地滑动
$$V_i^{n+1}=BC_{slip}(\hat{V}_i^{n+1})=\hat{V}_i^{n+1}-n(n^T\hat{V}_i^{n+1})$$
$$V_i^{n+1}=BC_{seperate}(\hat{V}_i^{n+1})=\hat{V}_i^{n+1}-n \ min(n^T\hat{V}_i^{n+1},0)$$


3.update particles

对于若可压缩液体，其体积比就决定了其内部的压强
$$J_P=V_p^n/V_P^0=det(\mathbf{F^n_p})$$
然而若直接使用$det(\mathbf{F^n_p})$进行计算，容易发生灾难性消除，因此选择每时刻更新，更新方法推导如下：
$$\mathbf{F}_p^{n+1}=(\mathbf{I}+\Delta t\mathbf{C}_p^n)\mathbf{F}_p^{n}$$
$$det(\mathbf{F}_p^{n+1})=det(\mathbf{I}+\Delta t\mathbf{C}_p^n)det(\mathbf{F}_p^{n})$$
$$$$
$$$$
$$J_{p}^{w,n+1}=(I+\Delta t\ tr(\mathbf{C}_p^n))J^{w,n}$$

