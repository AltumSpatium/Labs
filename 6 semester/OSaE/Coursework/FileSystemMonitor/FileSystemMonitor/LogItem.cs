using System;

namespace FileSystemMonitor
{
	public class LogItem
	{
		public int Id { get; set; }
		public string Name { get; set; }
		public string ChangeType { get; set; }
		public DateTime ChangeTime { get; set; }
		public long Size { get; set; }
		public string CheckSum { get; set; }

		public LogItem()
		{
			ChangeTime = DateTime.Now;
		}

		public LogItem(string name, string changeType, DateTime changeTime, long size, string checkSum)
		{
			Name = name;
			ChangeType = changeType;
			ChangeTime = changeTime;
			Size = size;
			CheckSum = checkSum;
		}
	}
}
