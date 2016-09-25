package smart.endlessnews;

import android.os.Parcel;
import android.os.Parcelable;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.Date;

public class News implements Parcelable{
    private String mTitle;
    private String mDescription;
    private String mFullText;
    private String mLink;
    private String mPicture;
    private String mCategory;
    private Date mPubDate;

    public News(String title, String description, String fullText,
                String link, String picture, String category, Date pubDate)
    {
        this.mTitle = title;
        this.mDescription = description;
        this.mFullText = fullText;
        this.mLink = link;
        this.mPicture = picture;
        this.mCategory = category;
        this.mPubDate = pubDate;
    }

    public String getTitle() {
        return mTitle;
    }

    public void setTitle(String title) {
        mTitle = title;
    }

    public String getDescription() {
        return mDescription;
    }

    public void setDescription(String description) {
        mDescription = description;
    }

    public String getFullText() {
        return mFullText;
    }

    public void setFullText(String fullText) {
        mFullText = fullText;
    }

    public String getLink() {
        return mLink;
    }

    public void setLink(String link) {
        mLink = link;
    }

    public String getPicture() {
        return mPicture;
    }

    public void setPicture(String picture) {
        mPicture = picture;
    }

    public String getCategory() {
        return mCategory;
    }

    public void setCategory(String category) {
        mCategory = category;
    }

    public Date getPubDate() {
        return mPubDate;
    }

    public void setPubDate(Date pubDate) {
        mPubDate = pubDate;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int flags) {
        parcel.writeString(mTitle);
        parcel.writeString(mDescription);
        parcel.writeString(mFullText);
        parcel.writeString(mLink);
        parcel.writeString(mPicture);
        parcel.writeString(mCategory);
        parcel.writeString(mPubDate.toString());
    }

    public static final Parcelable.Creator<News> CREATOR = new Parcelable.Creator<News>() {
        public News createFromParcel(Parcel in) {
            return new News(in);
        }

        public News[] newArray(int size) {
            return new News[size];
        }
    };

    private News(Parcel in) {
        this.mTitle = in.readString();
        this.mDescription = in.readString();
        this.mFullText = in.readString();
        this.mLink = in.readString();
        this.mPicture = in.readString();
        this.mCategory = in.readString();
        DateFormat format = SimpleDateFormat.getDateInstance();
        try {
            this.mPubDate = format.parse(in.readString());
        }
        catch (ParseException e) {
            this.mPubDate = new Date();
        }
    }
}
