package smart.endlessnews;

import android.app.Application;

public class MyApplication extends Application {
    private String lastNewsTitle;

    public String getLastNewsTitle() {
        return this.lastNewsTitle;
    }

    public void setLastNewsTitle(String lastNewsTitle) {
        this.lastNewsTitle = lastNewsTitle;
    }
}
