using System;
using System.Linq;

class Program
{
    static void Main(string[] args)
    {
        string s = Console.ReadLine();
        int sign = 0;
        bool same = false;
        for (int i = 0; i < s.Length / 2; i++)
        {
            if (s[i] != s[s.Length - i - 1])
            {
                sign++;
            }
            else if ((s[i] == s[s.Length - i - 1]) && (s[i] != s[i + 1]))
            {
                sign--;
            }

            else if ((s[i] == s[s.Length - i - 1]) && (s[i] == s[i + 1]))
            {
                same = true;
                break;
            }
        }

        if (sign > 0)
        {
            Console.WriteLine(s.Length);
        }
        else if (sign < 0)
        {
            Console.WriteLine(s.Length - 1);
        }
        else if (same || s.Length == 1)
        {
            Console.WriteLine(-1);
        }
    }
}