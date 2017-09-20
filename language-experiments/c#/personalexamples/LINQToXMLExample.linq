<Query Kind="Statements" />

var doc = XDocument.Load(@"C:\temp\icd9cmToBpMap.xml");
doc.Descendants().Where(item=>(int?)item.Attribute("relevance") >= 10).Remove();
doc.Descendants().Where(node=>node.Name == "code").Where(item=>item.Elements("monograph").Count() == 0).Remove();
var projection = doc.Descendants().Where(node=>node.Name == "code").Select(code=>new {ICD9Name=code.Attribute("term"),ICD9Code=code.Attribute("id"),BMJConditions = code.Descendants().Where(element=>element.Name == "monograph").Select(condition=>new {BMJConditionId = condition.Attribute("id"), BMJConditionName = condition.Attribute("title"), Classification = condition.Attribute("type"), Relevance = condition.Attribute("relevance")})});
var conditions = projection.SelectMany(code=>code.BMJConditions,(code,condition)=>new {code.ICD9Name.Value,code.ICD9Code.Value,condition.BMJConditionId.Value,condition.BMJConditionName.Value,condition.Classification.Value, condition.Relevance.Value});
conditions.Dump();
var fs = new StreamWriter(@"c:\temp\bmjconditions.csv");
fs.WriteLine("{0}, {1}, {2}, {3}, {4}","ICD9 Name", "ICD9 Code", "BMJ Condition ID", "BMJ Condition Name", "BMJ Classification", "BMJ Relevance");
foreach (var condition in conditions)
{
fs.WriteLine("{0}, {1}, {2}, {3}, {4}",condition.ICD9Name, condition.ICD9Code, condition.BMJConditionId, condition.BMJConditionName, condition.Classification, condition.Relevance);	
}
fs.Close();
//var codes = doc.Descendants().Where(node=>node.Name == "code").Select(item=>new {Code=item.Attribute("id").Value, Name=item.Attribute("term").Value}).Distinct().OrderBy(item=>item.Code);
//var uniquecodes = doc.Descendants().Where(node=>node.Name == "code").Select(item=>new {Code=item.Attribute("id").Value}).Distinct().OrderBy(item=>item.Code);
//uniquecodes.Dump();
//var monograph = doc.Descendants().Where(element=>element.Name == "monograph").Select(item=>new {Id=item.Attribute("id").Value}).Distinct();
//monograph.Count().Dump();
//codes.Dump();
//doc.Dump();
//var monographTypes = doc.Descendants().Where(element=>element.Name == "monograph").GroupBy(monograph=>monograph.Attribute("type").Value).Select(item=>new {MonoGraphType = item.Key, MonoGraphCount = item.Select(monograph1=>monograph1.Attribute("id").Value).Distinct().Count(), MonoGraphTitles = item.Select(monograph=>monograph.Attribute("title").Value).Distinct().OrderBy(value=>value)});
//monographTypes.Dump();
//var result = doc.Elements("codeSystem").Where(item=>item.Attribute("id").Value == "ICD9CM").Elements("code").GroupBy(item=>item.Attribute("id").Value).Where(grouping=>grouping.Key=="V62.89");
//result.Dump();