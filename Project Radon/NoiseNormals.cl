void kernel NoiseNormals(global float* voxels, global const float* settings, global float3* norms) {

    int x = get_global_id(0);
    int y = get_global_id(1);
    int z = get_global_id(2);

    float v = voxels[x + (y * ((int)settings[0] + 1)) + (z * ((int)settings[0] + 1) * ((int)settings[1] + 1))];

    float dx = v - voxels[(x + 1) + (y * ((int)settings[0] + 1)) + (z * ((int)settings[0] + 1) * ((int)settings[1] + 1))];
    float dy = v - voxels[x + ((y + 1) * ((int)settings[0] + 1)) + (z * ((int)settings[0] + 1) * ((int)settings[1] + 1))];
    float dz = v - voxels[x + (y * ((int)settings[0] + 1)) + ((z + 1) * ((int)settings[0] + 1) * ((int)settings[1] + 1))];

    norms[x + (y * ((int)settings[0] + 1)) + (z * ((int)settings[0] + 1) * ((int)settings[1] + 1))] = (float3)(1,1,1);//normalize((float3)(dx,dy,dz));


}