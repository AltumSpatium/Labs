using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab7
{
    public class PlaylistCollection : IEnumerable<Playlist>, INotifyCollectionChanged
    {
        List<Playlist> playlists = new List<Playlist>();
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public int Count
        {
            get { return playlists.Count; }
        }

        public List<Playlist> Playlists
        {
            get { return playlists; }
        }

        public void AddPlaylist(Playlist playlist)
        {
            if (playlists.Find(pl => (pl.ID == playlist.ID)) != null)
                return;
            playlists.Add(playlist);
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void RemovePlaylist(Playlist playlist)
        {
            playlists.Remove(playlist);
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void UpdatePlaylists()
        {
            playlists.Reverse();
            playlists.Reverse();
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public IEnumerator<Playlist> GetEnumerator()
        {
            foreach (var p in playlists)
            {
                yield return p;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
