package smart.endlessnews;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.ListView;

public class NewsActivity extends AppCompatActivity {
    private static final String TAG = "NewsActivity";

    final int MENU_ALL_ID = 1;
    final int MENU_SORTBYTITLE_ID = 2;
    final int MENU_SORTBYCATEGORY_ID = 3;

    Category category;
    NewsAdapter newsAdapter;

    ListView lvNews;

    DBHelper dbHelper;

    Category orderedByTitleCategory;
    Category orderedByNameCategory;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "NewsActivity OnCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_news);
        if (getSupportActionBar() != null)
            getSupportActionBar().hide();

        category = getIntent().getParcelableExtra("current_category");
        if (category == null)
            category = getIntent().getParcelableExtra("all_news");
        newsAdapter = new NewsAdapter(this, category);
        lvNews = (ListView)findViewById(R.id.lvNews);

        if (lvNews != null)
            lvNews.setAdapter(newsAdapter);

        dbHelper = new DBHelper(this);

        orderedByTitleCategory = new Category("AllNews", dbHelper.innerJoin("order by NW.title"));
        orderedByNameCategory = new Category("AllNews", dbHelper.innerJoin("order by CT.name"));
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        if (category.getName().equals("AllNews")) {
            menu.add(0, MENU_ALL_ID, 0, "All news");
            menu.add(0, MENU_SORTBYTITLE_ID, 0, "Sort by title");
            menu.add(0, MENU_SORTBYCATEGORY_ID, 0, "Sort by category");
        }
        return super.onCreateOptionsMenu(menu);
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
