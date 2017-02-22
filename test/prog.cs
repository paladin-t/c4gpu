/*
** The C4GPU (C for GPU) System.
**
** For the latest info, see https://github.com/c4gpu/c4gpu/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
**
** The C# eval test.
*/

using System;
using System.Diagnostics;
using System.Threading.Tasks;

public class Program
{
    public struct Vector2
    {
        public float x, y;

        public Vector2(float _x, float _y)
        {
            x = _x; y = _y;
        }
    }

    public struct Vector4
    {
        public float x, y, z, w;

        public Vector4(float _x, float _y, float _z, float _w)
        {
            x = _x; y = _y; z = _z; w = _w;
        }
    }

    private unsafe static float vec2_length(Vector2* p)
    {
        return (float)Math.Sqrt(p->x * p->x + p->y * p->y);
    }

    private unsafe static void vec2_mul_float(Vector2* p, float q)
    {
        p->x *= q;
        p->y *= q;
    }

    private unsafe static void vec2_mul_vec2(Vector2* p, Vector2* q)
    {
        p->x *= q->x;
        p->y *= q->y;
    }

    private unsafe static void vec4_add_vec4(Vector4* p, Vector4* q)
    {
        p->x += q->x;
        p->y += q->y;
        p->z += q->z;
        p->w += q->w;
    }

    private unsafe static void vec4_mul_float(Vector4* p, float q)
    {
        p->x *= q;
        p->y *= q;
        p->z *= q;
        p->w *= q;
    }

    private unsafe static void accel(Vector2* o, Vector2* p)
    {
        float l = 1.0f / vec2_length(p);
        vec2_mul_float(o, -1 * (l * l * l));
    }

    private unsafe static void eval(Vector4* o, Vector4* u0, Vector4* v0)
    {
        Vector4 p = *u0;
        Vector4 v = *v0;
        const float dt = 0.1f;
        for (int i = 0; i < 10000; ++i)
        {
            Vector2 a1, a2;
            Vector2 m = new Vector2(p.x, p.y);
            Vector2 n = new Vector2(p.z, p.w);
            accel(&a1, &m);
            accel(&a2, &n);
            Vector4 a = new Vector4(a1.x, a1.y, a2.x, a2.y);
            Vector4 _v = v;
            vec4_mul_float(&_v, dt);
            vec4_add_vec4(&p, &_v);
            vec4_mul_float(&a, dt);
            vec4_add_vec4(&v, &a);
        }
        *o = p;
    }

    public unsafe static void Main()
    {
        // Prepares.
        Console.Out.WriteLine("Preparing...");
        Vector4 un0 = new Vector4(100, 100, 100, 100);
        Vector4[] in0 = new Vector4[1000000];
        for (int i = 0; i < in0.Length; ++i)
        {
            in0[i] = new Vector4(1, 2, 3, 4);
        }
        Vector4[] out0 = new Vector4[in0.Length];

        // Computes.
        Console.Out.WriteLine("Begin compute...");
        Stopwatch sw = new Stopwatch();
        sw.Start();

        for (int i = 0; i < in0.Length; ++i)
        {
            fixed (Vector4* a = &out0[i], b = &in0[i])
            {
                eval(a, &un0, b);
            }
        }

        sw.Stop();
        Console.Out.WriteLine(string.Format("{0}ms cost.", sw.ElapsedMilliseconds));

        // Done.
        Console.Out.WriteLine("Done.");

        Console.In.Read();
    }
}
