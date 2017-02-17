using System;
using System.Diagnostics;

#region Vector lib.

public struct Vector2
{
    public float x, y;

    public static Vector2 Zero
    {
        get
        {
            return new Vector2(0, 0);
        }
    }

    public static Vector2 One
    {
        get
        {
            return new Vector2(1, 1);
        }
    }

    public float Magnitude
    {
        get
        {
            return (float)Math.Sqrt(x * x + y * y);
        }
    }

    public float SqrMagnitude
    {
        get
        {
            return (float)(x * x + y * y);
        }
    }

    public Vector2 Normalized
    {
        get
        {
            float mag = Magnitude;

            if (mag == 0)
                return Vector2.Zero;
            else
                return new Vector2(x / mag, y / mag);
        }
    }

    public Vector2(float x, float y)
    {
        this.x = x;
        this.y = y;
    }

    public static bool operator ==(Vector2 v1, Vector2 v2)
    {
        return v1.x == v2.x && v1.y == v2.y;
    }

    public static bool operator !=(Vector2 v1, Vector2 v2)
    {
        return v1.x != v2.x || v1.y != v2.y;
    }

    public static Vector2 operator +(Vector2 v1, Vector2 v2)
    {
        return new Vector2(v1.x + v2.x, v1.y + v2.y);
    }

    public static Vector2 operator -(Vector2 v1, Vector2 v2)
    {
        return new Vector2(v1.x - v2.x, v1.y - v2.y);
    }

    public static Vector2 operator -(Vector2 vector)
    {
        return new Vector2(vector.x * -1, vector.y * -1);
    }

    public static Vector2 operator *(Vector2 v1, Vector2 v2)
    {
        return new Vector2(v1.x * v2.x, v1.y * v2.y);
    }

    public static Vector2 operator *(Vector2 vector, float scalar)
    {
        return new Vector2(vector.x * scalar, vector.y * scalar);
    }

    public static Vector2 operator *(float scalar, Vector2 vector)
    {
        return new Vector2(vector.x * scalar, vector.y * scalar);
    }

    public static Vector2 operator /(Vector2 v1, Vector2 v2)
    {
        return new Vector2(v1.x / v2.x, v1.y / v2.y);
    }

    public static Vector2 operator ++(Vector2 v1)
    {
        return v1 + Vector2.One;
    }

    public static Vector2 operator --(Vector2 v1)
    {
        return v1 - Vector2.One;
    }

    public static float Angle(Vector2 v1, Vector2 v2)
    {
        return (float)Math.Acos(Vector2.Dot(v1, v2) / (v1.Magnitude * v2.Magnitude));
    }

    public static Vector2 ClampMagnitude(Vector2 vector, float maxLength)
    {
        if (vector.Magnitude <= maxLength)
            return vector;

        return vector.Normalized * maxLength;
    }

    public static float Distance(Vector2 v1, Vector2 v2)
    {
        return (v1 - v2).Magnitude;
    }

    public static float Dot(Vector2 v1, Vector2 v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    public static Vector2 Lerp(Vector2 v1, Vector2 v2, float time)
    {
        return v1 + time * (v2 - v1);
    }

    public static Vector2 Max(params Vector2[] vectors)
    {
        Vector2 vector = Vector2.Zero;

        for (int i = 0; i < vectors.Length; i++)
        {
            vector.x = vectors[i].x > vector.x ? vectors[i].x : vector.x;
            vector.y = vectors[i].y > vector.y ? vectors[i].y : vector.y;
        }

        return vector;
    }

    public static Vector2 Min(params Vector2[] vectors)
    {
        Vector2 vector = Vector2.Zero;

        for (int i = 0; i < vectors.Length; i++)
        {
            vector.x = vectors[i].x < vector.x ? vectors[i].x : vector.x;
            vector.y = vectors[i].y < vector.y ? vectors[i].y : vector.y;
        }

        return vector;
    }

    public static Vector2 MoveTowards(Vector2 v1, Vector2 v2, float maxDistance)
    {
        float mag = (v2 - v1).Magnitude;
        float distance = mag > maxDistance ? maxDistance : mag;
        return (v2 - v1).Normalized * distance;
    }

    public static Vector2 Normalize(Vector2 vector)
    {
        return vector.Normalized;
    }

    public override bool Equals(object obj)
    {
        return base.Equals(obj);
    }

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }

    public override string ToString()
    {
        return "(" + x + ", " + y + ")";
    }
}

public struct Vector3
{
    public float x, y, z;

    public static Vector3 Zero
    {
        get
        {
            return new Vector3(0, 0, 0);
        }
    }

    public static Vector3 One
    {
        get
        {
            return new Vector3(1, 1, 1);
        }
    }

    public float Magnitude
    {
        get
        {
            return (float)Math.Sqrt(x * x + y * y + z * z);
        }
    }

    public float SqrMagnitude
    {
        get
        {
            return (float)(x * x + y * y + z * z);
        }
    }

    public Vector3 Normalized
    {
        get
        {
            float mag = Magnitude;

            if (mag == 0)
                return Vector3.Zero;
            else
                return new Vector3(x / mag, y / mag, z / mag);
        }
    }

    public Vector3(float x, float y)
    {
        this.x = x;
        this.y = y;
        z = 0;
    }

    public Vector3(float x, float y, float z)
    {
        this.x = x;
        this.y = y;
        this.z = z;
    }

    public static bool operator ==(Vector3 v1, Vector3 v2)
    {
        return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
    }

    public static bool operator !=(Vector3 v1, Vector3 v2)
    {
        return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
    }

    public static Vector3 operator +(Vector3 v1, Vector3 v2)
    {
        return new Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }

    public static Vector3 operator -(Vector3 v1, Vector3 v2)
    {
        return new Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    public static Vector3 operator -(Vector3 vector)
    {
        return new Vector3(vector.x * -1, vector.y * -1, vector.z * -1);
    }

    public static Vector3 operator *(Vector3 v1, Vector3 v2)
    {
        return new Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
    }

    public static Vector3 operator *(Vector3 vector, float scalar)
    {
        return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
    }

    public static Vector3 operator *(float scalar, Vector3 vector)
    {
        return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
    }

    public static Vector3 operator /(Vector3 v1, Vector3 v2)
    {
        return new Vector3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
    }

    public static Vector3 operator ++(Vector3 v1)
    {
        return v1 + Vector3.One;
    }

    public static Vector3 operator --(Vector3 v1)
    {
        return v1 - Vector3.One;
    }

    public static float Angle(Vector3 v1, Vector3 v2)
    {
        return (float)Math.Acos(Vector3.Dot(v1, v2) / (v1.Magnitude * v2.Magnitude));
    }

    public static Vector3 ClampMagnitude(Vector3 vector, float maxLength)
    {
        if (vector.Magnitude <= maxLength)
            return vector;

        return vector.Normalized * maxLength;
    }

    public static float Distance(Vector3 v1, Vector3 v2)
    {
        return (v1 - v2).Magnitude;
    }

    public static float Dot(Vector3 v1, Vector3 v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    public static Vector3 Lerp(Vector3 v1, Vector3 v2, float time)
    {
        return v1 + time * (v2 - v1);
    }

    public static Vector3 Max(params Vector3[] vectors)
    {
        Vector3 vector = Vector3.Zero;

        for (int i = 0; i < vectors.Length; i++)
        {
            vector.x = vectors[i].x > vector.x ? vectors[i].x : vector.x;
            vector.y = vectors[i].y > vector.y ? vectors[i].y : vector.y;
            vector.z = vectors[i].z > vector.z ? vectors[i].z : vector.z;
        }

        return vector;
    }

    public static Vector3 Min(params Vector3[] vectors)
    {
        Vector3 vector = Vector3.Zero;

        for (int i = 0; i < vectors.Length; i++)
        {
            vector.x = vectors[i].x < vector.x ? vectors[i].x : vector.x;
            vector.y = vectors[i].y < vector.y ? vectors[i].y : vector.y;
            vector.z = vectors[i].z < vector.z ? vectors[i].z : vector.z;
        }

        return vector;
    }

    public static Vector3 MoveTowards(Vector3 v1, Vector3 v2, float maxDistance)
    {
        float mag = (v2 - v1).Magnitude;
        float distance = mag > maxDistance ? maxDistance : mag;
        return (v2 - v1).Normalized * distance;
    }

    public static Vector3 Normalize(Vector3 vector)
    {
        return vector.Normalized;
    }

    public override bool Equals(object obj)
    {
        return base.Equals(obj);
    }

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }

    public override string ToString()
    {
        return "(" + x + ", " + y + ", " + z + ")";
    }
}

public struct Vector4
{
    public float x, y, z, w;

    public static Vector4 Zero
    {
        get
        {
            return new Vector4(0, 0, 0, 0);
        }
    }

    public static Vector4 One
    {
        get
        {
            return new Vector4(1, 1, 1, 1);
        }
    }

    public float Magnitude
    {
        get
        {
            return (float)Math.Sqrt(x * x + y * y + z * z + w * w);
        }
    }

    public float SqrMagnitude
    {
        get
        {
            return (float)(x * x + y * y + z * z + w * w);
        }
    }

    public Vector4 Normalized
    {
        get
        {
            float mag = Magnitude;

            if (mag == 0)
                return Vector4.Zero;
            else
                return new Vector4(x / mag, y / mag, z / mag, w / mag);
        }
    }

    public Vector4(float x, float y)
    {
        this.x = x;
        this.y = y;
        z = 0;
        w = 0;
    }

    public Vector4(float x, float y, float z)
    {
        this.x = x;
        this.y = y;
        this.z = z;
        w = 0;
    }

    public Vector4(float x, float y, float z, float w)
    {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    public Vector4(Vector2 v0, Vector2 v1)
    {
        x = v0.x;
        y = v0.y;
        z = v1.x;
        w = v1.y;
    }

    public static bool operator ==(Vector4 v1, Vector4 v2)
    {
        return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z & v1.w == v2.w;
    }

    public static bool operator !=(Vector4 v1, Vector4 v2)
    {
        return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z || v1.w != v2.w;
    }

    public static Vector4 operator +(Vector4 v1, Vector4 v2)
    {
        return new Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
    }

    public static Vector4 operator -(Vector4 v1, Vector4 v2)
    {
        return new Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
    }

    public static Vector4 operator -(Vector4 vector)
    {
        return new Vector4(vector.x * -1, vector.y * -1, vector.z * -1, vector.w * -1);
    }

    public static Vector4 operator *(Vector4 v1, Vector4 v2)
    {
        return new Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
    }

    public static Vector4 operator *(Vector4 vector, float scalar)
    {
        return new Vector4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
    }

    public static Vector4 operator *(float scalar, Vector4 vector)
    {
        return new Vector4(vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar);
    }

    public static Vector4 operator /(Vector4 v1, Vector4 v2)
    {
        return new Vector4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
    }

    public static Vector4 operator ++(Vector4 v1)
    {
        return v1 + Vector4.One;
    }

    public static Vector4 operator --(Vector4 v1)
    {
        return v1 - Vector4.One;
    }

    public static float Angle(Vector4 v1, Vector4 v2)
    {
        return (float)Math.Acos(Vector4.Dot(v1, v2) / (v1.Magnitude * v2.Magnitude));
    }

    public static Vector4 ClampMagnitude(Vector4 vector, float maxLength)
    {
        if (vector.Magnitude <= maxLength)
            return vector;

        return vector.Normalized * maxLength;
    }

    public static float Distance(Vector4 v1, Vector4 v2)
    {
        return (v1 - v2).Magnitude;
    }

    public static float Dot(Vector4 v1, Vector4 v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
    }

    public static Vector4 Lerp(Vector4 v1, Vector4 v2, float time)
    {
        return v1 + time * (v2 - v1);
    }

    public static Vector4 Max(params Vector4[] vectors)
    {
        Vector4 vector = Vector4.Zero;

        for (int i = 0; i < vectors.Length; i++)
        {
            vector.x = vectors[i].x > vector.x ? vectors[i].x : vector.x;
            vector.y = vectors[i].y > vector.y ? vectors[i].y : vector.y;
            vector.z = vectors[i].z > vector.z ? vectors[i].z : vector.z;
            vector.w = vectors[i].w > vector.w ? vectors[i].w : vector.w;
        }

        return vector;
    }

    public static Vector4 Min(params Vector4[] vectors)
    {
        Vector4 vector = Vector4.Zero;

        for (int i = 0; i < vectors.Length; i++)
        {
            vector.x = vectors[i].x < vector.x ? vectors[i].x : vector.x;
            vector.y = vectors[i].y < vector.y ? vectors[i].y : vector.y;
            vector.z = vectors[i].z < vector.z ? vectors[i].z : vector.z;
            vector.w = vectors[i].w < vector.w ? vectors[i].w : vector.w;
        }

        return vector;
    }

    public static Vector4 MoveTowards(Vector4 v1, Vector4 v2, float maxDistance)
    {
        float mag = (v2 - v1).Magnitude;
        float distance = mag > maxDistance ? maxDistance : mag;
        return (v2 - v1).Normalized * distance;
    }

    public static Vector4 Normalize(Vector4 vector)
    {
        return vector.Normalized;
    }

    public override bool Equals(object obj)
    {
        return base.Equals(obj);
    }

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }

    public override string ToString()
    {
        return "(" + x + ", " + y + ", " + z + ", " + w + ")";
    }
}

#endregion

public class Program
{
    private static Vector2 Accel(Vector2 p)
    {
        float l = 1.0f / p.Magnitude;
        return -p * (l * l * l);
    }

    private static Vector4 Eval(Vector4 u0, Vector4 v0)
    {
        Vector4 p = u0;
        Vector4 v = v0;
        const float dt = 0.1f;
        for (int i = 0; i < 10000; ++i)
        {
            Vector2 a1 = Accel(new Vector2(p.x, p.y));
            Vector2 a2 = Accel(new Vector2(p.z, p.w));
            Vector4 a = new Vector4(a1, a2);
            p += v * dt;
            v += a * dt;
        }
        return p;
    }

    public static void Main()
    {
        Stopwatch sw = new Stopwatch();
        sw.Start();

        Vector4 un0 = new Vector4(100, 100, 100, 100);
        Vector4[] in0 = new Vector4[1000000];
        for (int i = 0; i < in0.Length; ++i)
            in0[i] = new Vector4(1, 2, 3, 4);
        Vector4[] out0 = new Vector4[in0.Length];
        for (int i = 0; i < in0.Length; ++i)
        {
            out0[i] = Eval(un0, in0[i]);
        }

        sw.Stop();
        Console.Out.WriteLine(string.Format("{0}ms cost.", sw.ElapsedMilliseconds));

        Console.In.Read();
    }
}
