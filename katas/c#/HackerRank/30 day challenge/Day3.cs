using System;
using System.Collections.Generic;
using System.IO;
class Day3 {
    static void Main(String[] args) {
        /* Enter your code here. Read input from STDIN. Print output to STDOUT. Your class should be named Solution */
        var mealCost = Double.Parse(Console.ReadLine());
        var tipPercent = Int32.Parse(Console.ReadLine());
        var taxPercent = Int32.Parse(Console.ReadLine());
        var totalMealCost = Math.Floor(mealCost + (mealCost*(((double)tipPercent/100)) + (mealCost*((double)taxPercent/100)));
        Console.WriteLine("The total meal cost is {0} dollars", totalMealCost);
        Console.ReadLine();
    }
}