namespace UnitTestProject1
{
	using Catalyst.Data.Models;

	public class PopulationEntity
	{
		public PopulationEntity(Table table)
		{
			this.ComposedEntity = table;
			table.DatabaseNM = "SAM";
			table.SchemaNM = "Population";
		}

		public string Name
		{
			get
			{
				return this.ComposedEntity.ViewName;
			}

			set
			{
				this.ComposedEntity.ViewName = value;
				this.ComposedEntity.TableNM = value + "BASE";
			}
		}

		public Table ComposedEntity { get; set; }


		public string PopulationGrain
		{
			get
			{
				return this.ComposedEntity.GetEntityAttributeValue("PopulationGrain");
			}

			set
			{
				this.ComposedEntity.SetEntityAttributeValue("PopulationGrain", value);
			}
		}
	}
}