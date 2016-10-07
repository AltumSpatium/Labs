package smart.endlessnews;

import android.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.ScaleGestureDetector;
import android.view.View;
import android.widget.Button;
import android.widget.ScrollView;
import android.widget.TextView;

public class ArticleActivity extends AppCompatActivity {
    private static final String TAG = "ArticleActivity";

    Category currentCategory;
    News currentNews;
    Button btnNextNews, btnPrevNews;
    ScrollView svMain;
    TextView tvFullText;

    NewsFragment fragment;

    float scale = 1f;
    float startSize;
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
        fragment = NewsFragment.newInstance(currentNews);
        ft.add(R.id.frNews, fragment);
        ft.commit();

        btnPrevNews = (Button) findViewById(R.id.btnPrevNews);
        btnPrevNews.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!prevNews()) finish();
            }
        });

        btnNextNews = (Button) findViewById(R.id.btnNextNews);
        btnNextNews.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if (!nextNews()) finish();
            }
        });

        svMain = (ScrollView) findViewById(R.id.svMain);
        svMain.setOnTouchListener(new OnSwipeTouchListener(ArticleActivity.this) {
            public void onSwipeRight() {
                if (!prevNews()) finish();
            }

            public void onSwipeLeft() {
                if (!nextNews()) finish();
            }
        });
    }

    public boolean nextNews() {
        if (++newsID < currentCategory.getNews().size()) {
            FragmentTransaction ft = getFragmentManager().beginTransaction();
            currentNews = currentCategory.getNews(newsID);
            NewsFragment fragment = NewsFragment.newInstance(currentNews);
            ft.replace(R.id.frNews, fragment);
            ft.commit();
            return true;
        }
        return false;
    }

    public boolean prevNews() {
        if (--newsID >= 0) {
            FragmentTransaction ft = getFragmentManager().beginTransaction();
            currentNews = currentCategory.getNews(newsID);
            NewsFragment fragment = NewsFragment.newInstance(currentNews);
            ft.replace(R.id.frNews, fragment);
            ft.commit();
            return true;
        }
        return false;
    }

    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG, "ArticleActivity OnPause");
        MyApplication myApp = (MyApplication) this.getApplication();
        myApp.setLastNewsTitle(currentNews.getTitle());
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "ArticleActivity OnResume");
        tvFullText = (TextView) fragment.getView().findViewById(R.id.tvFragmentFullText);
        startSize = tvFullText.getTextSize();

        tvFullText.setOnTouchListener(new OnScaleTouchListener(ArticleActivity.this) {
            @Override
            void resize(ScaleGestureDetector detector) {
                scale *= detector.getScaleFactor();
                scale = Math.max(0.5f, Math.min(scale, 1.2f));

                tvFullText.setTextSize(startSize * scale);
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();
        Log.d(TAG, "ArticleActivity OnStart");
    }

    @Override
    protected void onStop() {
        super.onStop();
        Log.d(TAG, "ArticleActivity OnStop");
    }

    @Override
    protected void onRestart() {
        super.onRestart();
        Log.d(TAG, "ArticleActivity OnRestart");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Log.d(TAG, "ArticleActivity OnDestroy");
    }
}
