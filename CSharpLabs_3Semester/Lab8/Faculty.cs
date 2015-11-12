using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Windows;

namespace Lab8
{
    public class Faculty : IEnumerable<Group>, INotifyCollectionChanged
    {
        public string name = "FCSaN";
        public List<Group> groups = new List<Group>();
        public event NotifyCollectionChangedEventHandler CollectionChanged;

        public int Count
        {
            get { return groups.Count; }
        }

        public void AddGroup(Group group)
        {
            bool found = false;
            foreach (Group gr in this)
            {
                if (gr.Number == group.Number)
                {
                    MessageBox.Show("Such group already exists!");
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                groups.Add(group);
                if (CollectionChanged != null)
                    CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
            }
        }

        public void RemoveGroup(Group group)
        {
            groups.Remove(group);
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void ClearFaculty()
        {
            groups.Clear();
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public void MergeGroups(Group first, Group second)
        {
            for (int i = 0; i < second.Count; i++)
            {
                first.AddStudent(second.students[i]);
            }
            this.RemoveGroup(second);
            if (CollectionChanged != null)
                CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
        }

        public Student SearchStudent(string name, string surname, int age)
        {
            foreach (Group gr in this)
            {
                foreach (Student st in gr)
                {
                    if (st.Name == name && st.Surname == surname && st.Age == age)
                    {
                        return st;
                    }
                }
            }
            return null;
        }

        public void TransferStudent(Student student, string number)
        {
            Student transferedStudent = new Student("", "");
            bool found = false;
            foreach (Group gr in this)
            {
                if (gr.Number == number)
                {
                    found = true;
                    break;
                }
            }
            if (found)
            {
                foreach (Group gr in this)
                {
                    foreach (Student st in gr)
                    {
                        if (st.Name == student.Name && st.Surname == student.Surname)
                        {
                            transferedStudent.Name = st.Name;
                            transferedStudent.Surname = st.Surname;
                            transferedStudent.Age = st.Age;
                            transferedStudent.Group = st.Group;
                            gr.ExpellStudent(st);
                            break;
                        }
                    }
                }
                foreach (Group gr in this)
                {
                    if (gr.Number == number)
                    {
                        gr.AddStudent(transferedStudent);
                        break;
                    }
                }
                if (CollectionChanged != null)
                    CollectionChanged(this, new NotifyCollectionChangedEventArgs(NotifyCollectionChangedAction.Reset));
            }
        }

        public IEnumerator<Group> GetEnumerator()
        {
            foreach (Group g in groups)
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
