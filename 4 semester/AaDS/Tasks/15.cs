using System;
using System.Collections.Generic;

class Program
{
    static void Main(string[] args)
    {
        string line = Console.ReadLine();
        int len = line.Length;
        int n = Convert.ToInt32(Console.ReadLine());
        if (n == 0)
        {
            Console.WriteLine(line);
            return;
        }
        int[] start = new int[len];
        int[] end = new int[len];
        int counter = 0;

        for (int i = 0; i < n; i++)
        {
            string[] request = Console.ReadLine().Split(' ');
            int s = Convert.ToInt32(request[0]);
            int e = Convert.ToInt32(request[1]);
            if (s > e)
            {
                int temp = s;
                s = e;
                e = temp;
            }
            start[s - 1] += 1;
            end[e - 1] += 1;
        }

        for (int i = 0; i < len; i++)
        {
            counter += start[i];
            if (counter % 2 != 0)
            {
                if (Char.IsLower(line[i]))
                {
                    Console.Write(Char.ToUpper(line[i]));
                }
                else Console.Write(Char.ToLower(line[i]));
            }
            else Console.Write(line[i]);
            counter -= end[i];
        }
        return;
    }
}