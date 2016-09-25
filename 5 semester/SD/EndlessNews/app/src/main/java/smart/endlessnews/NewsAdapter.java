package smart.endlessnews;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;
import java.util.ArrayList;

public class NewsAdapter extends BaseAdapter {
    private Context ctx;
    private ArrayList<News> news;
    private LayoutInflater lInflater;

    public NewsAdapter(Context context, ArrayList<News> news) {
        this.ctx = context;
        this.news = news;
        lInflater = (LayoutInflater) ctx.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    }

    @Override
    public int getCount() {
        return news.size();
    }

    @Override
    public News getItem(int position) {
        return news.get(position);
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

        String description = news.getDescription();
        if (description.length() > 120) description = description.substring(0, 120) + "...";
        ((TextView) view.findViewById(R.id.tvTitle)).setText(news.getTitle());
        ((TextView) view.findViewById(R.id.tvDescription)).setText(description);

        ImageView ivPicture = (ImageView)view.findViewById(R.id.ivPicture);
        new DownloadImageTask(ivPicture).execute(news.getPicture());

        view.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
            }
        });

        return view;
    }
}
