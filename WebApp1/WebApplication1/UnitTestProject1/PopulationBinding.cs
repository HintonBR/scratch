namespace UnitTestProject1
{
	using Catalyst.Data.Models.Bindings;

	public class PopulationBinding
	{
		private PopulationEntity cachedPopulationEntity;

		public PopulationBinding(SqlBinding composedSqlBinding)
		{
			this.ComposedSqlBinding = composedSqlBinding;
			this.CompilationNeeded = true;
		}

		public bool CompilationNeeded { get; set; }

		public string Name
		{
			get
			{
				return this.ComposedSqlBinding.BindingName;
			}

			set
			{
				this.ComposedSqlBinding.BindingName = value;
			}
		}

		public SqlBinding ComposedSqlBinding { get; set; }

		public PopulationEntity DestinationPopulationEntity
		{
			get
			{
				if (this.cachedPopulationEntity == null)
				{
					this.cachedPopulationEntity = (PopulationEntity)new PopulationEntity(this.ComposedSqlBinding.DestinationEntity);
				}
				return this.cachedPopulationEntity;
			}
			set
			{
				this.ComposedSqlBinding.DestinationEntity = value.ComposedEntity;
			}
		}

		public string SelectedMasterPopulationTable
		{
			get
			{
				return this.ComposedSqlBinding.GetBindingAttributeValue("SelectedMasterPopulationTable");
			}
			set
			{
				this.ComposedSqlBinding.SetBindingAttributeValue("SelectedMasterPopulationTable", value);
			}
		}
	}
}