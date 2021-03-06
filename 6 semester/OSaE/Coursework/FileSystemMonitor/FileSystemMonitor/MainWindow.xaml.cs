﻿using System;
using System.Windows;
using System.Data.Entity;
using System.Windows.Controls;
using System.Windows.Forms;
using System.IO;
using System.Security.Cryptography;

namespace FileSystemMonitor
{
	/// <summary>
	/// Interaction logic for MainWindow.xaml
	/// </summary>
	public partial class MainWindow : Window
	{
		LogContext db;
		FileSystemWatcher watcher = new FileSystemWatcher();

		bool watching = false;

		public MainWindow()
		{
			InitializeComponent();

			db = new LogContext();
			db.Logs.Load();
			dgLog.ItemsSource = db.Logs.Local.ToBindingList();

			watcher.Changed += new FileSystemEventHandler(OnChanged);
			watcher.Created += new FileSystemEventHandler(OnChanged);
			watcher.Deleted += new FileSystemEventHandler(OnChanged);
			watcher.Renamed += new RenamedEventHandler(OnRenamed);
		}	

		private void OnChanged(object source, FileSystemEventArgs e)
		{
			string name = e.FullPath;
			string changeType;

			switch (e.ChangeType.ToString())
			{
				case "Created":
					changeType = "Создан";
					break;
				case "Changed":
					changeType = "Изменён";
					break;
				case "Deleted":
					changeType = "Удалён";
					break;
				default:
					changeType = "";
					break;
			}

			long size = GetSize(name);
			string checksum = ComputeMD5Checksum(name);

			App.Current.Dispatcher.Invoke(delegate
			{
				LogItem log = new LogItem(name, changeType, DateTime.Now, size, checksum);
				db.Logs.Add(log);
				db.SaveChanges();
			});
		}

		private void OnRenamed(object source, RenamedEventArgs e)
		{
			string name = string.Format("{0} -> {1}", e.OldFullPath, e.FullPath);
			long size = GetSize(e.FullPath);
			string checksum = ComputeMD5Checksum(e.FullPath);

			App.Current.Dispatcher.Invoke(delegate
			{
				LogItem log = new LogItem(name, "Переименован", DateTime.Now, size, checksum);
				db.Logs.Add(log);
				db.SaveChanges();
			});
		}

		private long GetSize(string path)
		{
			if (File.Exists(path))
			{
				FileInfo file = new FileInfo(path);
				return file.Length / 1024;
			}
			else if (Directory.Exists(path))
			{
				DirectoryInfo directory = new DirectoryInfo(path);
				long size = 0;
				foreach (FileInfo file in directory.GetFiles())
				{
					size += file.Length;
				}

				return size / 1024;
			}

			return 0;
		}

		private string ComputeMD5Checksum(string path)
		{
			if (File.Exists(path))
			{
				using (FileStream fs = File.OpenRead(path))
				{
					MD5 md5 = new MD5CryptoServiceProvider();
					byte[] fileData = new byte[fs.Length];
					fs.Read(fileData, 0, (int)fs.Length);
					byte[] checksum = md5.ComputeHash(fileData);
					string result = BitConverter.ToString(checksum).Replace("-", "");
					return result;
				}
			}

			return "";
		}

		private void btnClearLogs_Click(object sender, RoutedEventArgs e)
		{
			db.Logs.RemoveRange(db.Logs);
			db.SaveChanges();
		}

		private void btnStartMonitoring_Click(object sender, RoutedEventArgs e)
		{
			watching = !watching;

			if (watching)
			{
				btnStartMonitoring.Content = "Остановить мониторинг";
				enableInterface(false);

				watcher.Path = tbDir.Text;
				watcher.Filter = tbFilter.Text;

				watcher.IncludeSubdirectories = (bool)cbNested.IsChecked;

				NotifyFilters filters = new NotifyFilters();

				if ((bool)cbAttrs.IsChecked)
					filters |= NotifyFilters.Attributes;

				if ((bool)cbDirname.IsChecked)
					filters |= NotifyFilters.DirectoryName;

				if ((bool)cbFilename.IsChecked)
					filters |= NotifyFilters.FileName;

				if ((bool)cbSecurity.IsChecked)
					filters |= NotifyFilters.Security;

				if ((bool)cbWrite.IsChecked)
					filters |= NotifyFilters.LastWrite;

				watcher.NotifyFilter = filters;

				watcher.EnableRaisingEvents = true;
			}
			else
			{
				btnStartMonitoring.Content = "Начать мониторинг";
				enableInterface(true);

				watcher.EnableRaisingEvents = false;
			}
		}

		private void enableInterface(bool enabled)
		{
			tbDir.IsEnabled = enabled;
			tbFilter.IsEnabled = enabled;
			btnOpenDir.IsEnabled = enabled;
			gbEvents.IsEnabled = enabled;
		}

		private bool checkDirectory()
		{
			string path = tbDir.Text.ToString();
			bool directoryExists = Directory.Exists(path);

			if (!directoryExists) labelError.Content = "Такой папки не существует!";
			else labelError.Content = "";

			return directoryExists;
		}

		private void tbDir_TextChanged(object sender, TextChangedEventArgs e)
		{
			btnStartMonitoring.IsEnabled = checkDirectory();
		}

		private void btnOpenDir_Click(object sender, RoutedEventArgs e)
		{
			FolderBrowserDialog fbd = new FolderBrowserDialog();

			DialogResult result = fbd.ShowDialog();

			if (result == System.Windows.Forms.DialogResult.OK)
			{
				tbDir.Text = fbd.SelectedPath;
			}
		}
	}
}
