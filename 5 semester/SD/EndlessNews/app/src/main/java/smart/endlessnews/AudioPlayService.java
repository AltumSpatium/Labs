package smart.endlessnews;

import android.app.Service;
import android.content.Intent;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.net.Uri;
import android.os.Binder;
import android.os.IBinder;

public class AudioPlayService extends Service {
    AudioPlayBinder binder = new AudioPlayBinder();
    MediaPlayer mediaPlayer = new MediaPlayer();
    Track track;

    public AudioPlayService() {
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        track = intent.getParcelableExtra("track");
        try {
            Uri uri = Uri.parse("file://" + track.getData());
            mediaPlayer.setDataSource(this, uri);
            mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
            mediaPlayer.prepare();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public IBinder onBind(Intent intent) {
        return binder;
    }

    public void play() {
        mediaPlayer.start();
    }

    public void pauseOrResume() {
        if (mediaPlayer.isPlaying())
            mediaPlayer.pause();
        else mediaPlayer.start();
    }

    public void stop() {
        mediaPlayer.stop();
    }

    class AudioPlayBinder extends Binder {
        AudioPlayService getService() {
            return AudioPlayService.this;
        }
    }
}
