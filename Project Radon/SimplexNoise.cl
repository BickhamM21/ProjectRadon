
__constant float F3 =  0.3333333;
__constant float G3 =  0.1666667;



inline float3 random3(float3 c) {

    float i;

    float j = 4096.0 * sin(dot(c, (float3)(17, 59, 15)));
    float3 r;
    r.z = fract(512 * j, &i);
    j *= .125;
    r.x = fract(512.0*j, &i);
    j *= .125;
	r.y = fract(512.0*j, &i);

    return (float3)(r.x - 0.5, r.y - 0.5, r.z - 0.5) ;

}


inline float simplex3d(float3 p) {

    p /= 90;

    float3 s = floor(p + dot(p, (float3)(F3,F3,F3)));
    float3 x = p - s + dot(s, (float3)(G3,G3,G3));

    float3 e = step((float3)(0.0,0.0,0.0), x - x.yzx);
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

    w = max(0.6f - w, 0.0f);

    d.x = dot(random3(s), x);
    d.y = dot(random3(s + i1), x1);
    d.z = dot(random3(s + i2), x2);
    d.w = dot(random3(s + 1), x3);

    w *= w;
    w *= w;
    d *= w;

    /* 3. return the sum of the four surflets */
    return dot(d, (float4)(52.0f,52.0f,52.0f,52.0f)) * 25600;


}

inline float Circle(float3 p) {
    float radius = 125;

    float z = p.z;
    float y = p.y + radius - 10;
    float x = p.x;

    float circle = (x * x + y * y + z * z - (radius * radius));

    return circle;

}

inline float simplex3d_fractal(float3 m) {


    return   (0.5333333*simplex3d(m)
    			+0.2666667*simplex3d(2*m)
    			+0.1333333*simplex3d(4*m)
    			+0.0666667*simplex3d(8*m)
    			+0.0366667*simplex3d(16*m)
    			+0.0166667*simplex3d(32*m)
    			+0.0086667*simplex3d(64*m)
    			+0.0046667*simplex3d(128*m))
    			+ Circle(m);
}

/*float radius = 125;

    float z = pos.z - 8; //+ cos(glfwGetTime()) * 5;
    float y = pos.y + radius - 10;
    float x = pos.x - 8; //+ sin(glfwGetTime()) * 5;


    float circle = (x * x + y * y + z * z - (radius * radius));



    return circle;*/

void kernel SimplexNoise(global float* voxels, global const float* settings, global float3* norms) {


    int x = get_global_id(0);
    int y = get_global_id(1);
    int z = get_global_id(2);

    float x1 = (x * settings[3]) + settings[4];
    float y1 = (y * settings[3]) + settings[5];
    float z1 = (z * settings[3]) + settings[6];



    voxels[x + (y * ((int)settings[0] + 1)) + (z * ((int)settings[0] + 1) * ((int)settings[1] + 1))] = simplex3d_fractal((float3)(x1,y1,z1));//x1 * x1 + y1 * y1 + z1 * z1 - 36;


}