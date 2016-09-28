package smart.endlessnews;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.ListView;

public class NewsActivity extends AppCompatActivity {
    private static final String TAG = "NewsActivity";

    Category category;
    NewsAdapter newsAdapter;

    ListView lvNews;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "NewsActivity OnCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_news);
        if (getSupportActionBar() != null)
            getSupportActionBar().hide();

        category = getIntent().getParcelableExtra("current_category");
        newsAdapter = new NewsAdapter(this, category.getNews());
        lvNews = (ListView)findViewById(R.id.lvNews);

        if (lvNews != null)
            lvNews.setAdapter(newsAdapter);
    }
}
