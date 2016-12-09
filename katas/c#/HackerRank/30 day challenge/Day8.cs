using System; 
using System.Collections.Generic;
using System.IO;
using System.Linq;

class Day8
{
    public void Run(String[] args)
    {
        Dictionary<string, int> Phonebook = new Dictionary<string, int>();
        int n = Convert.ToInt32(Console.ReadLine());
        foreach(int index in Enumerable.Range(1, n)) 
        {
            string[] arr_temp = Console.ReadLine().Split(' ');
            if (arr_temp.Count() < 2 || arr_temp.Count() > 2) 
            {
                throw new Exception("Invalid phonebook entry");
            }

            Phonebook.Add(arr_temp[0], Int32.Parse(arr_temp[1]));    
        }
        
        var query = Console.ReadLine();
        var queryLists = new List<string>();
        while(!string.IsNullOrWhiteSpace(query)) 
        {
            queryLists.Add(query);
            query = Console.ReadLine();
        }

        foreach(var item in queryLists) 
        {
            if(Phonebook.ContainsKey(item)) 
            {
                Console.WriteLine($"{item}={Phonebook[item]}");
            }
            else 
            {
                Console.WriteLine("Not found");
            }
        }
    }
}

