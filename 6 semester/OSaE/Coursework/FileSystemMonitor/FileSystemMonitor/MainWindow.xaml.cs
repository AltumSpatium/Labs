using System.Windows;
using System.Data.Entity;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Forms;

using System.IO;

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
		}

		private void checkDirectory(object sender, RoutedEventArgs e)
		{
			db.Logs.Add(new LogItem());
			db.SaveChanges();
		}		

		private static void OnChanged(object source, FileSystemEventArgs e)
		{

		}

		private static void OnRenamed(object source, RenamedEventArgs e)
		{

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
			}
			else
			{
				btnStartMonitoring.Content = "Начать мониторинг";
				enableInterface(true);
			}

		}

		private void enableInterface(bool enabled)
		{
			tbDir.IsEnabled = enabled;
			tbFilter.IsEnabled = enabled;
			btnOpenDir.IsEnabled = enabled;
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
