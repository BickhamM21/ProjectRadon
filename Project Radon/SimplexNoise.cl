
__constant float F3 =  0.3333333;
__constant float G3 =  0.1666667;



inline float3 random3(float3 c) {

    float i;

    float j = 4096.0 * sin(dot(c, float3(17, 59, 15)));
    float3 r;
    r.z = fract(512 * j, &i);
    j *= .125;
    r.x = fract(512.0*j, &i);
    j *= .125;
	r.y = fract(512.0*j, &i);

    return float3(r.x - 0.5, r.y - 0.5, r.z - 0.5) ;

}


inline float simplex3d(float3 p) {

    float3 s = floor(p + dot(p, float3(F3)));
    float3 x = p - s + dot(s, float3(G3));

    float3 e = step(float3(0.0), x - x.yzx);
    float3 i1 = e*(1 - e.zxy);
    float3 i2 = 1 - e.zxy*(1 - e);


    float3 x1 = x - i1 + G3;
	float3 x2 = x - i2 + 2*G3;
	float3 x3 = x - 1 + 3*G3;

    float4 w, d;

    w.x = dot(x, x);
    w.y = dot(x1, x1);
    w.z = dot(x2, x2);
    w.w = dot(x3, x3);

    w = max(6 - w, 0.0f);

    d.x = dot(random3(s), x);
    d.y = dot(random3(s + i1), x1);
    d.z = dot(random3(s + i2), x2);
    d.w = dot(random3(s + 1), x3);

    w *= w;
    w *= w;
    d *= w;

    /* 3. return the sum of the four surflets */
    return dot(d, float4(52.0f));


}


inline float simplex3d_fractal(float3 m) {
    return   0.5333333*simplex3d(m)
			+0.2666667*simplex3d(2*m)
			+0.1333333*simplex3d(4*m)
			+0.0666667*simplex3d(8*m);
}

void kernel SimplexNoise(global float* voxels, const int ChunkSizeX, const int ChunkSizeY, const int ChunkSizeZ, const float gridSize) {


    int x = get_global_id(0);
    int y = get_global_id(1);
    int z = get_global_id(2);




    voxels[(z * (ChunkSizeX + 1) * (ChunkSizeY + 1)) + (y * (ChunkSizeX + 1)) + x] = simplex3d_fractal(float3(x * gridSize,y * gridSize,z * gridSize));


}