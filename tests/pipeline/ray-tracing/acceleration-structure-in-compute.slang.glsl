#version 460
#extension GL_EXT_ray_tracing : require
layout(row_major) uniform;
layout(row_major) buffer;
int helper_0(accelerationStructureEXT a_0, int b_0)
{
    return b_0;
}

layout(binding = 1)
uniform accelerationStructureEXT entryPointParams_x_0;

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
void main()
{
    int _S1 = helper_0(entryPointParams_x_0, 1);
    return;
}
