package smart.endlessnews;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.util.Log;
import android.widget.ImageView;

import java.io.InputStream;

class DownloadImageTask extends AsyncTask<String, Void, Bitmap> {
    private ImageView imageView;
    private Bitmap downloadedImage;

    DownloadImageTask() {}

    DownloadImageTask(ImageView imageView) {
        this.imageView = imageView;
    }

    protected Bitmap doInBackground(String... urls) {
        String urlDisplay = urls[0];
        Bitmap bmp = null;
        try {
            InputStream in = new java.net.URL(urlDisplay).openStream();
            bmp = BitmapFactory.decodeStream(in);
        }
        catch (Exception e) {
            Log.e("Error", e.getMessage());
            e.printStackTrace();
        }
        return bmp;
    }

    protected void onPostExecute(Bitmap result) {
        if (imageView != null) imageView.setImageBitmap(result);
        downloadedImage = result;
    }

    public Bitmap getDownloadedImage() {
        return downloadedImage;
    }
}
