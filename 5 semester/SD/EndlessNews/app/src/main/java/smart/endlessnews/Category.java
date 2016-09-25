package smart.endlessnews;

import android.os.Parcel;
import android.os.Parcelable;
import java.util.ArrayList;

public class Category implements Parcelable {
    private String mName;
    private boolean mDeleteState = false;
    private ArrayList<News> mNews = new ArrayList<>();

    public Category(String name) {
        this.mName = name;
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

    public boolean getDeleteState() {
        return mDeleteState;
    }

    public void setDeleteState(boolean deleteState) {
        mDeleteState = deleteState;
    }

    public void addNews(News news) {
        mNews.add(news);
    }

    public ArrayList<News> getNews() {
        return mNews;
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
