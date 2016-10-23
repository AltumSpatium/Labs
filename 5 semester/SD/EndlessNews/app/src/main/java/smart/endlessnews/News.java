package smart.endlessnews;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Parcel;
import android.os.Parcelable;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

class News implements Parcelable {
    private String mTitle;
    private String mDescription;
    private String mFullText;
    private String mLink;
    private String mPicture;
    private String mCategory;
    private Date mPubDate;

    News(String title, String description, String fullText,
                String link, String picture, String category, Date pubDate) {
        this.mTitle = title;
        this.mDescription = description;
        this.mFullText = fullText;
        this.mLink = link;
        this.mPicture = picture;
        this.mCategory = category;
        this.mPubDate = pubDate;
    }

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

    String getTitle() {
        return mTitle;
    }

    String getDescription() {
        return mDescription;
    }

    String getFullText() {
        return mFullText;
    }

    String getLink() {
        return mLink;
    }

    String getPicture() {
        return mPicture;
    }

    String getCategory() {
        return mCategory;
    }

    Date getPubDate() {
        return mPubDate;
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
}

class NewsRepository {
    private SQLiteDatabase db;
    private ArrayList<Category> categories;

    NewsRepository(ArrayList<Category> categories) {
        this.categories = categories;
    }

    private int getId(String category) {
        for (int i = 0; i < categories.size(); i++) {
            if (categories.get(i).getName().equals(category))
                return i + 1;
        }

        return 1;
    }

    void connect(SQLiteDatabase db) {
        this.db = db;
    }

    void create() {
        db.execSQL(
                "create table if not exists NewsTable ("
                        + "id integer primary key autoincrement,"
                        + "title text,"
                        + "description text,"
                        + "fulltext text,"
                        + "link text,"
                        + "picture text,"
                        + "category text,"
                        + "pubdate text,"
                        + "cat_id integer" + ");"
        );
    }

    void add(News news) {
        ContentValues cv = new ContentValues();

        cv.put("title", news.getTitle());
        cv.put("description", news.getDescription());
        cv.put("fulltext", news.getFullText());
        cv.put("link", news.getLink());
        cv.put("picture", news.getPicture());
        cv.put("category", news.getCategory());
        cv.put("pubdate", news.getPubDate().toString());
        cv.put("cat_id", getId(news.getCategory()));

        db.insert("NewsTable", null, cv);
    }

    ArrayList<News> loadAll() {
        ArrayList<News> news = new ArrayList<>();
        Cursor c = db.query("NewsTable", null, null, null, null, null, null);

        if (c != null) {
            if (c.moveToFirst()) {
                int titleColIndex = c.getColumnIndex("title");
                int descriptionColIndex = c.getColumnIndex("description");
                int fulltextColIndex = c.getColumnIndex("fulltext");
                int linkColIndex = c.getColumnIndex("link");
                int pictureColIndex = c.getColumnIndex("picture");
                int categoryColIndex = c.getColumnIndex("category");
                int pubdateColIndex = c.getColumnIndex("pubdate");

                do {
                    String title = c.getString(titleColIndex);
                    String description = c.getString(descriptionColIndex);
                    String fulltext = c.getString(fulltextColIndex);
                    String link = c.getString(linkColIndex);
                    String picture = c.getString(pictureColIndex);
                    String category = c.getString(categoryColIndex);
                    Date pubdate;
                    DateFormat format = SimpleDateFormat.getDateInstance();
                    try {
                        pubdate = format.parse(c.getString(pubdateColIndex));
                    } catch (ParseException e) {
                        pubdate = new Date();
                    }

                    news.add(new News(title, description, fulltext, link, picture, category, pubdate));
                } while (c.moveToNext());
            }

            c.close();
        }

        return news;
    }
}
