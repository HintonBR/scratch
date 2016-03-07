namespace UnitTestProject1
{
	using System.Data.Entity;
	using System.Data.Entity.Migrations;
	using System.Linq;
	using System.Transactions;

	using Catalyst.Data;
	using Catalyst.Data.Models;

	internal class PopulationPersistService
	{
		private PopulationSqlCompiler populationSqlCompiler;

		private MetadataContext dataContext;

		public PopulationPersistService(PopulationSqlCompiler populationSqlCompiler, MetadataContext context)
		{
			this.dataContext = context;
			this.populationSqlCompiler = populationSqlCompiler;
		}

		public void SavePopulation(Population pop)
		{
			this.CompileBindingsThatHaveChanged(pop);
			using (var scope = new TransactionScope())
			{
				this.SaveEntities(pop);
				this.dataContext.SaveChanges();
				this.SaveBindings(pop);
				this.dataContext.SaveChanges();
			}
		}

		private void SaveBindings(Population pop)
		{
			foreach (var populationBindings in pop.Nodes.Select(item => item.ComposedSqlBinding))
			{
				this.dataContext.Bindings.AddOrUpdate(populationBindings);
			}
		}

		private void SaveEntities(Population pop)
		{
			foreach (Table populationEntity in pop.Nodes.Select(item => item.DestinationPopulationEntity.ComposedEntity))
			{
				this.dataContext.Tables.AddOrUpdate(populationEntity);
			}
		}

		private void CompileBindingsThatHaveChanged(Population pop)
		{
			foreach (var populationBinding in pop.Nodes.Where(item => item.CompilationNeeded))
			{
				this.populationSqlCompiler.Compile(populationBinding);
			}
		}
	}
}