using System;

public delegate void Handler(int val);

class RationalNumber : IEquatable<RationalNumber>
{
    int _n;
    int _m;
    static string str;

    public RationalNumber(int n, int m)
    {
        _n = n;
        _m = m;
        if (_m == 0)
            throw new Exception("Denominator can't equals zero!");
    }
    
    public int N
    {
        get { return _n; }
        set { _n = value; }
    }

    public int M
    {
        get { return _m; }
        set
        {   
            if (value == 0)
            {
                if (ZeroValueSet != null)
                {
                    ZeroValueSet(value);
                    throw new Exception("Denominator can't equals zero!");
                }
            }
            _m = value;
        }
    }

    public event Handler ZeroValueSet;

    public static RationalNumber operator + (RationalNumber a, RationalNumber b)
    {
        if (a._m == b._m) return new RationalNumber(a._n + b._n, a._m);
        else return new RationalNumber(a._n * b._m  + b._n * a._m, a._m * b._m);
    }

    public static RationalNumber operator - (RationalNumber a, RationalNumber b)
    {
        if (a._m == b._m) return new RationalNumber(a._n - b._n, a._m);
        else return new RationalNumber(a._n * b._m - b._n * a._m, a._m * b._m);
    }

    public static RationalNumber operator * (RationalNumber a, RationalNumber b)
    {
        return new RationalNumber(a._n * b._n, a._m * b._m);
    }

    public static RationalNumber operator / (RationalNumber a, RationalNumber b)
    {
        return new RationalNumber(a._n * b._m, a._m * b._n);
    }

    public override string ToString()
    {
        return string.Format("{0}/{1}", _n, _m);
    }

    public string StrFormat()
    {
        return string.Format("Numerator: {0}, Denominator: {1}", _n, _m);
    }

    public RationalNumber StrToObj(string str)
    {
        string strn = "";
        string strm = "";

        for (int i = 0; i < str.Length; i++)
        {
                if (str[i] == '/')
                {
                    for (i++ ; i < str.Length; i++)
                    {
                        strm += str[i];
                    }                    
                }
                else strn += str[i];
        }
        RationalNumber ratnum1 = new RationalNumber(Convert.ToInt32(strn), Convert.ToInt32(strm));
        return ratnum1;
    }

    public bool Equals(RationalNumber other)
    {
        if (other == null)
            return false;
        if ( ((double)this._n)/((double)this._m) == (((double)other._n)/((double)other._m)))
            return true;
        else
            return false;
    }

    public static implicit operator RationalNumber(double x)
    {
        string strint = "";
        string strfract = "";
        double counter = 0;
        int n;
        int m;

        str = x.ToString();
        for (int i = 0; i < str.Length; i++)
        {
            if (str[i] == '.' || str[i] == ',')
            {
                for (i++; i < str.Length; i++ )
                {
                    strfract += str[i];
                    counter++;
                }
            }
            else strint += str[i];
        }
        if (strfract.Length == 0)
             strfract = "0";
        m = (int) Math.Pow(10, counter);
        n = (Convert.ToInt32(strint) * m + Convert.ToInt32(strfract));
        return new RationalNumber(n, m);
    }

    public static implicit operator RationalNumber(int x)
    {
        return new RationalNumber(x, 1);
    }

    public static implicit operator double(RationalNumber ratnum)
    {
        return (((double)ratnum._n) / ((double)ratnum._m));
    }

    public static implicit operator int(RationalNumber ratnum)
    {
        return (ratnum._n / ratnum._m);
    }

    public static implicit operator string(RationalNumber ratnum)
    {
        return ratnum.ToString();
    }
}

class Program
{
    static void Main()
    {
        try
        {
            RationalNumber ratnum1 = new RationalNumber(0, 1);
            RationalNumber ratnum2 = new RationalNumber(0, 1);
            string str = "";
            string str2 = "";
            ConsoleKeyInfo ch;
            ConsoleKeyInfo ch1;

            Console.Clear();
            Console.WriteLine("Enter N");
            ratnum1.N = int.Parse(Console.ReadLine());
            Console.Clear();
            Console.WriteLine("Enter M");

            ratnum1.ZeroValueSet += i => { Console.Clear(); Console.WriteLine("Denominator can't equals zero!"); Console.ReadKey(); };

            ratnum1.M = int.Parse(Console.ReadLine());
            Console.Clear();
            Console.WriteLine("Choose string format:");
            Console.WriteLine("1");
            Console.WriteLine("2");
            ch = Console.ReadKey();
            if (ch.KeyChar == '1')
                str = ratnum1.ToString();
            if (ch.KeyChar == '2')
                str = ratnum1.StrFormat();
            while (true)
            {
                Console.Clear();
                Console.WriteLine("Your rational number: ");
                Console.WriteLine(str);
                Console.WriteLine("");
                Console.WriteLine("1 - Add the rational number");
                Console.WriteLine("2 - Subtract the rational number");
                Console.WriteLine("3 - Multiplied by the rational number");
                Console.WriteLine("4 - Divided by the rational number");
                Console.WriteLine("5 - To equal with rational number");
                Console.WriteLine("6 - Change string format");
                Console.WriteLine("0 - Exit");
                ch1 = Console.ReadKey();
                if (ch1.KeyChar == '1')
                {
                    Console.Clear();
                    Console.WriteLine("Enter N");
                    ratnum2.N = int.Parse(Console.ReadLine());
                    Console.Clear();
                    Console.WriteLine("Enter M");

                    ratnum2.ZeroValueSet += i => { Console.Clear(); Console.WriteLine("Denominator can't equals zero!"); Console.ReadKey(); };

                    ratnum2.M = int.Parse(Console.ReadLine());
                    ratnum1 = ratnum1 + ratnum2;
                    if (ch.KeyChar == '1')
                        str = ratnum1.ToString();
                    if (ch.KeyChar == '2')
                        str = ratnum1.StrFormat();
                    Console.Clear();
                    Console.WriteLine("Press any key ...");
                }

                if (ch1.KeyChar == '2')
                {
                    Console.Clear();
                    Console.WriteLine("Enter N");
                    ratnum2.N = int.Parse(Console.ReadLine());
                    Console.Clear();
                    Console.WriteLine("Enter M");

                    ratnum2.ZeroValueSet += i => { Console.Clear(); Console.WriteLine("Denominator can't equals zero!"); Console.ReadKey(); };

                    ratnum2.M = int.Parse(Console.ReadLine());
                    ratnum1 = ratnum1 - ratnum2;
                    if (ch.KeyChar == '1')
                        str = ratnum1.ToString();
                    if (ch.KeyChar == '2')
                        str = ratnum1.StrFormat();
                    Console.Clear();
                    Console.WriteLine("Press any key ...");
                }

                if (ch1.KeyChar == '3')
                {
                    Console.Clear();
                    Console.WriteLine("Enter N");
                    ratnum2.N = int.Parse(Console.ReadLine());
                    Console.Clear();
                    Console.WriteLine("Enter M");

                    ratnum2.ZeroValueSet += i => { Console.Clear(); Console.WriteLine("Denominator can't equals zero!"); Console.ReadKey(); };

                    ratnum2.M = int.Parse(Console.ReadLine());
                    ratnum1 = ratnum1 * ratnum2;
                    if (ch.KeyChar == '1')
                        str = ratnum1.ToString();
                    if (ch.KeyChar == '2')
                        str = ratnum1.StrFormat();
                    Console.Clear();
                    Console.WriteLine("Press any key ...");
                }

                if (ch1.KeyChar == '4')
                {
                    Console.Clear();
                    Console.WriteLine("Enter N");
                    ratnum2.N = int.Parse(Console.ReadLine());
                    Console.Clear();
                    Console.WriteLine("Enter M");

                    ratnum2.ZeroValueSet += i => { Console.Clear(); Console.WriteLine("Denominator can't equals zero!"); Console.ReadKey(); };

                    ratnum2.M = int.Parse(Console.ReadLine());
                    ratnum1 = ratnum1 / ratnum2;
                    if (ch.KeyChar == '1')
                        str = ratnum1.ToString();
                    if (ch.KeyChar == '2')
                        str = ratnum1.StrFormat();
                    Console.Clear();
                    Console.WriteLine("Press any key ...");
                }

                if (ch1.KeyChar == '5')
                {
                    Console.Clear();
                    Console.WriteLine("Enter rational number: ");
                    str2 = Console.ReadLine();
                    ratnum2 = ratnum2.StrToObj(str2);
                    Console.Clear();
                    if (ratnum1.Equals(ratnum2))
                        Console.WriteLine("Yes");
                    else
                        Console.WriteLine("No");
                }

                if (ch1.KeyChar == '6')
                {
                    Console.Clear();
                    Console.WriteLine("Choose string format:");
                    Console.WriteLine("1");
                    Console.WriteLine("2");
                    ch = Console.ReadKey();
                    if (ch.KeyChar == '1')
                        str = ratnum1.ToString();
                    if (ch.KeyChar == '2')
                        str = ratnum1.StrFormat();
                    Console.Clear();
                    Console.WriteLine("Press any key ...");
                }

                if (ch1.KeyChar == '0')
                    break;
                Console.ReadKey();
            }
        }
        catch (Exception ex)
        {
            Console.Clear();
            Console.WriteLine(ex.Message);
        }
    }
}

