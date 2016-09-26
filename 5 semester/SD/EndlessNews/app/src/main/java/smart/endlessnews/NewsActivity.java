package smart.endlessnews;

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
        News news = new News("This is my first news's title!",
                "This news is goddamn useless! But I hope that someday I will finish RSS" +
                        " parser and news will be loaded automatically from news-websites!!!!!!!",
                "Full text is unavailable now.",
                "Link is unavailable now.",
                "http://scoopak.com/wp-content/uploads/2013/06/free-hd-natural-wallpapers-download-for-pc.jpg",
                category.getName(),
                new Date(100));
        category.addNews(news);

        newsAdapter = new NewsAdapter(this, category.getNews());

        lvNews = (ListView)findViewById(R.id.lvNews);

        if (lvNews != null)
            lvNews.setAdapter(newsAdapter);
    }
}
