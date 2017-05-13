using System;

namespace FileSystemMonitor
{
	public class LogItem
	{
		public int Id { get; set; }
		public string Name { get; set; }
		public string ChangeType { get; set; }
		public DateTime ChangeTime { get; set; }
		public int Size { get; set; }
		public int CheckSum { get; set; }

		public LogItem()
		{
			ChangeTime = DateTime.Now;
		}

		public LogItem(int id, string name, string changeType, DateTime changeTime, int size, int checkSum)
		{
			Id = id;
			Name = name;
			ChangeType = changeType;
			ChangeTime = changeTime;
			Size = size;
			CheckSum = checkSum;
		}
	}
}
