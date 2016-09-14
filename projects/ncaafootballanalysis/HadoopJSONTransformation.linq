<Query Kind="Program">
  <NuGetReference>Newtonsoft.Json</NuGetReference>
  <Namespace>Newtonsoft.Json</Namespace>
  <Namespace>Newtonsoft.Json.Bson</Namespace>
  <Namespace>Newtonsoft.Json.Converters</Namespace>
  <Namespace>Newtonsoft.Json.Linq</Namespace>
  <Namespace>Newtonsoft.Json.Schema</Namespace>
  <Namespace>Newtonsoft.Json.Serialization</Namespace>
  <Namespace>System.Net</Namespace>
  <Namespace>System.Threading.Tasks</Namespace>
</Query>

void Main()
{
	string allgames = "http://data.ncaa.com/sites/default/files/data/scoreboard/football/fbs/2016/01/scoreboard.json";
	string gameinfo;
	using (var client = new WebClient())
{
    gameinfo = client.DownloadString(allgames);
}
	var obj = JObject.Parse(gameinfo);
	var gameurls = obj["scoreboard"].Children()["games"].Children().Select(i => @"http://data.ncaa.com"+i);
	Parallel.ForEach(gameurls, gameurl =>
	DownloadAndStore(gameurl));
	
}

// Define other methods and classes here
private void DownloadAndStore(string url) {

using (var client = new WebClient())
{
    var gameinfo = client.DownloadString(url);
	var obj = JObject.Parse(gameinfo);
	var teamstatsurl = obj["tabsArray"].Children().Children().Where(i => i["type"].ToString() == "team-stats").Select(i =>  @"http://data.ncaa.com"+ i["file"]).FirstOrDefault();
	var teamstats = client.DownloadString(teamstatsurl);
	WriteToFile(teamstats,gameinfo);
}
}

private void WriteToFile(string teamstatsjson, string gameinfojson) {
	var rootString = @"C:\Source\Environments\spark16\bin\sourcefiles\processedfiles\hasdemofiles\";
	
	var obj = JObject.Parse(gameinfojson);
	string gameroot = obj["home"]["nameSeo"].ToString() + "-" + obj["away"]["nameSeo"].ToString();
	string gameinfoname = gameroot + "gameinfo.json";
	string teamstatsname = gameroot + "teamstats.json";
		using(var writer = new JsonTextWriter(new StreamWriter(rootString + @"gameinfo\" +  gameinfoname)))
	{
	obj.WriteTo(writer);
	writer.Close();
	}
	
	obj = JObject.Parse(teamstatsjson);
		using(var writer = new JsonTextWriter(new StreamWriter(rootString + @"teamstats\" + teamstatsname)))
	{
	obj.WriteTo(writer);
	writer.Close();
	}
}