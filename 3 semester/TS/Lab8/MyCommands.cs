using System;
using System.Collections;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Input;
using Microsoft.Win32;

namespace Lab8
{
    public class MyCommands
    {
        private static RoutedUICommand addGroup;
        private static RoutedUICommand removeGroup;
        private static RoutedUICommand addStudent;
        private static RoutedUICommand expellStudent;
        private static RoutedUICommand chooseMerge;
        private static RoutedUICommand merge;
        private static RoutedUICommand chooseTransfer;
        private static RoutedUICommand transfer;
        private static RoutedUICommand cancel;

        static MyCommands()
        {
            InputGestureCollection inputs = new InputGestureCollection();
            inputs.Add(new KeyGesture(Key.G, ModifierKeys.Alt, "Alt+G"));
            addGroup = new RoutedUICommand("Add group", "Add group", typeof(MyCommands), inputs);
            inputs = new InputGestureCollection();
            inputs.Add(new KeyGesture(Key.R, ModifierKeys.Alt, "Alt+R"));
            removeGroup = new RoutedUICommand("Remove group", "Remove group", typeof(MyCommands), inputs);
            inputs = new InputGestureCollection();
            inputs.Add(new KeyGesture(Key.S, ModifierKeys.Alt, "Alt+S"));
            addStudent = new RoutedUICommand("Add student", "Add student", typeof(MyCommands), inputs);
            inputs = new InputGestureCollection();
            inputs.Add(new KeyGesture(Key.E, ModifierKeys.Alt, "Alt+E"));
            expellStudent = new RoutedUICommand("Add student", "Add student", typeof(MyCommands), inputs);
            inputs = new InputGestureCollection();
            inputs.Add(new KeyGesture(Key.C, ModifierKeys.Alt, "Alt+C"));
            chooseMerge = new RoutedUICommand("Choose", "Choose", typeof(MyCommands), inputs);
            inputs = new InputGestureCollection();
            inputs.Add(new KeyGesture(Key.M, ModifierKeys.Alt, "Alt+M"));
            merge = new RoutedUICommand("Merge", "Merge", typeof(MyCommands), inputs);
            inputs = new InputGestureCollection();
            inputs.Add(new KeyGesture(Key.X, ModifierKeys.Alt, "Alt+X"));
            chooseTransfer = new RoutedUICommand("Choose", "Choose", typeof(MyCommands), inputs);
            inputs = new InputGestureCollection();
            inputs.Add(new KeyGesture(Key.T, ModifierKeys.Alt, "Alt+T"));
            transfer = new RoutedUICommand("Transfer", "Transfer", typeof(MyCommands), inputs);
            inputs = new InputGestureCollection();
            inputs.Add(new KeyGesture(Key.Z, ModifierKeys.Control, "Ctrl+Z"));
            cancel = new RoutedUICommand("Cancel", "Cancel", typeof(MyCommands), inputs);
        }

        public static RoutedUICommand AddGroup
        {
            get { return addGroup; }
        }

        public static RoutedUICommand RemoveGroup
        {
            get { return removeGroup; }
        }

        public static RoutedUICommand AddStudent
        {
            get { return addStudent; }
        }

        public static RoutedUICommand ExpellStudent
        {
            get { return expellStudent; }
        }

        public static RoutedUICommand ChooseMerge
        {
            get { return chooseMerge; }
        }

        public static RoutedUICommand Merge
        {
            get { return merge; }
        }

        public static RoutedUICommand ChooseTransfer
        {
            get { return chooseTransfer; }
        }

        public static RoutedUICommand Transfer
        {
            get { return transfer; }
        }

        public static RoutedUICommand Cancel
        {
            get { return cancel; }
        }
    }
}