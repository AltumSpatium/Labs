package smart.endlessnews;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.Toast;
import java.util.ArrayList;

class CategoryAdapter extends BaseAdapter {
    private Context ctx;
    private ArrayList<Category> categories;
    private LayoutInflater lInflater;

    CategoryAdapter(Context context, ArrayList<Category> categories) {
        this.ctx = context;
        this.categories = categories;
        lInflater = (LayoutInflater) ctx.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    @Override
    public int getCount() {
        return categories.size();
    }

    @Override
    public Category getItem(int position) {
        return categories.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(final int position, View convertView, ViewGroup parent) {
        View view = convertView != null ? convertView :
                lInflater.inflate(R.layout.category_item, parent, false);
        Category category = getItem(position);

        ((TextView) view.findViewById(R.id.tvCategoryName)).setText(category.getName());
        CheckBox cbDelete = (CheckBox) view.findViewById(R.id.cbDelete);
        cbDelete.setTag(position);
        cbDelete.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                getItem((int)buttonView.getTag()).setDeleteState(isChecked);
            }
        });

        view.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Category category = getItem(position);
                Intent intent = new Intent(ctx, NewsActivity.class);

                intent.putExtra("current_category", category);
                ctx.startActivity(intent);
            }
        });

        return view;
    }

    ArrayList<Category> addCategory(String name, ArrayList<News> allNews) {
        for (Category c : categories)
            if (c.getName().equals(name)) {
                Toast.makeText(lInflater.getContext(), "Such category already exists!",
                        Toast.LENGTH_SHORT).show();
                return categories;
            }

        Category newCategory = new Category(name);
        for (News n : allNews) {
            if (n.getCategory().equals(name))
                newCategory.addNews(n);
        }

        categories.add(newCategory);
        return categories;
    }

    ArrayList<Category> deleteCategory() {
        ArrayList<Category> finalCategories = new ArrayList<>();
        for (Category c : categories)
            if (!c.getDeleteState())
                finalCategories.add(c);
        categories = finalCategories;
        return categories;
    }
}
