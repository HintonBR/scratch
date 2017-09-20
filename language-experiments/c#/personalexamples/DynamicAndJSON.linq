<Query Kind="Program">
  <Reference>&lt;ProgramFilesX64&gt;\Microsoft SDKs\Azure\.NET SDK\v2.9\bin\plugins\Diagnostics\Newtonsoft.Json.dll</Reference>
  <Namespace>Newtonsoft.Json.Linq</Namespace>
  <Namespace>Newtonsoft.Json</Namespace>
  <Namespace>System.Dynamic</Namespace>
</Query>

void Main()
{
	JObject o = new JObject(
	    new JProperty("Name", "John Smith"),
	     new JProperty("BirthDate", new DateTime(1983, 3, 20))
	     );
	 
	 o.ToString().Dump();
	 JsonSerializer serializer = new JsonSerializer();
	 Person p = (Person)serializer.Deserialize(new JTokenReader(o), typeof(Person));
	 
	 Console.WriteLine(p.Name);
	 JsonConvert.SerializeObject(p).Dump();
	 
	 dynamic d = new ExpandoObject();
	 d.Birthdate = DateTime.Now;
	 d.Name = "John Smith";
	 Console.WriteLine(JsonConvert.SerializeObject(d));
}

// Define other methods and classes here
public class Person 
{
	public string Name {get; set;}
	public DateTime BirthDate {get; set;}
}