package smart.endlessnews;

import android.os.PersistableBundle;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ListView;
import android.widget.Toast;

import java.util.Date;

public class NewsActivity extends AppCompatActivity {

    Category category;
    NewsAdapter newsAdapter;

    ListView lvNews;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_news);

        category = getIntent().getParcelableExtra("current_category");
        newsAdapter = new NewsAdapter(this, category.getNews());
        lvNews = (ListView)findViewById(R.id.lvNews);

        if (lvNews != null)
            lvNews.setAdapter(newsAdapter);
    }
}
