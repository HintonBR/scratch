namespace UnitTestProject1
{
	public abstract class PopulationCompiler
	{
		public void Compile(PopulationBinding populationBinding)
		{
			this.CompileInternals(populationBinding);
			populationBinding.CompilationNeeded = false;
		}

		protected abstract void CompileInternals(PopulationBinding populationBinding);
	}
}