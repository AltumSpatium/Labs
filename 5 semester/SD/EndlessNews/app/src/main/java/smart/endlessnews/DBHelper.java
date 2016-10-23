package smart.endlessnews;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

class DBHelper extends SQLiteOpenHelper {
    private boolean isNews;

    DBHelper(Context ctx) {
        super(ctx, "EndlessDB", null, 1);
        isNews = ctx.getClass() == MainActivity.class || ctx.getClass() == NewsActivity.class;
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        if (isNews) {
            // Table for storing full info about News
            NewsRepository newsRepository = new NewsRepository(null);
            newsRepository.connect(db);
            newsRepository.create();

            // Table for storing full info about Categories
            CategoryRepository categoryRepository = new CategoryRepository();
            categoryRepository.connect(db);
            categoryRepository.create();
        } else {
            // Table for storing full info about Tracks on device
            TrackRepository trackRepository = new TrackRepository();
            trackRepository.connect(db);
            trackRepository.create();
        }
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        if (isNews) {
            db.execSQL("DROP TABLE IF EXISTS NewsTable");
            db.execSQL("DROP TABLE IF EXISTS CategoryTable");
        } else {
            db.execSQL("DROP TABLE IF EXISTS TrackTable");
        }

        onCreate(db);
    }

    ArrayList<News> innerJoin(String orderBy) {
        ArrayList<News> news = new ArrayList<>();
        SQLiteDatabase db = this.getWritableDatabase();

        Cursor c = db.rawQuery(
                "select NW.title, NW.description, NW.fulltext, NW.link, NW.picture, NW.pubdate, CT.name as Name "
                + "from NewsTable as NW "
                + "inner join CategoryTable as CT "
                + "on NW.cat_id = CT.id "
                + orderBy,
                new String[] {}
        );

        if (c != null) {
            if (c.moveToFirst()) {
                int titleColIndex = c.getColumnIndex("title");
                int descriptionColIndex = c.getColumnIndex("description");
                int fulltextColIndex = c.getColumnIndex("fulltext");
                int linkColIndex = c.getColumnIndex("link");
                int pictureColIndex = c.getColumnIndex("picture");
                int pubdateColIndex = c.getColumnIndex("pubdate");
                int nameColIndex = c.getColumnIndex("Name");

                do {
                    String title = c.getString(titleColIndex);
                    String description = c.getString(descriptionColIndex);
                    String fulltext = c.getString(fulltextColIndex);
                    String link = c.getString(linkColIndex);
                    String picture = c.getString(pictureColIndex);
                    Date pubdate;
                    DateFormat format = SimpleDateFormat.getDateInstance();
                    try {
                        pubdate = format.parse(c.getString(pubdateColIndex));
                    }
                    catch (ParseException e) {
                        pubdate = new Date();
                    }
                    String name = c.getString(nameColIndex);

                    news.add(new News(title, description, fulltext, link, picture, name, pubdate));
                } while (c.moveToNext());
            }

            c.close();
        }

        return news;
    }
}
