<Query Kind="Program">
  <Namespace>System.Security</Namespace>
</Query>

static void Main()
{
	SqlConnectionStringBuilder builder = new SqlConnectionStringBuilder();
	builder["Data Source"] = "platformazuresql.database.windows.net"; // replace with your server name
	builder["Initial Catalog"] = "platformazuresqldw"; // replace with your database name
	builder["Authentication"] = SqlAuthenticationMethod.ActiveDirectoryPassword;
	builder["Connect Timeout"] = 30;
	string username = "<username here>"; // replace with your username

	using (SqlConnection connection = new SqlConnection(builder.ConnectionString))
	{
		try
		{
			var password = "<password here>";
			var secure = new SecureString();
			password.ToCharArray().ToList().ForEach(c => secure.AppendChar(c));
			secure.MakeReadOnly();
			connection.Credential = new SqlCredential(username,secure);
			connection.Open();
			using (SqlCommand cmd = new SqlCommand(@"SELECT SUSER_SNAME()", connection))
			{
				Console.WriteLine("You have successfully logged on as: " + (string)cmd.ExecuteScalar());
			}
		}
		catch (Exception ex)
		{
			Console.WriteLine(ex.Message);
		}
	}
	Console.WriteLine("Please press any key to stop");
	Console.Read();
}

// Define other methods and classes here
