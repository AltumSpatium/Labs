using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace Lab7
{
    /// <summary>
    /// Interaction logic for Window2.xaml
    /// </summary>
    public partial class Window2 : Window
    {
        CompositionCollection compositions = new CompositionCollection();
        PlaylistCollection playlists = new PlaylistCollection();

        public Window2(CompositionCollection _compositions, PlaylistCollection _playlists)
        {
            InitializeComponent();
            compositions = _compositions;
            playlists = _playlists;
            listbox1.ItemsSource = compositions;
            listbox2.ItemsSource = playlists;
        }

        private void buttonAddPlaylist_Click(object sender, RoutedEventArgs e)
        {
            if (textboxTitle.Text != null)
            {
                playlists.AddPlaylist(new Playlist((playlists.Count > 0 ? playlists.Playlists[playlists.Count - 1].ID + 1 : 1), textboxTitle.Text));
                textboxTitle.Text = "";
            }
        }

        private void buttonRemovePlaylist_Click(object sender, RoutedEventArgs e)
        {
            if (listbox2.SelectedItem != null)
            {
                playlists.RemovePlaylist((Playlist)listbox2.SelectedItem);
                listbox3.ItemsSource = null;
            }
        }

        private void buttonAddComposition_Click(object sender, RoutedEventArgs e)
        {
            if (listbox1.SelectedItem != null && listbox2.SelectedItem != null)
            {
                Composition selectedComposition = (Composition)listbox1.SelectedItem;
                Playlist selectedPlaylist = (Playlist)listbox2.SelectedItem;
                selectedPlaylist.AddComposition(selectedComposition);
                playlists.UpdatePlaylists();
                listbox1.UnselectAll();
                listbox2.UnselectAll();
            }
        }

        private void listbox2_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (listbox2.SelectedItem != null)
            {
                Playlist selectedPlaylist = (Playlist)listbox2.SelectedItem;
                listbox3.ItemsSource = selectedPlaylist;
            }
        }

        private void buttonRemoveComposition_Click(object sender, RoutedEventArgs e)
        {
            if (listbox2.SelectedItem != null && listbox3.SelectedItem != null)
            {
                Composition selectedComposition = (Composition)listbox3.SelectedItem;
                Playlist selectedPlaylist = (Playlist)listbox2.SelectedItem;
                selectedPlaylist.RemoveComposition(selectedComposition);
                playlists.UpdatePlaylists();
                listbox2.UnselectAll();
            }
        }
    }
}
