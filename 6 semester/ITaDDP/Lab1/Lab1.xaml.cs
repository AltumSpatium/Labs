using System;
using System.Text;
using System.Windows;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.ComponentModel;

namespace UdpChat
{
	public partial class MainWindow : Window
	{
		private static int localPort;
		private static int remotePort;
		private static IPAddress remoteIPAddress;

		private static bool receiving = false;
		private Thread receivingThread;

		private void Send(string datagram)
		{
			UdpClient sender = new UdpClient();
			IPEndPoint endPoint = new IPEndPoint(remoteIPAddress, remotePort);

			try
			{
				byte[] bytes = Encoding.UTF8.GetBytes(datagram);
				sender.Send(bytes, bytes.Length, endPoint);

			}
			catch (Exception e) {}
			finally { sender.Close(); }
		}

		public void Receive()
		{
			UdpClient receiver = new UdpClient(localPort);
			IPEndPoint remoteIpEndPoint = null;

			try
			{
				while (receiving)
				{
					byte[] receivedBytes = receiver.Receive(ref remoteIpEndPoint);
					string receivedData = Encoding.UTF8.GetString(receivedBytes);

					Dispatcher.Invoke(() => { textBlockChat.Text += receivedData + "\n"; });
				}
			}
			catch (Exception e)	{}
			finally	{ receiver.Close(); }
		}

		public MainWindow()
		{
			InitializeComponent();
		}

		private void buttonChange_Click(object sender, RoutedEventArgs e)
		{
			if (textBoxLocalPort.Text == "" || textBoxRemotePort.Text == "" || textBoxRemoteIPAddress.Text == "")
				return;

			buttonChange.Visibility = Visibility.Hidden;

			labelLocalPort.Visibility = Visibility.Visible;
			labelRemotePort.Visibility = Visibility.Visible;
			labelRemoteIPAddress.Visibility = Visibility.Visible;

			textBoxLocalPort.Visibility = Visibility.Hidden;
			textBoxRemotePort.Visibility = Visibility.Hidden;
			textBoxRemoteIPAddress.Visibility = Visibility.Hidden;

			labelLocalPort.Content = textBoxLocalPort.Text;
			labelRemotePort.Content = textBoxRemotePort.Text;
			labelRemoteIPAddress.Content = textBoxRemoteIPAddress.Text;

			localPort = Convert.ToInt32(textBoxLocalPort.Text);
			remotePort = Convert.ToInt32(textBoxRemotePort.Text);
			remoteIPAddress = IPAddress.Parse(textBoxRemoteIPAddress.Text);

			receivingThread = new Thread(new ThreadStart(Receive));
			receivingThread.Start();

			receiving = true;
		}

		private void buttonClear_Click(object sender, RoutedEventArgs e)
		{
			textBlockChat.Text = "";
		}

		private void buttonSend_Click(object sender, RoutedEventArgs e)
		{
			if (textBoxSend.Text == "" || !receiving) return;

			string message = textBoxSend.Text;
			textBoxSend.Text = "";

			textBlockChat.Text += "You: " + message + "\n";
			Send(message);
		}

		private void Window_Closing(object sender, CancelEventArgs e)
		{
			receiving = false;
			receivingThread.Abort();
		}

		private void textBoxSend_KeyDown(object sender, System.Windows.Input.KeyEventArgs e)
		{
			if (e.Key == System.Windows.Input.Key.Enter)
			{
				buttonSend_Click(sender, e);
			}
		}
	}
}
