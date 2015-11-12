using System;
using System.Linq;
using System.Threading.Tasks;
using System.Threading;
using System.Windows;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Windows.Controls;
using System.IO;

namespace Lab7
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        CompositionCollection compositions = new CompositionCollection();
        PlaylistCollection playlists = new PlaylistCollection();
        Window1 w1;
        Window2 w2;

        public MainWindow()
        {
            InitializeComponent();
            if (File.Exists("Playlists.txt"))
            {
                Serializer slop1 = new Serializer("Playlists.txt", false);
                playlists = (PlaylistCollection)slop1.ReadObject();
                slop1.Close();
            }
            if (File.Exists("Compositions.txt"))
            {
                Serializer slop2 = new Serializer("Compositions.txt", false);
                compositions = (CompositionCollection)slop2.ReadObject();
                slop2.Close();
            }
            listbox1.ItemsSource = playlists;
        }

        private void menuItemCreatePlaylist_Click(object sender, RoutedEventArgs e)
        {
            w2 = new Window2(compositions, playlists);
            w2.Show();
        }

        private void menuItemAddComposition_Click(object sender, RoutedEventArgs e)
        {
            w1 = new Window1(compositions);
            w1.Show();
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            if (w1 != null)
                w1.Close();
            if (w2 != null)
                w2.Close();
            Serializer slcl1 = new Serializer("Playlists.txt", true);
            Serializer slcl2 = new Serializer("Compositions.txt", true);
            slcl1.WriteObject(playlists);
            slcl2.WriteObject(compositions);
            slcl1.Close();
            slcl2.Close();
        }

        private void menuItemPlay_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null)
            {
                UserControl1 uc = new UserControl1(tabcontrol1, (Playlist)listbox1.SelectedItem);
                TabItem ti = new TabItem();
                ti.Content = uc;
                ti.Header = ((Playlist)listbox1.SelectedItem).Title;
                tabcontrol1.Items.Add(ti);
            }
        }
    }
}
