using System.Data.Entity;

namespace FileSystemMonitor
{
	public class LogContext : DbContext
	{
		public LogContext() : base("DefaultConnection")
		{

		}

		public DbSet<LogItem> Logs { get; set; }
	}
}
