using System;
class Day3
{
    public void Run(String[] args)
    {
        /* Enter your code here. Read input from STDIN. Print output to STDOUT. Your class should be named Solution */
        int n = int.Parse(Console.ReadLine());
        if (n % 2 != 0)
        {
            Console.WriteLine("Weird");
        }
        else if (2 <= n && n <= 5)
        {
            Console.WriteLine("Not Weird");
        }
        else if (6 <= n && n <= 20)
        {
            Console.WriteLine("Weird");
        }
        else if (n > 20)
        {
            Console.WriteLine("Not Weird");
        }
    }
}