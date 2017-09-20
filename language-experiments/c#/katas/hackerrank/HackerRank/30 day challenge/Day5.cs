using System; 
using System.Collections.Generic;
using System.IO;
using System.Linq;

class Day5
{
    public void Run(String[] args)
    {
        int n = Convert.ToInt32(Console.ReadLine());

         //2 <= n <= 20
         if (n <= 2 || n >= 20) return;

         //Print n x i = result
         foreach(var i in Enumerable.Range(1,10)) 
         {
            Console.WriteLine($"{n} x {i} = {n*i}");
         }
    }
}

