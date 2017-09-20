<Query Kind="Program" />

void Main()
{
long sum = 0;
	foreach(var number in Fibonacci(4000000)) {
		sum+=number%2==0?number:0;
	}
	sum.Dump();
}
IEnumerable<long> Fibonacci(long maxvalue) {
    long startvalue = 1;
	long lastvalue = 1;
   if (startvalue == 1) yield return 1;
   if (lastvalue+startvalue > maxvalue) yield break;
   for(startvalue = 1; startvalue <= maxvalue;) {
   	yield return lastvalue + startvalue;
	var newstartvalue = lastvalue +startvalue;
	lastvalue = startvalue;
	startvalue = newstartvalue;
   }
}

// Define other methods and classes here
