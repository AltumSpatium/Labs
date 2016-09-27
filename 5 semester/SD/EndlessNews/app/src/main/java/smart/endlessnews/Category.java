package smart.endlessnews;

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

    public void setNews(ArrayList<News> news) {
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
