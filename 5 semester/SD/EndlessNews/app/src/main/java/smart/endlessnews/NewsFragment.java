package smart.endlessnews;

import android.app.Fragment;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

public class NewsFragment extends Fragment {
    public View onCreateView(LayoutInflater lInflater, ViewGroup container, Bundle savedInstanceState) {
        View v = lInflater.inflate(R.layout.news_fragment, null);

        return v;
    }
}
