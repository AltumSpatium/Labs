using System;
using System.Collections.Generic;

class Program
{
    static void Main(string[] args)
    {
        string[] inp = Console.ReadLine().Split(' ');
        int n = Convert.ToInt32(inp[0]), k = Convert.ToInt32(inp[1]);
        string[] line = Console.ReadLine().Split(' ');
        List<long> sequence = new List<long>();
        for (int i = 0; i < n; i++)
        {
            sequence.Add(Convert.ToInt64(line[i]));
        }
        sequence.Sort();
        long left = 0, right = 0, lmul = 0, rmul = 0, mul = 1;
        int l_index = 0, r_index = n - 1;
        bool negisbigger = false;
        long module = 1000000000 + 7;
        if (k % 2 != 0)
        {
            mul = (mul * sequence[r_index--]) % module;
            k--;
        }
        while (k != 0)
        {
            right = sequence[r_index - 1] * sequence[r_index];
            left = sequence[l_index] * sequence[l_index + 1];

            if (left >= right)
                negisbigger = true;
            else negisbigger = false;

            right = right % module;
            left = left % module;

            rmul = (right * mul) % module;
            lmul = (left * mul) % module;

            if (mul < 0)
            {
                if (negisbigger)
                {
                    mul = rmul;
                    r_index -= 2;
                }
                else
                {
                    mul = lmul;
                    l_index += 2;
                }
            }
            else
            {
                if (!negisbigger)
                {
                    mul = rmul;
                    r_index -= 2;
                }
                else
                {
                    mul = lmul;
                    l_index += 2;
                }
            }
            k -= 2;
        }
        Console.WriteLine((mul + module) % module);
    }
}