using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab7
{
    public class Playlist : IEnumerable<Composition>, INotifyCollectionChanged
    {
        List<Composition> compositions = new List<Composition>();
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public int ID { get; set; }

        public string Title { get; set; }

        public TimeSpan Length { get; set; }

        public double Rating { get; set; }

        public int Count
        {
            get { return compositions.Count; }
        }

        public List<Composition> Compositions
        {
            get { return compositions; }
        }

        public void AddComposition(Composition composition)
        {
            if (compositions.Find(comp => (comp.ID == composition.ID)) != null)
                return;
            compositions.Add(composition);
            int seconds = (compositions.Sum(comp => comp.Length.Seconds) + compositions.Sum(comp => comp.Length.Minutes) * 60 + compositions.Sum(comp => comp.Length.Hours) * 3600);
            Length = new TimeSpan(seconds / 3600, (seconds % 3600) / 60, (seconds % 3600) % 60);
            if (compositions.Count != 0)
                Rating = compositions.Average(comp => comp.Rating);
            else Rating = 0;

            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void RemoveComposition(Composition composition)
        {
            compositions.Remove(composition);
            int seconds = (compositions.Sum(comp => comp.Length.Seconds) + compositions.Sum(comp => comp.Length.Minutes) * 60 + compositions.Sum(comp => comp.Length.Hours) * 3600);
            Length = new TimeSpan(seconds / 3600, (seconds % 3600) / 60, (seconds % 3600) % 60);
            if (compositions.Count != 0)
                Rating = compositions.Average(comp => comp.Rating);
            else Rating = 0;
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public Playlist()
        {
            ID = 0;
            Title = "";
            Length = new TimeSpan(0, 0, 0);
            Rating = 0;
        }

        public Playlist(int _id, string _title)
        {
            ID = _id;
            Title = _title;
            Length = new TimeSpan(0, 0, 0);
            Rating = 0;
        }

        public IEnumerator<Composition> GetEnumerator()
        {
            foreach (var p in compositions)
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
