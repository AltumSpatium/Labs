using System;

class Lab2
{
    static void Task1()
    {
        DateTime date = DateTime.Now;
        string datestr = date.ToString("G");
        int[] str = new int[10];
        string digstr = "";

        for (int i = 0; i < datestr.Length; i++)
        {
            char ch = datestr[i];
            if (ch >= '0' && ch <= '9')
            {
                int pos = ch - '0';
                str[pos]++;
            }
        }
        for (int i = 0; i < str.Length; i++)
        {
            if (str[i] != 0)
                digstr = digstr + "'" + i + "' - " + str[i] + " ";

        }
        Console.WriteLine("Условие задания:");
        Console.WriteLine("Получить текущее время и дату в двух разных форматах и вывести на экран");
        Console.WriteLine("количество нулей, единиц, ..., девяток в их записи");
        Console.WriteLine("");
        Console.WriteLine("Текущая дата и время:");
        Console.WriteLine("");
        Console.WriteLine(date.ToString("G"));
        Console.WriteLine(date.ToString("F"));
        Console.WriteLine("");
        Console.WriteLine("В строке '" + datestr + "' символов:");
        Console.WriteLine(digstr);
        Console.WriteLine("");
        Console.WriteLine("");
        Console.WriteLine("Нажмите любую клавишу для выхода...");
    }

    static void Task2()
    {
        string alph = "abcdefghijklmnopqrstuvwxyz";
        Random rnd = new Random();

        while (true)
        {
            string str = string.Empty;
            ConsoleKeyInfo ch;

            for (int i = 0; i < 4; i++)
            {
                str += alph[rnd.Next(0, alph.Length - 1)];
            }
            do
            {
                Console.Clear();
                Console.WriteLine("Условие задания:");
                Console.WriteLine("Сгенерировать равновероятно случайную строку длиной не более четырех");
                Console.WriteLine("строчных английских букв");
                Console.WriteLine("");
                Console.WriteLine("Случайная строка: " + str);
                Console.WriteLine("");
                Console.WriteLine("Сгенерировать строку еще раз? (y/n)");
                ch = Console.ReadKey();
            } while (ch.KeyChar != 'y' && ch.KeyChar != 'n');
            if (ch.KeyChar == 'n')
                break;
        }
        Console.Clear();
        Console.WriteLine("Нажмите любую клавишу для выхода...");
    }

    static void Task3()
    {
        //Дана строка из 256 английских букв. Записать через пробел 30 символов этой строки, стоящих на случайных местах. Желательно сделать только одно обращение к классу Random.
        string str = "ahfgrteydufhgnvmcbzcxvdfsgahdkflgurteutiyobnfjgldhqoeavdfsgahdkfhcmftuifhdkbnomfgeobnfjgldqhqgnvmcbzcxvogahdkfhcmfldhqeaqvdfsldhqeavdofsahdkfhcmftuifhdqkbnmfahodkfhcmftuifhdkbnqmfahfgrteyduflgurlteutiflqdhqeavdfsldhqeavdffldhqeqavdfsldhqeavdfhfynmlpurcqzx";
        Random rnd = new Random();


        while (true)
        {
            string str2 = string.Empty;
            ConsoleKeyInfo ch;
            for (int i = 0; i < 30; i++)
            {
                str2 += str[rnd.Next(0, str.Length - 1)];
            }
            do
            {
                Console.Clear();
                Console.WriteLine("Условие задания:");
                Console.WriteLine("Дана строка из 256 английских букв. Записать через пробел 30 символов этой ");
                Console.WriteLine("строки, стоящих на случайных местах. Желательно сделать только одно");
                Console.WriteLine("обращение к классу Random");
                Console.WriteLine("");
                Console.WriteLine("Строка:");
                Console.WriteLine(str);
                Console.WriteLine("");
                Console.WriteLine("30 символов из строки:");
                for (int i = 0; i < str2.Length; i++)
                {
                    Console.Write(str2[i] + " ");
                }
                Console.WriteLine("");
                Console.WriteLine("Сгенерировать строку еще раз? (y/n)");
                ch = Console.ReadKey();
            } while (ch.KeyChar != 'y' && ch.KeyChar != 'n');
            if (ch.KeyChar == 'n')
                break;
        }
        Console.Clear();
        Console.WriteLine("Нажмите любую клавишу для выхода...");
    }

    static void Main()
    {
        char ch;

        while (true)
        {
            Console.Clear();
            Console.WriteLine("Выберите пункт меню:");
            Console.WriteLine("");
            Console.WriteLine("1 - Задание 1");
            Console.WriteLine("2 - Задание 2");
            Console.WriteLine("3 - Задание 3");
            Console.WriteLine("0 - Выход");
            ch = Console.ReadKey().KeyChar;
            if (ch == '1')
            {
                Console.Clear();
                Task1();
            }
            if (ch == '2')
            {
                Console.Clear();
                Task2();
            }
            if (ch == '3')
            {
                Console.Clear();
                Task3();
            }
            if (ch == '0')
            {
                break;
            }
            Console.ReadKey();
        }
    }
}