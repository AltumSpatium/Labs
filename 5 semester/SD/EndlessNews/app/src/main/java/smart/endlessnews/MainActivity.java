package smart.endlessnews;

import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.Toast;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    final int MENU_ADD_ID = 1;
    final int MENU_DELETE_ID = 2;
    final int MENU_QUIT_ID = 3;

    ArrayList<Category> categories = new ArrayList<>();
    ArrayList<News> allNews = new ArrayList<>();
    CategoryAdapter categoryAdapter;

    ListView lvMain;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        if (savedInstanceState == null) addTestCategories();

        categoryAdapter = new CategoryAdapter(this, categories);

        lvMain = (ListView) findViewById(R.id.lvMain);

        if (lvMain != null)
            lvMain.setAdapter(categoryAdapter);
    }

    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        outState.putInt("categories_count", categories.size());
        outState.putInt("news_count", allNews.size());
        for (int i = 0; i < categories.size(); i++)
            outState.putParcelable("category" + i, categories.get(i));
        for (int i = 0; i < allNews.size(); i++)
            outState.putParcelable("news" + i, allNews.get(i));
    }

    protected void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        int categoriesCount = savedInstanceState.getInt("categories_count");
        int newsCount = savedInstanceState.getInt("news_count");
        for (int i = 0; i < categoriesCount; i++)
            categories.add((Category) savedInstanceState.getParcelable("category" + i));
        for (int i = 0; i < newsCount; i++)
            allNews.add((News) savedInstanceState.getParcelable("news" + i));
    }

    public void addTestCategories() {
        for (int i = 0; i < 20; i++)
            categories.add(new Category("Category " + (i + 1)));
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
                lvMain.setAdapter(categoryAdapter);
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
                    lvMain.setAdapter(categoryAdapter);
                }
                break;
            case MENU_QUIT_ID:
                finish();
                break;
        }
        return super.onOptionsItemSelected(item);
    }
}
