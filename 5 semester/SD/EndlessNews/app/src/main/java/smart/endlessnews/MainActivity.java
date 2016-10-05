package smart.endlessnews;

import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.concurrent.TimeUnit;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    final int MENU_ADD_ID = 1;
    final int MENU_DELETE_ID = 2;
    final int MENU_AUDIO_ID = 3;
    final int MENU_QUIT_ID = 4;

    String RSS_URL = "http://www.vesti.ru/vesti.rss";
    String lastNewsTitle;
    boolean userCategories = false;

    private SharedPreferences sharedPreferences;

    ArrayList<Category> categories = new ArrayList<>();
    ArrayList<News> allNews = new ArrayList<>();
    CategoryAdapter categoryAdapter;

    ListView lvMain;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "MainActivity OnCreate");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        sharedPreferences = getSharedPreferences("prefs", Context.MODE_PRIVATE);

        if (savedInstanceState != null) {
            userCategories = savedInstanceState.getBoolean("userCategories");
            RSS_URL = savedInstanceState.getString("rssurl");
            int categoriesCount = savedInstanceState.getInt("categories_count");
            int newsCount = savedInstanceState.getInt("news_count");
            for (int i = 0; i < categoriesCount; i++)
                categories.add((Category) savedInstanceState.getParcelable("category" + i));
            for (int i = 0; i < newsCount; i++)
                allNews.add((News) savedInstanceState.getParcelable("news" + i));
        } else {
            userCategories = sharedPreferences.getBoolean("userCategories", false);
            RSS_URL = sharedPreferences.getString("rssurl", RSS_URL);
            int size = sharedPreferences.getInt("categoriesCount", 0);
            if (size > 0 && categories.isEmpty())
                for (int i = 0; i < size; i++) {
                    String name = sharedPreferences.getString("categoryName" + i, "");
                    categories.add(new Category(name));
                }

            RSSParser rssParser = new RSSParser(RSS_URL);
            rssParser.execute();
            try {
                allNews = rssParser.get(3, TimeUnit.SECONDS);
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }

        if (categories.isEmpty() && !userCategories) {
            categories = createCategories(allNews);
            userCategories = true;
        }
        else categories = fillCategories(allNews, categories);

        categoryAdapter = new CategoryAdapter(this, categories);

        lvMain = (ListView) findViewById(R.id.lvMain);

        if (lvMain != null)
            lvMain.setAdapter(categoryAdapter);
    }

    protected void onSaveInstanceState(Bundle outState) {
        Log.d(TAG, "MainActivity OnSaveInstanceState");
        super.onSaveInstanceState(outState);
        outState.putBoolean("userCategories", userCategories);
        outState.putString("rssurl", RSS_URL);
        outState.putInt("categories_count", categories.size());
        outState.putInt("news_count", allNews.size());
        for (int i = 0; i < categories.size(); i++)
            outState.putParcelable("category" + i, categories.get(i));
        for (int i = 0; i < allNews.size(); i++)
            outState.putParcelable("news" + i, allNews.get(i));
    }

    protected void onPause() {
        Log.d(TAG, "MainActivity OnPause");
        super.onPause();
        SharedPreferences.Editor editor = sharedPreferences.edit();
        editor.putBoolean("userCategories", userCategories);
        editor.putString("rssurl", RSS_URL);
        int size = categories.size();
        editor.putInt("categoriesCount", size);
        for (int i = 0; i < size; i++)
            editor.putString("categoryName" + i, categories.get(i).getName());
        editor.apply();
    }

    protected void onResume() {
        Log.d(TAG, "MainActivity OnResume");
        super.onResume();
        MyApplication myApp = (MyApplication) this.getApplication();
        lastNewsTitle = myApp.getLastNewsTitle();
        if (lastNewsTitle != null) {
            String text = "Last viewed news: " + lastNewsTitle;
            ((TextView) findViewById(R.id.tvLastNews)).setText(text);
        }
    }

    private HashMap<String, ArrayList<News>> fillHashMap(ArrayList<News> news) {
        HashMap<String, ArrayList<News>> hashMap = new HashMap<>();

        for (News n : news) {
            if (hashMap.get(n.getCategory()) == null)
                hashMap.put(n.getCategory(), new ArrayList<News>());
            hashMap.get(n.getCategory()).add(n);
        }
        return hashMap;
    }

    public ArrayList<Category> createCategories(ArrayList<News> news) {
        ArrayList<Category> newCategories = new ArrayList<>();
        HashMap<String, ArrayList<News>> hashMap = fillHashMap(news);

        for (String name : hashMap.keySet())
            newCategories.add(new Category(
                    name,
                    hashMap.get(name)));

        return newCategories;
    }

    public ArrayList<Category> fillCategories(ArrayList<News> news, ArrayList<Category> catToFill) {
        HashMap<String, ArrayList<News>> hashMap = fillHashMap(news);

        for (int i = 0; i < catToFill.size(); i++) {
            Category category = catToFill.get(i);
            if (hashMap.get(category.getName()) != null)
                catToFill.get(i).setNews(hashMap.get(category.getName()));
        }

        return catToFill;
    }

    public boolean isNeededDeletion() {
        for (Category c : categories)
            if (c.getDeleteState()) return true;
        return false;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        menu.add(0, MENU_ADD_ID, 0, "Add new category");
        menu.add(0, MENU_DELETE_ID, 0, "Delete selected categories");
        menu.add(0, MENU_AUDIO_ID, 0, "Audio Player");
        menu.add(0, MENU_QUIT_ID, 0, "Quit");
        return super.onCreateOptionsMenu(menu);
    }

    public void addNewCategory() {
        AlertDialog.Builder alert = new AlertDialog.Builder(this);
        alert.setTitle("New category");
        alert.setMessage("Enter new category name:");

        final EditText input = new EditText(this);
        alert.setView(input);

        alert.setPositiveButton("Add", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int btnID) {
                String value = input.getText().toString();
                if (value.isEmpty()) {
                    Toast.makeText(getApplicationContext(),
                            "Name can't be empty!",
                            Toast.LENGTH_SHORT).show();
                    return;
                }
                categories = categoryAdapter.addCategory(value, allNews);
                categoryAdapter.notifyDataSetChanged();
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
            case MENU_ADD_ID:
                addNewCategory();
                break;
            case MENU_DELETE_ID:
                if (isNeededDeletion()) {
                    categories = categoryAdapter.deleteCategory();
                    categoryAdapter.notifyDataSetChanged();
                }
                if (categories.isEmpty()) userCategories = false;
                break;
            case MENU_AUDIO_ID:
                Intent intent = new Intent(MainActivity.this, AudioPlayerActivity.class);
                startActivity(intent);
                break;
            case MENU_QUIT_ID:
                finish();
                break;
        }
        return super.onOptionsItemSelected(item);
    }
}
