package smart.endlessnews;

import android.app.FragmentTransaction;
import android.support.v4.app.Fragment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

public class ArticleActivity extends AppCompatActivity {

    Category currentCategory;
    News currentNews;
    Button btnNextNews, btnPrevNews;

    int newsID;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_article);

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
}
