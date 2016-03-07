namespace UnitTestProject1
{
	using System.ComponentModel.DataAnnotations;

	using Catalyst.Data;
	using Catalyst.Data.Enums;
	using Catalyst.Data.Validations;

	using Microsoft.VisualStudio.TestTools.UnitTesting;

	[TestClass]
	public class IntegrationTests
	{
		[TestMethod]
		public void CreatePopulationWithOnlyMasterPatientList()
		{

			var context = new MetadataContext("Data Source=localhost;Initial Catalog=EDWAdmin;Integrated Security=True;");
			PopulationSqlCompiler populationSqlCompiler = new PopulationSqlCompiler();
			PopulationPersistService service = new PopulationPersistService(populationSqlCompiler, context);
			
			//arrange
			string selectedMasterPatientTable = "Essentials.Patient";
			string selectedPopulationGrain = "Patient";
			var pop = new Population(selectedPopulationGrain, selectedMasterPatientTable);

			//act
			service.SavePopulation(pop);

			//assert
		}
	}
}
