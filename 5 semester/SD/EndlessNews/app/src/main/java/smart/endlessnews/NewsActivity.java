package smart.endlessnews;

import android.content.Context;
import android.content.DialogInterface;
import android.database.sqlite.SQLiteDatabase;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.Random;

public class NewsActivity extends AppCompatActivity {
    static {
        System.loadLibrary("sortNDK");
    }

    private static final String TAG = "NewsActivity";

    final int MENU_ALL_ID = 1;
    final int MENU_SORTBYTITLE_ID = 2;
    final int MENU_SORTBYCATEGORY_ID = 3;
    final int MENU_SORTBYWORD_ID = 4;
    final int MENU_SORTBYWORD_NDK_ID = 5;

    Category category;
    ArrayList<News> allnews;
    NewsAdapter newsAdapter;

    ListView lvNews;

    DBHelper dbHelper;

    Category orderedByTitleCategory;
    Category orderedByNameCategory;

    public native int[] sortNDK(int[] news, int[] freqs, int size);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "NewsActivity OnCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_news);
        if (getSupportActionBar() != null)
            getSupportActionBar().hide();

        category = getIntent().getParcelableExtra("current_category");
        newsAdapter = new NewsAdapter(this, category);
        lvNews = (ListView) findViewById(R.id.lvNews);

        if (lvNews != null)
            lvNews.setAdapter(newsAdapter);

        dbHelper = new DBHelper(this);
        SQLiteDatabase db = dbHelper.getWritableDatabase();

        orderedByTitleCategory = new Category("AllNews", dbHelper.innerJoin("order by NW.title", db));
        orderedByNameCategory = new Category("AllNews", dbHelper.innerJoin("order by CT.name", db));
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        if (category.getName().equals("AllNews")) {
            menu.add(0, MENU_ALL_ID, 0, "All news");
            menu.add(0, MENU_SORTBYTITLE_ID, 0, "Sort by title");
            menu.add(0, MENU_SORTBYCATEGORY_ID, 0, "Sort by category");
            menu.add(0, MENU_SORTBYWORD_ID, 0, "Sort by word (Java)");
            menu.add(0, MENU_SORTBYWORD_NDK_ID, 0, "Sort by word (NDK)");
        }
        return super.onCreateOptionsMenu(menu);
    }

    public int getWordFreq(String word, String text) {
        word = word.toLowerCase();
        text = text.toLowerCase();

        int count = 0, i = text.indexOf(word);
        while (i >= 0) {
            count++;
            i = text.indexOf(word, i + 1);
        }

        return count;
    }

    public HashMap<News, Integer> mapWordFrequency(String word) {
        HashMap<News, Integer> wordFreq = new HashMap<>();

        if (word.equals("_test")) {
            for (int i = 0; i < 10000; i++)
                wordFreq.put(new News("Test " + i, "a", "a", "a", "a", "TestCategory", new Date()),
                        new Random().nextInt(10000));
        }
        else {
            for (News n : category.getNews())
                wordFreq.put(n, getWordFreq(word, n.getFullText()));
        }

        return wordFreq;
    }

    public int[] sortJava(int[] news, int[] freqs, int size) {
        for (int i = size - 1; i >= 0; i--) {
            for (int j = 0; j < i; j++) {
                if (freqs[j] < freqs[j+1]) {
                    int tmp = freqs[j];
                    freqs[j] = freqs[j+1];
                    freqs[j+1] = tmp;

                    tmp = news[j];
                    news[j] = news[j+1];
                    news[j+1] = tmp;
                }
            }
        }

        return news;
    }

    public ArrayList<News> sortJavaStart(HashMap<News, Integer> mappedFreq) {
        ArrayList<News> sortedNews = new ArrayList<>();
        ArrayList<News> newsList = new ArrayList<>();
        Collection<Integer> freqsList = mappedFreq.values();

        for (News n : mappedFreq.keySet())
            newsList.add(n);

        int[] freqs = new int[freqsList.size()], news = new int[freqsList.size()];
        int index = 0;

        for (Integer i : freqsList) {
            news[index] = index;
            freqs[index++] = i;
        }

        long time = System.currentTimeMillis();
        news = sortJava(news, freqs, freqsList.size());
        time = System.currentTimeMillis() - time;

        Toast.makeText(getApplicationContext(),
                "Worked for " + time + "ms",
                Toast.LENGTH_SHORT).show();

        for (Integer i : news)
            sortedNews.add(newsList.get(i));

        return sortedNews;
    }

    public ArrayList<News> sortNDKStart(HashMap<News, Integer> mappedFreq) {
        ArrayList<News> sortedNews = new ArrayList<>();
        ArrayList<News> newsList = new ArrayList<>();
        Collection<Integer> freqsList = mappedFreq.values();

        for (News n : mappedFreq.keySet())
            newsList.add(n);

        int[] freqs = new int[freqsList.size()], news = new int[freqsList.size()];
        int index = 0;

        for (Integer i : freqsList) {
            news[index] = index;
            freqs[index++] = i;
        }

        long time = System.currentTimeMillis();
        news = sortNDK(news, freqs, freqsList.size());
        time = System.currentTimeMillis() - time;

        Toast.makeText(getApplicationContext(),
                "Worked for " + time + "ms",
                Toast.LENGTH_SHORT).show();

        for (Integer i : news)
            sortedNews.add(newsList.get(i));

        return sortedNews;
    }

    public ArrayList<News> prepareSort(String word, boolean isNDK) {
        HashMap<News, Integer> mappedFreq = mapWordFrequency(word);

        if (isNDK) return sortNDKStart(mappedFreq);
        else return sortJavaStart(mappedFreq);
    }

    public void sortByWordAppFreq(final boolean isNDK) {
        final Context ctx = this;

        AlertDialog.Builder alert = new AlertDialog.Builder(this);
        alert.setTitle("Sort by frequency");
        alert.setMessage("Enter sorting word:");

        final EditText input = new EditText(this);
        alert.setView(input);

        alert.setPositiveButton("Sort", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int btnID) {
                String word = input.getText().toString();
                if (word.isEmpty()) {
                    Toast.makeText(getApplicationContext(),
                            "Word can't be empty!",
                            Toast.LENGTH_SHORT).show();
                    return;
                }
                allnews = prepareSort(word, isNDK);

                newsAdapter = new NewsAdapter(ctx, new Category("AllNews", allnews));
                if (lvNews != null)
                    lvNews.setAdapter(newsAdapter);
            }
        });

        alert.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int btnID) {
            }
        });

        alert.show();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case MENU_ALL_ID:
                newsAdapter = new NewsAdapter(this, category);
                if (lvNews != null)
                    lvNews.setAdapter(newsAdapter);
                break;
            case MENU_SORTBYTITLE_ID:
                newsAdapter = new NewsAdapter(this, orderedByTitleCategory);
                if (lvNews != null)
                    lvNews.setAdapter(newsAdapter);
                break;
            case MENU_SORTBYCATEGORY_ID:
                newsAdapter = new NewsAdapter(this, orderedByNameCategory);
                if (lvNews != null)
                    lvNews.setAdapter(newsAdapter);
                break;
            case MENU_SORTBYWORD_ID:
                sortByWordAppFreq(false);
                break;
            case MENU_SORTBYWORD_NDK_ID:
                sortByWordAppFreq(true);
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG, "NewsActivity OnPause");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "NewsActivity OnResume");
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.d(TAG, "NewsActivity OnStart");
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.d(TAG, "NewsActivity OnStop");
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        Log.d(TAG, "NewsActivity OnRestart");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "NewsActivity OnDestroy");
    }
}
