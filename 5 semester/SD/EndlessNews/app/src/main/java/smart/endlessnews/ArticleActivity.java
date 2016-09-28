package smart.endlessnews;

import android.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class ArticleActivity extends AppCompatActivity {
    private static final String TAG = "ArticleActivity";

    Category currentCategory;
    News currentNews;
    Button btnNextNews, btnPrevNews;

    int newsID;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "ArticleActivity OnCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_article);
        if (getSupportActionBar() != null)
            getSupportActionBar().hide();

        currentCategory = getIntent().getParcelableExtra("current_category");
        newsID = getIntent().getIntExtra("current_news", newsID);
        currentNews = currentCategory.getNews(newsID);

        FragmentTransaction ft = getFragmentManager().beginTransaction();
        NewsFragment fragment = NewsFragment.newInstance(currentNews);
        ft.add(R.id.frNews, fragment);
        ft.commit();

        btnPrevNews = (Button) findViewById(R.id.btnPrevNews);
        btnPrevNews.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (--newsID >= 0) {
                    FragmentTransaction ft = getFragmentManager().beginTransaction();
                    currentNews = currentCategory.getNews(newsID);
                    NewsFragment fragment = NewsFragment.newInstance(currentNews);
                    ft.replace(R.id.frNews, fragment);
                    ft.commit();
                }
                else finish();
            }
        });

        btnNextNews = (Button) findViewById(R.id.btnNextNews);
        btnNextNews.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (++newsID < currentCategory.getNews().size()) {
                    FragmentTransaction ft = getFragmentManager().beginTransaction();
                    currentNews = currentCategory.getNews(newsID);
                    NewsFragment fragment = NewsFragment.newInstance(currentNews);
                    ft.replace(R.id.frNews, fragment);
                    ft.commit();
                }
                else finish();
            }
        });
    }

    protected void onPause() {
        Log.d(TAG, "ArticleActivity OnPause");
        super.onPause();
        MyApplication myApp = (MyApplication) this.getApplication();
        myApp.setLastNewsTitle(currentNews.getTitle());
    }
}
