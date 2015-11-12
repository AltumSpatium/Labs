using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;

namespace Lab9
{
    public class Group : IEnumerable<Student>, INotifyCollectionChanged
    {
        public List<Student> students = new List<Student>();
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public string Number { get; set; }

        public Group()
        {
            Number = "";
        }

        public Group(string number)
        {
            Number = number;
        }

        public int Count
        {
            get { return students.Count; }
        }

        public void AddStudent(Student student)
        {
            students.Add(student);
            student.Group = Number;
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void ExpellStudent(Student student)
        {
            students.Remove(student);
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void ClearGroup()
        {
            students.Clear();
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public IEnumerator<Student> GetEnumerator()
        {
            foreach (Student g in students)
            {
                yield return g;
            }
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }
    }
}