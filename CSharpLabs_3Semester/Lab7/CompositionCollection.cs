using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab7
{
    public class CompositionCollection : IEnumerable<Composition>, INotifyCollectionChanged
    {
        List<Composition> compositions = new List<Composition>();
        public event NotifyCollectionChangedEventHandler CollectionChanged;

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
            if (compositions.Find(pl => (pl.ID == composition.ID)) != null)
                return;
            compositions.Add(composition);
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void RemoveComposition(Composition composition)
        {
            compositions.Remove(composition);
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void UpdateComposition(Composition composition, string newTitle, string newPerformer, int newMinutes, int newSeconds, Composition.Genres newGenre, int newRating)
        {
            foreach (var comp in compositions)
            {
                if (comp == composition)
                {
                    comp.Title = "\"" + newTitle + "\"";
                    comp.Performer = newPerformer;
                    comp.Length = new TimeSpan(0, newMinutes, newSeconds);
                    comp.Genre = newGenre;
                    comp.Rating = newRating;
                }
            }
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public IEnumerator<Composition> GetEnumerator()
        {
            foreach (var c in compositions)
            {
                yield return c;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}
