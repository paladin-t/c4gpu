using System;
using System.Diagnostics;

public class Program
{
    public static void Main()
    {
        Stopwatch sw = new Stopwatch();
        sw.Start();
        Console.WriteLine("Hello, world!");
        sw.Stop();
        Console.Out.WriteLine(string.Format("{0}ms cost.", sw.ElapsedTicks));
    }
}
