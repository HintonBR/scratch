namespace UnitTestProject1
{
	using System.Linq;

	using Catalyst.Data.Models;
	using Catalyst.Data.Models.ObjectAttributeValues;

	public static class AttributeValueExtensions
	{
		public static string GetEntityAttributeValue(this Table entity, string attributeName)
		{
			var attribute =
				entity.AttributeValues.FirstOrDefault(
					item => item.AttributeName == attributeName);
			return attribute?.LongTextValue;
		}

		public static void SetEntityAttributeValue(this Table entity, string attributeName, string value)
		{
			EntityAttributeValue attribute = entity.AttributeValues.FirstOrDefaultOfAttributeType(attributeName);

			if (attribute == null)
			{
				attribute = new EntityAttributeValue { AttributeName = attributeName };
				entity.AttributeValues.Add(attribute);
			}

			attribute.LongTextValue = value;
		}

		public static string GetBindingAttributeValue(this Binding binding, string attributeName)
		{
			var attribute =
				binding.AttributeValues.FirstOrDefault(
					item => item.AttributeName == attributeName);
			return attribute == null ? null : attribute.LongTextValue;
		}

		public static void SetBindingAttributeValue(this Binding binding, string attributeName, string value)
		{
			BindingAttributeValue attribute = binding.AttributeValues.FirstOrDefaultOfAttributeType(attributeName);

			if (attribute == null)
			{
				attribute = new BindingAttributeValue { AttributeName = attributeName };
				binding.AttributeValues.Add(attribute);
			}

			attribute.LongTextValue = value;
		}
	}
}