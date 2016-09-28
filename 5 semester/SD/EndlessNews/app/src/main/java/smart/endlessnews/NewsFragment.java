package smart.endlessnews;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

public class NewsFragment extends Fragment {

    public static NewsFragment newInstance(News news) {
        NewsFragment newsFragment = new NewsFragment();
        Bundle args = new Bundle();
        args.putParcelable("news", news);
        newsFragment.setArguments(args);
        return newsFragment;
    }

    @Override
    public View onCreateView(LayoutInflater lInflater, ViewGroup container, Bundle savedInstanceState) {
        News news = getArguments().getParcelable("news");
        View v = lInflater.inflate(R.layout.news_fragment, container, false);
        if (news != null) {
            new DownloadImageTask((ImageView) v.findViewById(R.id.ivFragmentPicture)).
                    execute(news.getPicture());
            ((TextView) v.findViewById(R.id.tvFragmentTitle)).
                    setText(news.getTitle());
            ((TextView) v.findViewById(R.id.tvFragmentFullText)).
                    setText(news.getFullText());
            ((TextView) v.findViewById(R.id.tvFragmentLink)).
                    setText(news.getLink());
            ((TextView) v.findViewById(R.id.tvFragmentPubDate)).
                    setText(news.getPubDate().toString());
        }

        return v;
    }
}
