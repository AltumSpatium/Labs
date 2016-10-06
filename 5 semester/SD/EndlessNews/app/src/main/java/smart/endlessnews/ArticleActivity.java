package smart.endlessnews;

import android.app.FragmentTransaction;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ScrollView;

public class ArticleActivity extends AppCompatActivity {
    private static final String TAG = "ArticleActivity";

    Category currentCategory;
    News currentNews;
    Button btnNextNews, btnPrevNews;
    ScrollView svMain;

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

        /*final ImageView ivFragmentPicture = (ImageView) findViewById(R.id.ivFragmentPicture);
        ivFragmentPicture.setOnTouchListener(new OnScaleTouchListener(ArticleActivity.this) {
            @Override
            void scaleDilute(float coeff) {
                int currWidth = ivFragmentPicture.getLayoutParams().width;
                int currHeight = ivFragmentPicture.getLayoutParams().height;
                ivFragmentPicture.setLayoutParams(new ActionBar.LayoutParams(
                        (int)(currWidth * coeff), (int)(currHeight * coeff)));
            }

            @Override
            void scalePinch(float coeff) {
                int currWidth = ivFragmentPicture.getLayoutParams().width;
                int currHeight = ivFragmentPicture.getLayoutParams().height;
                ivFragmentPicture.setLayoutParams(new ActionBar.LayoutParams(
                        (int)(currWidth * coeff), (int)(currHeight * coeff)));
            }
        });*/
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
