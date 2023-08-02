#version 460
layout(local_size_x = 1, local_size_y = 1) in;
layout(rgba32f, binding = 0) uniform image2D screenImageData;
layout(location = 0) uniform float t;

uniform vec3 camPos = vec3(0, 2.0, -5.0);
uniform vec3 camUp = vec3(0,1.0,0);
uniform vec3 camRight = vec3(1.0,0,0);
uniform vec3 camAt = vec3(0,0,1.0);


struct Plane
{
    vec3 normal;
    vec3 d;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Sphere
{
    vec3 center;
    float radius;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 pos;
    vec3 color;
    float power;
};

#define NUM_SPHERES 2
#define NUM_PLANES 1
#define NUM_LIGHTS 1

float FindMinimum(float[4] t)
{
    float min = 9999999.0;

    for (int i = 0; i < 4; i++)
    {
        if (t[i] < min)
            min = t[i];
    }

    return min;
}

const Sphere spheres[] =
{
    { vec3(0.0,0.0,5.0), 3.0, vec3(0.5,0.0,0.0), vec3(0.2,0.0,0.0), vec3(0.5,0.5,0.5), 600.0},
    { vec3(-6.0,0.0,5.0), 2.0, vec3(0.0,0.0,0.5), vec3(0.0,0.0,0.2), vec3(0.5,0.5,0.5), 500.0 }
};

const Plane planes[] =
{
    // Y axis plane
    {vec3(0.0,1.0,0.0), vec3(0.0,-3.0,0.0), vec3(0.5,0.5,0.5), vec3(0.5,0.5,0.5), vec3(0.5,0.5,0.5), 10.0 },
    { vec3(0.0,0.0,-1.0), vec3(0.0,0.0,8.0), vec3(0.5,0.5,0.5), vec3(0.5,0.5,0.5), vec3(0.5,0.5,0.5), 10.0 }
};

Light lights[] =
{
    { vec3(-1.0,-2.0,-5.0), vec3(1.0,1.0,1.0), 15.0 },
    { vec3(-1.0,-2.0,-3.0), vec3(0.0,0.0,1.0), 30 }
};

float IntersectPlane(vec3 rayPos, vec3 rayDir, Plane p)
{
    float denom = dot(p.normal, rayDir);

    if (abs(denom) > 0.001f)
    {
        float t = dot((p.d - rayPos), p.normal) / denom;
        if (t >= 0)
            return t;
    }

    return 9999999.0;
}

vec3 SphereNormal(Sphere s, vec3 pt)
{
    vec3 n = pt - s.center;
    n = normalize(n);
    return n;
}

float IntersectSphere(vec3 origin, vec3 direction, Sphere s)
{
    vec3 eyetocenter = s.center - origin;
    float d = dot(eyetocenter, direction);
    float eyedot = dot(eyetocenter, eyetocenter);
    float radsqrd = s.radius * s.radius;
    float disc = d * d - (eyedot - radsqrd);

    if (disc < 0.0)
        return 9999999.0;
    else
        return (d - sqrt(disc));
}

vec3 Shade(vec3 rayPos, vec3 rayDir, vec3 inter, vec3 normal, int type)
{
    // find ray from light to intersection
    vec3 l = lights[0].pos - inter;
    float distance = length(l);
    l = normalize(l);
    float dotln = dot(normal, l);

    vec3 ndotln = normal * dotln;
    vec3 dndotln = ndotln * 2.0;
    vec3 r = l - dndotln;
    r = normalize(r);
    vec3 v = inter - camPos;
    v = normalize(v);
    float dotrv = dot(r, v);

    float lambert = max(dotln, 0.0);
    float specular = 0.0;
    float shininess;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specColor;
    vec3 lightColor = lights[0].color;
    float lightPow = lights[0].power;

    switch (type)
    {
        case 0:
            ambientColor = planes[0].ambient;
            diffuseColor = planes[0].diffuse;
            specColor = planes[0].specular;
            shininess = planes[0].shininess;
            break;
        case 1:
            ambientColor = spheres[0].ambient;
            diffuseColor = spheres[0].diffuse;
            specColor = spheres[0].specular;
            shininess = spheres[0].shininess;
            break;
        case 2:
            ambientColor = spheres[1].ambient;
            diffuseColor = spheres[1].diffuse;
            specColor = spheres[1].specular;
            shininess = spheres[1].shininess;
            break;
        case 3:
            ambientColor = planes[1].ambient;
            diffuseColor = planes[1].diffuse;
            specColor = planes[1].specular;
            shininess = planes[1].shininess;
            break;
        default:
            break;
    }

    if (lambert > 0.0)
    {
        vec3 hd = normalize(l + v);
        float specAngle = max(dot(hd, normal), 0.0);
        specular = pow(specAngle, shininess);
    }

    // calculate light equation for light ray hitting the surface
    vec3 rayColor = ambientColor + (diffuseColor * lambert * lightColor * lightPow / distance) + (specColor * specular * lightColor * lightPow / distance);

    return rayColor;
}

vec3 Trace(vec3 rayPos, vec3 rayDir)
{
    vec3 col = vec3(0.0, 0.0, 0.0);

    float t[4];
    t[0] = IntersectPlane(rayPos, rayDir, planes[0]);
    t[1] = IntersectPlane(rayPos, rayDir, planes[1]);
    t[2] = IntersectSphere(rayPos, rayDir, spheres[0]);
    t[3] = IntersectSphere(rayPos, rayDir, spheres[1]);

    float closest;

    // find the smallest distance from camera
    closest = FindMinimum(t);

    vec3 norm, hitDir, hitPt;

    hitDir = rayDir * closest;
    hitPt = rayPos + hitDir;

    if (closest == t[0])
    {
        norm = planes[0].normal;
        // ambient light + reflected ligh
        col = Shade(rayPos, rayDir, hitPt, norm, 0);
    }
    else if (closest == t[2])
    {
        norm = SphereNormal(spheres[0], hitPt);
        col = Shade(rayPos, rayDir, hitPt, norm, 1);
    }
    else if (closest == t[3])
    {
        norm = SphereNormal(spheres[1], hitPt);
        col = Shade(rayPos, rayDir, hitPt, norm, 2);
    }
    else if (closest == t[1])
    {
        norm = planes[1].normal;
        col = Shade(rayPos, rayDir, hitPt, norm, 3);
    }
    else // the ray has hit the background
        return vec3(0, 0, 0);

    return col;
}

void main()
{
    ivec2 xy = ivec2(gl_GlobalInvocationID.xy);
    ivec2 dim = imageSize(screenImageData);

    // calculate inverse of projection 
    float px = (float(xy.x * 2 - dim.x) / float(dim.x));
    float py = (float(xy.y * 2 - dim.y) / float(dim.y));
    // align with camera
    vec3 kx = camRight * px;
    vec3 ky = camUp * py;
    vec3 kxky = kx + ky;
    vec3 dir = camAt + kxky;
    dir = normalize(dir);

    // animate the light
    lights[0].pos.x = lights[0].pos.x + 5.0 * cos(t);
    lights[0].pos.z = lights[0].pos.z + 5.0 * sin(t);
    vec3 c = Trace(camPos, dir); 
    imageStore(screenImageData, xy, vec4(c.rgb, 1.0));
}
