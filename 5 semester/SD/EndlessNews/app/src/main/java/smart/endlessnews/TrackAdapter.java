package smart.endlessnews;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;

import java.util.ArrayList;

class TrackAdapter extends BaseAdapter {
    private Context ctx;
    private ArrayList<Track> tracks;
    private LayoutInflater lInflater;

    TrackAdapter(Context ctx, ArrayList<Track> tracks) {
        this.ctx = ctx;
        this.tracks = tracks;
        lInflater = (LayoutInflater) ctx.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    @Override
    public int getCount() {
        return tracks.size();
    }

    @Override
    public Track getItem(int position) {
        return tracks.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(final int position, View convertView, ViewGroup parent) {
        View view = convertView != null ? convertView :
                lInflater.inflate(R.layout.track_item, parent, false);;
        final Track track = getItem(position);

        ((TextView)view.findViewById(R.id.tvTrackTitle)).setText(track.getTitle());
        ((TextView)view.findViewById(R.id.tvTrackArtist)).setText(track.getArtist());

        view.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(ctx, TrackActivity.class);

                intent.putExtra("current_track", track);
                ctx.startActivity(intent);
            }
        });

        view.setOnLongClickListener(new View.OnLongClickListener() {
            public boolean onLongClick(View view) {
                final CharSequence[] items = { "Delete" };

                AlertDialog.Builder builder = new AlertDialog.Builder(ctx);

                builder.setTitle("Action:");
                builder.setItems(items, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int item) {
                        DBHelper dbHelper = new DBHelper(ctx);
                        SQLiteDatabase db = dbHelper.getWritableDatabase();
                        TrackRepository repository = new TrackRepository();

                        repository.connect(db);
                        repository.delete(track.getTitle());
                        tracks.remove(position);
                        notifyDataSetChanged();
                    }
                });

                AlertDialog alert = builder.create();

                alert.show();
                return false;
            }
        });

        //((AudioPlayerActivity)ctx).registerForContextMenu(view);

        return view;
    }
}
