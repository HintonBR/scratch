<Query Kind="Program" />

void Main()
{
	var answer = 0;
	var longestLength = 0;
	var range = Enumerable.Range(1, 1000000);
	foreach (var element in range)
	{
		var result = GenerateCollatzSequence(element);
		if (result.Count() >= longestLength) {
			longestLength = result.Count();
			answer = element;
			
//			
		}
		
	}
	longestLength.Dump();
			answer.Dump();
	
}

IEnumerable<long> GenerateCollatzSequence(long startingNumber) {
	yield return startingNumber;
	for(; startingNumber >1;) {
		if (startingNumber % 2==0) {
			startingNumber = startingNumber /2;
			yield return startingNumber;
		}
		else {
			startingNumber = 3*startingNumber + 1;
			yield return startingNumber;
		}
	
	}
}
// Define other methods and classes here