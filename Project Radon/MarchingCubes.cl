inline float3 VertexInterp(float IsoValue, float3 p1, float3 p2, float v1, float v2) {

    double mu;

    float3 p;

    if(abs((float)(IsoValue - v1)) < 0.00001) {
        return(p1);
    }

    if(abs((float)(IsoValue - v2)) < 0.00001) {
        return(p2);
    }

    if(abs((float)(v1 - v2)) < 0.00001) {
        return(p1);
    }

    mu = (IsoValue - v1)/(v2 - v1);
    p.x = p1.x + mu * (p2.x - p1.x);
    p.y = p1.y + mu * (p2.y - p1.y);
    p.z = p1.z + mu * (p2.z - p1.z);

    return p;

}


void kernel MarchingCubes(global float* voxels, global float* settings, global int* edgeTable, global int* triTable, global float* vertices) {


    int x1 = get_global_id(0);
    int y1 = get_global_id(1);
    int z1 = get_global_id(2);


    float x = (get_global_id(0) * settings[3]) + settings[4];
    float y = (get_global_id(1) * settings[3]) + settings[5];
    float z = (get_global_id(2) * settings[3]) + settings[6];

    float3 corners[8];
    corners[0] = (float3)(x,y,z);
    corners[1] = corners[0] + (float3)(settings[3],0,0);
    corners[2] = corners[0] + (float3)(settings[3],settings[3],0);
    corners[3] = corners[0] + (float3)(0,settings[3],0);

    corners[4] = corners[0] + (float3)(0,0,settings[3]);
    corners[5] = corners[0] + (float3)(settings[3],0,settings[3]);
    corners[6] = corners[0] + (float3)(settings[3],settings[3],settings[3]);
    corners[7] = corners[0] + (float3)(0,settings[3],settings[3]);


    float vals[8];

    int cubeIndex = 0;
    if(vals[0] < settings[7]) cubeIndex |= 1;
    if(vals[1] < settings[7]) cubeIndex |= 2;
    if(vals[2] < settings[7]) cubeIndex |= 4;
    if(vals[3] < settings[7]) cubeIndex |= 8;
    if(vals[4] < settings[7]) cubeIndex |= 16;
    if(vals[5] < settings[7]) cubeIndex |= 32;
    if(vals[6] < settings[7]) cubeIndex |= 64;
    if(vals[7] < settings[7]) cubeIndex |= 128;






}