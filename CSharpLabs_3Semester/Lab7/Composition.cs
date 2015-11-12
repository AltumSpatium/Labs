using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Lab7
{
    public class Composition
    {
        public enum Genres
        {
            Rock,
            Rap,
            Pop,
            Country,
            Jazz,
            No_genre
        }

        public int ID { get; set; }

        public string Title { get; set; }

        public TimeSpan Length { get; set; }

        public string Performer { get; set; }

        public Genres Genre { get; set; }

        public int Rating { get; set; }

        public Composition()
        {
            ID = 0;
            Title = "";
            Length = new TimeSpan(0, 0, 0);
            Performer = "";
            Genre = 0;
            Rating = 0;
        }

        public Composition(int _id, string _title, string _length, string _performer, Genres _genre, int _rating)
        {
            ID = _id;
            Title = "\"" + _title + "\"";
            Length = TimeSpan.Parse(_length);
            Performer = _performer;
            Genre = _genre;
            Rating = _rating;
        }
    }
}
