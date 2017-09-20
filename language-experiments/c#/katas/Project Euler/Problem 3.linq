<Query Kind="Program" />

void Main()
{
    var number =  600851475143;
	var divisor = 2;
	var upperbound = number /2 ;
	var answer = 0l;
	for(long element = upperbound; element >=2;)
	{
		if (number % element == 0)
		{
		if (IsPrime(element)) 
		{
			answer = element;
			break;
		}
		}
		divisor++;
		element = number /divisor;
	}	
	answer.Dump();
}
private bool IsPrime(long input) 
{
	if (input % 2 == 0) return false;
	if (input % 5 == 0) return false;
	var upperbound = (long) Math.Sqrt(input);
	for(long i = 3; i <= upperbound; i+=2) {
		if (input % i == 0) return false;
	}
	return true;
}