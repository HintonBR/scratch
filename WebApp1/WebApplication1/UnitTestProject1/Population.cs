namespace UnitTestProject1
{
	using System;
	using System.Collections.Generic;
	using System.Collections.ObjectModel;
	using System.Linq;

	using Catalyst.Data.Enums;
	using Catalyst.Data.Models;
	using Catalyst.Data.Models.Bindings;

	public class Population
	{
		private List<PopulationBinding> nodes;

		public Population(string selectedPopulationGrain, string selectedMasterPopulationTable)
		{
			this.nodes = new List<PopulationBinding>();
			if (selectedPopulationGrain == null)
			{
				throw new ArgumentNullException(nameof(selectedPopulationGrain));
			}
			if (selectedMasterPopulationTable == null)
			{
				throw new ArgumentNullException(nameof(selectedMasterPopulationTable));
			}

			var table = new Table { ViewName = "End Population", TableNM = "End PopulationBASE" };
			table.DestinationConnection = new Connection { SystemName = "EDW", DataSystemTypeCode = DataSystemTypeCodes.SqlServer };

			var finalPopulationEntity = new PopulationEntity(table)
				                            {
					                            PopulationGrain =
						                            selectedPopulationGrain
				                            };

			var firstBinding = new PopulationBinding(new SqlBinding { BindingName = "First Binding" });
			firstBinding.SelectedMasterPopulationTable = selectedMasterPopulationTable;
			firstBinding.DestinationPopulationEntity = finalPopulationEntity;
			this.nodes.Add(firstBinding);
		}


		public bool CompilationNeeded
		{
			get
			{
				return this.Nodes.Any(item => item.CompilationNeeded);
			}
		}

		public string SelectedPopulationGrain
		{
			get
			{
				return this.Nodes[this.Nodes.Count].DestinationPopulationEntity.PopulationGrain;
			}
		}
		
		public ReadOnlyCollection<PopulationBinding> Nodes
		{
			get
			{
				return this.nodes.AsReadOnly();
			} 
		}
	}
}