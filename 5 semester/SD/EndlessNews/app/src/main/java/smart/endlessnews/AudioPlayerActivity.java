package smart.endlessnews;

import android.content.ContentResolver;
import android.database.Cursor;
import android.net.Uri;
import android.provider.MediaStore;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ListView;

import java.util.ArrayList;

public class AudioPlayerActivity extends AppCompatActivity {

    ArrayList<Track> tracks = new ArrayList<>();
    TrackAdapter trackAdapter;

    ListView lvTracks;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_audio_player);
        if (getSupportActionBar() != null)
            getSupportActionBar().hide();

        tracks = findTracks();
        trackAdapter = new TrackAdapter(this, tracks);
        lvTracks = (ListView)findViewById(R.id.lvTracks);

        if (lvTracks != null)
            lvTracks.setAdapter(trackAdapter);
    }

    public ArrayList<Track> findTracks() {
        ArrayList<Track> tracks = new ArrayList<>();

        String[] projection = {
                MediaStore.Audio.Media._ID,
                MediaStore.Audio.Media.ARTIST,
                MediaStore.Audio.Media.TITLE,
                MediaStore.Audio.Media.DATA
        };

        Uri uriExternal = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI;
        Uri uriInternal = MediaStore.Audio.Media.INTERNAL_CONTENT_URI;

        ContentResolver contentResolver = getContentResolver();

        Cursor cursorExt = contentResolver.query(uriExternal, projection, null, null, null);
        Cursor cursorInt = contentResolver.query(uriInternal, projection, null, null, null);

        if (cursorExt != null) {
            for (int i = 0; i < cursorExt.getCount(); i++) {
                cursorExt.moveToPosition(i);

                int id = cursorExt.getInt(cursorExt.getColumnIndex(MediaStore.Audio.Media._ID));
                String title = cursorExt.getString(cursorExt.getColumnIndex(MediaStore.Audio.Media.TITLE));
                String artist = cursorExt.getString(cursorExt.getColumnIndex(MediaStore.Audio.Media.ARTIST));
                String data = cursorExt.getString(cursorExt.getColumnIndex(MediaStore.Audio.Media.DATA));

                Track track = new Track(id, title, artist, data);
                if (!track.getArtist().equals("<unknown>"))
                    tracks.add(track);
            }
            cursorExt.close();
        }

        if (cursorInt != null) {
            for (int i = 0; i < cursorInt.getCount(); i++) {
                cursorInt.moveToPosition(i);

                int id = cursorInt.getInt(cursorInt.getColumnIndex(MediaStore.Audio.Media._ID));
                String title = cursorInt.getString(cursorInt.getColumnIndex(MediaStore.Audio.Media.TITLE));
                String artist = cursorInt.getString(cursorInt.getColumnIndex(MediaStore.Audio.Media.ARTIST));
                String data = cursorInt.getString(cursorInt.getColumnIndex(MediaStore.Audio.Media.DATA));

                Track track = new Track(id, title, artist, data);
                if (!track.getArtist().equals("<unknown>"))
                    tracks.add(track);
            }
            cursorInt.close();
        }

        return tracks;
    }
}
