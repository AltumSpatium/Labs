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
    DBHelper(Context ctx) {
        super(ctx, "EndlessDB", null, 1);
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        // Table for storing full info about News
        db.execSQL(
                "create table NewsTable ("
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

        // Table for storing full info about Categories
        db.execSQL(
                "create table CategoryTable ("
                + "id integer primary key autoincrement,"
                + "name text" + ");"
        );
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS NewsTable");
        db.execSQL("DROP TABLE IF EXISTS CategoryTable");

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
