using System;

class Program
{
    static string[] cards_ico = {
    "6♠", "7♠", "8♠", "9♠", "10♠", "В♠", "Д♠", "К♠", "Т♠",
    "6♣", "7♣", "8♣", "9♣", "10♣", "В♣", "Д♣", "К♣", "Т♣",
    "6♥", "7♥", "8♥", "9♥", "10♥", "В♥", "Д♥", "К♥", "Т♥",
    "6♦", "7♦", "8♦", "9♦", "10♦", "В♦", "Д♦", "К♦", "Т♥" };

    static int[] cards_pnt = {
    6, 7, 8, 9, 10, 2, 3, 4, 11,
    6, 7, 8, 9, 10, 2, 3, 4, 11,
    6, 7, 8, 9, 10, 2, 3, 4, 11,
    6, 7, 8, 9, 10, 2, 3, 4, 11 };

    static int[] cards_use = new int[36];

    static int cards_imy = 0;

    static int GetNextCard()
    {
        Random rand = new System.Random();

        while (true)
        {
            int result = rand.Next(36);

            for (int offset = 0; offset < cards_use.Length; offset++)
            {
                if (cards_use[offset] < 0)
                {
                    cards_use[offset] = result;
                    return result;
                }
                else if (cards_use[offset] == result)
                {
                    result = -1;
                    break;
                }
            }
        }
    }

    static int ShowCards(bool _my)
    {
        int result = 0;

        if (_my)
        {
            for (int offset = 0; offset < cards_imy; offset++)
            {
                Console.Write(cards_ico[cards_use[offset]] + " ");
                result += cards_pnt[cards_use[offset]];
            }
        }
        else
        {
            for (int offset = cards_imy; offset < cards_use.Length && cards_use[offset] >= 0; offset++)
            {
                Console.Write(cards_ico[cards_use[offset]] + " ");
                result += cards_pnt[cards_use[offset]];
            }
        }

        return result;
    }

    static void Main(string[] args)
    {
        for (int index = 0; index < cards_use.Length; index++)
            cards_use[index] = -1;

        GetNextCard();
        GetNextCard();
        cards_imy = 2;

        Console.Clear();

        int mypnt = 0, abpnt = 0;

        while (true)
        {
            Console.Clear();
            Console.Write("Ваши карты: ");
            mypnt = ShowCards(true);
            Console.Write("\nВаши очки : " + mypnt);

            if (mypnt >= 21)
                break;

            Console.Write("\n\nДобавить еще карту? (y/n) : ");
            ConsoleKeyInfo kinfo = Console.ReadKey();

            if (kinfo.KeyChar == 'y')
            {
                GetNextCard();
                cards_imy++;
            }
            else if (kinfo.KeyChar == 'n')
                break;
        }

        Console.Clear();
        Console.Write("Ваши карты: ");
        mypnt = ShowCards(true);
        Console.Write("\nВаши очки : " + mypnt);

        Console.WriteLine("\n");

        if (mypnt > 21)
            Console.WriteLine("\nВы проиграли ...");
        else if (mypnt == 21 || (mypnt == 22 && cards_imy == 2))
            Console.WriteLine("\nВы победили ...");
        else
        {
            int abn_ind = 0;

            while (abpnt < mypnt)
            {
                abpnt += cards_pnt[GetNextCard()];
                abn_ind++;
            }

            Console.Write("\nМои карты : ");
            abpnt = ShowCards(false);
            Console.Write("\nМои очки  : " + abpnt);

            Console.WriteLine("\n");
            if (abpnt > 21) abpnt = 0;

            if(abpnt == 22 && abn_ind == 2)
                Console.WriteLine("\nВы проиграли ...");
            else if (mypnt > abpnt)
                Console.WriteLine("\nВы победили ...");
            else if (mypnt < abpnt)
                Console.WriteLine("\nВы проиграли ...");
            else
                Console.WriteLine("\nНичья ...");
        }

        Console.ReadKey();
    }
}
