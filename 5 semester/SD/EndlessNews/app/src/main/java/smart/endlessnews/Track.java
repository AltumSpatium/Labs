package smart.endlessnews;

import android.os.Parcel;
import android.os.Parcelable;

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

    public int getID() {
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
