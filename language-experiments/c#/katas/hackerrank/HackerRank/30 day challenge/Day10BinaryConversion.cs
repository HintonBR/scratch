using System; 
using System.Collections.Generic; 
using System.IO; 
using System.Linq; 

class Day10BinaryConversion
{
    public void Run(String[] args)
    {
        do
        {
            int number = Int32.Parse(Console.ReadLine()); 

            var result = factorial(number); 
            Console.WriteLine($"{result} is the factorial of {number}"); 
        }
        while (1 == 1); 
    }

    public int factorial(int n)
    {
        if (n == 1)return n; 
        return n * factorial(n-1); 
    }
}

