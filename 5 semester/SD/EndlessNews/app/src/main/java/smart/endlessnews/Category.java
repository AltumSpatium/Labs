package smart.endlessnews;

import android.content.ContentValues;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.os.Parcel;
import android.os.Parcelable;
import java.util.ArrayList;

class Category implements Parcelable {
    private String mName;
    private boolean mDeleteState = false;
    private ArrayList<News> mNews = new ArrayList<>();

    Category(String name) {
        this.mName = name;
    }

    Category(String name, ArrayList<News> news) {
        this.mName = name;
        this.mNews = news;
    }

    private Category(Parcel in) {
        this.mName = in.readString();
        int size = in.readInt();
        for (int i = 0; i < size; i++)
            mNews.add((News) in.readParcelable(News.class.getClassLoader()));
    }

    public String getName() {
        return mName;
    }

    public void setName(String name) {
        mName = name;
    }

    boolean getDeleteState() {
        return mDeleteState;
    }

    void setDeleteState(boolean deleteState) {
        mDeleteState = deleteState;
    }

    void addNews(News news) {
        mNews.add(news);
    }

    News getNews(int position) {
        return mNews.get(position);
    }

    ArrayList<News> getNews() {
        return mNews;
    }

    void setNews(ArrayList<News> news) {
        this.mNews = news;
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel parcel, int flags)
    {
        parcel.writeString(mName);
        parcel.writeInt(mNews.size());
        for (News n : mNews)
            parcel.writeParcelable(n, 0);
    }

    public static final Parcelable.Creator<Category> CREATOR = new Parcelable.Creator<Category>() {
        public Category createFromParcel(Parcel in) {
            return new Category(in);
        }

        public Category[] newArray(int size) {
            return new Category[size];
        }
    };
}

class CategoryRepository {
    private SQLiteDatabase db;

    void connect(SQLiteDatabase db) {
        this.db = db;
    }

    void create() {
        db.execSQL(
                "create table if not exists CategoryTable ("
                        + "id integer primary key autoincrement,"
                        + "name text" + ");"
        );
    }

    void add(Category category) {
        ContentValues cv = new ContentValues();
        cv.put("name", category.getName());
        db.insert("CategoryTable", null, cv);
    }

    ArrayList<Category> loadAll() {
        ArrayList<Category> categories = new ArrayList<>();
        Cursor c = db.query("CategoryTable", null, null, null, null, null, null);

        if (c != null) {
            if (c.moveToFirst()) {
                int nameColIndex = c.getColumnIndex("name");

                do {
                    String name = c.getString(nameColIndex);
                    categories.add(new Category(name));
                } while (c.moveToNext());
            }

            c.close();
        }

        return categories;
    }

    void saveAll(ArrayList<Category> categories) {
        for (Category c : categories)
            add(c);
    }
}
