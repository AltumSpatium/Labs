using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Media;

namespace Lab7
{
    /// <summary>
    /// Interaction logic for UserControl1.xaml
    /// </summary>
    public partial class UserControl1 : UserControl
    {
        TabControl tabcontrol1;
        Thread th = new Thread(Action);
        ThreadParams tp;
        SynchronizationContext _sunccontext;

        public enum PlayingStatus
        {
            BeginPlaying,
            PlayingProgress,
            EndPlaying
        }

        public enum PlayingCommand
        {
            Play,
            Pause,
            Stop
        }

        public class ThreadParams
        {
            public UserControl1 player;
            public bool isTerminate;
            public PlayingCommand command;
            public Playlist playlist;
            public int doStep;
        }
        
        private static void Action(object _object)
        {
            ThreadParams thparams = (ThreadParams) _object;

            Composition currcomp = (thparams.playlist.Compositions.Count > 0 ? thparams.playlist.Compositions[0] : null);
            int seconds = (currcomp != null ? (currcomp.Length.Minutes * 60 + currcomp.Length.Seconds) : 0);
            int currindex = 0;
            int currsec = 0;

            while (!thparams.isTerminate)
            {
                Thread.Sleep(100);
                
                if (thparams.command == PlayingCommand.Play)
                {
                    if (currcomp == null)
                        continue;

                    if (thparams.doStep > 0 && (thparams.playlist.Compositions.Count > (currindex + 1)))
                    {
                        SystemSounds.Beep.Play();
                        thparams.player.OnStatus(PlayingStatus.EndPlaying, currcomp, 100);

                        currindex++;
                        currcomp = (thparams.playlist.Compositions.Count > currindex ? thparams.playlist.Compositions[currindex] : null);
                        seconds = (currcomp != null ? (currcomp.Length.Minutes * 60 + currcomp.Length.Seconds) : 0);
                        currsec = 0;
                        thparams.doStep = 0;
                        continue;
                    }
                    else if (thparams.doStep < 0 && (currindex > 0))
                    {
                        SystemSounds.Beep.Play();
                        thparams.player.OnStatus(PlayingStatus.EndPlaying, currcomp, 100);

                        currindex--;
                        currcomp = (thparams.playlist.Compositions.Count > currindex ? thparams.playlist.Compositions[currindex] : null);
                        seconds = (currcomp != null ? (currcomp.Length.Minutes * 60 + currcomp.Length.Seconds) : 0);
                        currsec = 0;
                        thparams.doStep = 0;
                        continue;
                    }

                    if (currsec == 0)
                        thparams.player.OnStatus(PlayingStatus.BeginPlaying, currcomp, 0);
                    
                    currsec++;

                    if ((currsec / 10) >= seconds)
                    {
                        SystemSounds.Beep.Play();
                        thparams.player.OnStatus(PlayingStatus.EndPlaying, currcomp, 100);
                        
                        currindex++;
                        currcomp = (thparams.playlist.Compositions.Count > currindex ? thparams.playlist.Compositions[currindex] : null);
                        seconds = (currcomp != null ? (currcomp.Length.Minutes * 60 + currcomp.Length.Seconds) : 0);
                        currsec = 0;
                    }
                    else if ((currsec % 50) == 0)
                    {
                        SystemSounds.Beep.Play();
                        int progress = (currsec * 10)/ seconds;
                        thparams.player.OnStatus(PlayingStatus.PlayingProgress, currcomp, progress);                                                
                    }
                }
                else if (thparams.command == PlayingCommand.Pause)
                {
                   if (currcomp == null)
                       continue;

                   if (thparams.doStep > 0 && (thparams.playlist.Compositions.Count > (currindex + 1)))
                   {
                       SystemSounds.Beep.Play();
                       thparams.player.OnStatus(PlayingStatus.EndPlaying, currcomp, 100);

                       currindex++;
                       currcomp = (thparams.playlist.Compositions.Count > currindex ? thparams.playlist.Compositions[currindex] : null);
                       seconds = (currcomp != null ? (currcomp.Length.Minutes * 60 + currcomp.Length.Seconds) : 0);
                       currsec = 0;
                       thparams.doStep = 0;
                       continue;
                   }
                   else if (thparams.doStep < 0 && (currindex > 0))
                   {
                       SystemSounds.Beep.Play();
                       thparams.player.OnStatus(PlayingStatus.EndPlaying, currcomp, 100);

                       currindex--;
                       currcomp = (thparams.playlist.Compositions.Count > currindex ? thparams.playlist.Compositions[currindex] : null);
                       seconds = (currcomp != null ? (currcomp.Length.Minutes * 60 + currcomp.Length.Seconds) : 0);
                       currsec = 0;
                       thparams.doStep = 0;
                       continue;
                   }
                }
                else if (thparams.command == PlayingCommand.Stop)
                {
                    currcomp = (thparams.playlist.Compositions.Count > 0 ? thparams.playlist.Compositions[0] : null);
                    seconds = (currcomp != null ? (currcomp.Length.Minutes * 60 + currcomp.Length.Seconds) : 0);
                    currindex = 0; currsec = 0;
                }
            }
        }

        public void OnStatus(PlayingStatus ps, Composition comp, int position)
        {
            if (ps == PlayingStatus.BeginPlaying)
            {
                _sunccontext.Post(_ => textboxPlaying.Text = comp.Title + " " + comp.Performer + " " + comp.Length, null);
                _sunccontext.Post(_ => slider1.Value = position, null);
            }
            else if (ps == PlayingStatus.PlayingProgress)
            {
                _sunccontext.Post(_ => textboxPlaying.Text = comp.Title + " " + comp.Performer + " " + comp.Length, null);
                _sunccontext.Post(_ => slider1.Value = position, null);
            }
            else if (ps == PlayingStatus.EndPlaying)
            {
                _sunccontext.Post(_ => textboxPlaying.Text = comp.Title + " " + comp.Performer + " " + comp.Length, null);
                _sunccontext.Post(_ => slider1.Value = position, null);
            }
        }

        public UserControl1(TabControl _tabcontrol1, Playlist _playlist)
        {
            InitializeComponent();
            _sunccontext = SynchronizationContext.Current;
            tabcontrol1 = _tabcontrol1;
            tp = new ThreadParams() { player = this, command = PlayingCommand.Stop, playlist = _playlist, isTerminate = false, doStep = 0 };
            th.IsBackground = true;
            th.Start(tp);
        }

        private void close_Click(object sender, RoutedEventArgs e)
        {
            foreach (TabItem item in tabcontrol1.Items)
            {
                if (item.Content == this)
                {
                    tp.isTerminate = true;
                    th.Join();

                    tabcontrol1.Items.Remove(item);
                    return;
                }
            }            
        }

        private void buttonPlay_Click(object sender, RoutedEventArgs e)
        {
            if (tp.command == PlayingCommand.Pause || tp.command == PlayingCommand.Stop)
            {
                tp.command = PlayingCommand.Play;
                buttonPlay.Content = "Pause";
            }
            else if (tp.command == PlayingCommand.Play)
            {
                tp.command = PlayingCommand.Pause;
                buttonPlay.Content = "Play";
            }
        }

        private void buttonStop_Click(object sender, RoutedEventArgs e)
        {
            tp.command = PlayingCommand.Stop;
            buttonPlay.Content = "Play";
            textboxPlaying.Text = "";
            slider1.Value = 0;
        }

        private void buttonNext_Click(object sender, RoutedEventArgs e)
        {
            tp.doStep = 1;
        }

        private void buttonPrev_Click(object sender, RoutedEventArgs e)
        {
            tp.doStep = -1;
        }
    }
}
