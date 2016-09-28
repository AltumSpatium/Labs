package smart.endlessnews;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.ArrayList;

class NewsAdapter extends BaseAdapter {
    private Context ctx;
    private ArrayList<News> allNews;
    private LayoutInflater lInflater;

    NewsAdapter(Context context, ArrayList<News> allNews) {
        this.ctx = context;
        this.allNews = allNews;
        lInflater = (LayoutInflater) ctx.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    @Override
    public int getCount() {
        return allNews.size();
    }

    @Override
    public News getItem(int position) {
        return allNews.get(position);
    }

    @Override
    public long getItemId(int position) {
        return position;
    }

    @Override
    public View getView(final int position, View convertView, ViewGroup parent) {
        View view = convertView != null ? convertView :
                lInflater.inflate(R.layout.news_item, parent, false);
        News news = getItem(position);

        ImageView ivPicture = (ImageView)view.findViewById(R.id.ivPicture);
        new DownloadImageTask(ivPicture).execute(news.getPicture());

        String title = news.getTitle();
        String description = news.getDescription();
        if (title.length() > 40) title = title.substring(0, 40) + "...";
        if (description.length() > 140) description = description.substring(0, 120) + "...";
        ((TextView) view.findViewById(R.id.tvTitle)).setText(title);
        ((TextView) view.findViewById(R.id.tvDescription)).setText(description);

        view.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Category currentCategory = new Category(getItem(position).getCategory(), allNews);
                Intent intent = new Intent(ctx, ArticleActivity.class);

                intent.putExtra("current_category", currentCategory);
                intent.putExtra("current_news", position);
                ctx.startActivity(intent);
            }
        });

        return view;
    }
}
