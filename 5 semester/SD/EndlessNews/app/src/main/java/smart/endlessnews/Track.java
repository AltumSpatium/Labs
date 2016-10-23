package smart.endlessnews;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Parcel;
import android.os.Parcelable;

import java.util.ArrayList;

class Track implements Parcelable {
    private int mID;
    private String mTitle;
    private String mArtist;
    private String mData;

    Track(int id, String title, String artist, String data) {
        this.mID = id;
        this.mTitle = title;
        this.mArtist = artist;
        this.mData = data;
    }

    private Track(Parcel in) {
        this.mID = in.readInt();
        this.mTitle = in.readString();
        this.mArtist = in.readString();
        this.mData = in.readString();
    }

    int getID() {
        return mID;
    }

    String getTitle() {
        return mTitle;
    }

    String getArtist() {
        return mArtist;
    }

    String getData() {
        return mData;
    }

    public int describeContents() {
        return 0;
    }

    public void writeToParcel(Parcel parcel, int flags) {
        parcel.writeInt(mID);
        parcel.writeString(mTitle);
        parcel.writeString(mArtist);
        parcel.writeString(mData);
    }

    public static final Parcelable.Creator<Track> CREATOR = new Parcelable.Creator<Track>() {
        public Track createFromParcel(Parcel in) {
            return new Track(in);
        }

        public Track[] newArray(int size) {
            return new Track[size];
        }
    };
}

class TrackRepository {
    private SQLiteDatabase db;

    void connect(SQLiteDatabase db) {
        this.db = db;
    }

    void create() {
        db.execSQL(
                "create table if not exists TrackTable ("
                + "id integer primary key autoincrement, "
                + "title text, "
                + "artist text, "
                + "data text" + ");"
        );
    }

    void add(Track track) {
        ContentValues cv = new ContentValues();

        cv.put("title", track.getTitle());
        cv.put("artist", track.getArtist());
        cv.put("data", track.getData());

        db.insert("TrackTable", null, cv);
    }

    void delete(String title) {
        db.delete("TrackTable", "title = '" + title + "'", null);
    }

    ArrayList<Track> loadAll() {
        ArrayList<Track> tracks = new ArrayList<>();
        Cursor c = db.query("TrackTable", null, null, null, null, null, null);

        if (c != null) {
            if (c.moveToFirst()) {
                int idColIndex = c.getColumnIndex("id");
                int titleColIndex = c.getColumnIndex("title");
                int artistColIndex = c.getColumnIndex("artist");
                int dataColIndex = c.getColumnIndex("data");

                do {
                    int id = c.getInt(idColIndex);
                    String title = c.getString(titleColIndex);
                    String artist = c.getString(artistColIndex);
                    String data = c.getString(dataColIndex);

                    tracks.add(new Track(id, title, artist, data));
                } while (c.moveToNext());
            }

            c.close();
        }

        return tracks;
    }

    void saveAll(ArrayList<Track> tracks) {
        db.delete("TrackTable", null, null);

        for (Track t : tracks)
            add(t);
    }
}
