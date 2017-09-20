<Query Kind="Program">
  <Namespace>System.Threading.Tasks</Namespace>
</Query>

void Main()
{
	var sum = 0L;
    var range = Enumerable.Range(2,2000000-2);
//    foreach(var num in range) {
//	 if (IsPrime(num)) {
//	 	sum += num;
//		//num.Dump();
//	 }
//	 
//	}
//	sum.Dump();
//		sum = range.Sum(n => IsPrime(n)?(long)n:0L);    
//	sum.Dump();
	sum = 0;
	Parallel.ForEach(range,()=>0,(int num,ParallelLoopState loopState, long localSum) => {localSum = (long) (localSum + (IsPrime(num)?(long)num:0L)); return localSum;},(localSum) => Interlocked.Add(ref sum, localSum));
	sum.Dump();
}
private bool IsPrime(long input) 
{
	if (input == 2 || input ==5) return true;
	if (input % 2 == 0) return false;
	if (input % 5 == 0) return false;
	var upperbound = (long) Math.Sqrt(input);
	for(long i = 3; i <= upperbound; i+=2) {
		if (input % i == 0) return false;
	}
	return true;
}