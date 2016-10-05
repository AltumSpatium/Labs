package smart.endlessnews;

import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class TrackActivity extends AppCompatActivity implements View.OnClickListener {
    Track track;

    boolean bound = false;
    ServiceConnection serviceConnection;
    AudioPlayService service;
    Intent intent;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_track);

        track = getIntent().getParcelableExtra("current_track");
//        intent = savedInstanceState == null ? new Intent(this, AudioPlayService.class) :
//                (Intent)savedInstanceState.getParcelable("intent");
        intent = new Intent(this, AudioPlayService.class);
        intent.putExtra("track", track);

        ((TextView)findViewById(R.id.tvPlayingTitle)).setText(track.getTitle());
        ((TextView)findViewById(R.id.tvPlayingArtist)).setText(track.getArtist());

        serviceConnection = new ServiceConnection() {
            @Override
            public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
                service = ((AudioPlayService.AudioPlayBinder)iBinder).getService();
                bound = true;
            }

            @Override
            public void onServiceDisconnected(ComponentName componentName) {
                bound = false;
            }
        };
    }

    protected void onSaveInstanceState(Bundle outState) {
        outState.putParcelable("intent", intent);
    }

    @Override
    protected void onStart() {
        super.onStart();
        bindService(intent, serviceConnection, 0);
    }

    @Override
    protected void onStop() {
        super.onStop();
        if (bound) {
            unbindService(serviceConnection);
            bound = false;
        }
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnPlay:
                if (!bound) {
                    startService(intent);
                }
                else service.play();
                if (((Button)findViewById(R.id.btnPause)).getText().equals("Resume"))
                    ((Button)findViewById(R.id.btnPause)).setText(R.string.pause);
                break;
            case R.id.btnPause:
                if (((Button)view).getText().equals("Pause"))
                    ((Button)view).setText(R.string.resume);
                else if (((Button)view).getText().equals("Resume"))
                    ((Button)view).setText(R.string.pause);
                service.pauseOrResume();
                break;
            case R.id.btnStop:
                if (bound) {
                    service.stop();
                    stopService(intent);
                    bound = false;
                }
                break;
        }
    }
}
