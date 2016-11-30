using System; 
using System.Collections.Generic;
using System.IO;
using System.Linq;

class Day6
{
    public void Run(String[] args)
    {
        int linesToRead = Convert.ToInt32(Console.ReadLine());
        List<string> strings = new List<string>();


         //2 <= n <= 20
         if (linesToRead < 1 || linesToRead > 10) return;

         //Print n x i = result
         foreach(var i in Enumerable.Range(1,linesToRead)) 
         {
            var word = Console.ReadLine();
            strings.Add(word);
         }

         foreach(var word in strings) 
         {
            Console.WriteLine("{0} {1}",
                string.Join("", word.Where((letter, index) => index%2 == 0)),
                string.Join("", word.Where((letter, index) => index%2 == 1))
            );
         }
    }
}

