using System;
class Day2
{
    public void Run(String[] args)
    {
        /* Enter your code here. Read input from STDIN. Print output to STDOUT. Your class should be named Solution */
        var mealCost = Double.Parse(Console.ReadLine());
        var tipPercent = Int32.Parse(Console.ReadLine());
        var taxPercent = Int32.Parse(Console.ReadLine());
        var totalMealCost = Math.Round(mealCost + (mealCost * ((double)tipPercent / 100)) + (mealCost * ((double)taxPercent / 100)));
        Console.WriteLine("The total meal cost is {0} dollars.", totalMealCost);
    }
}