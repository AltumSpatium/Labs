package smart.endlessnews;

import android.provider.DocumentsContract;
import android.util.Log;
import android.widget.Toast;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.Date;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

class RSSParser {
    private String rssURL;
    private static final String TAG = "MyApp";

    RSSParser(String rssURL) {
        this.rssURL = rssURL;
    }

    public void setRssURL(String rssURL) {
        this.rssURL = rssURL;
    }

    ArrayList<News> parseFeed() {
        ArrayList<News> news = new ArrayList<>();

        Log.d(TAG, "START");

        try {
            URL url = new URL(rssURL);
            Log.d(TAG, "URL CREATED");
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            Log.d(TAG, "CONN CREATED");
            Log.d(TAG, "Response code: " + connection.getResponseCode());
            if (connection.getResponseCode() == HttpURLConnection.HTTP_OK) {
                Log.d(TAG, "CONNECTED");
                InputStream in = connection.getInputStream();
                Log.d(TAG, "GOT STREAM");
                DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
                DocumentBuilder db = dbf.newDocumentBuilder();

                Document document = db.parse(in);
                Log.d(TAG, "GOT DOCUMENT");
                Element element = document.getDocumentElement();
                Log.d(TAG, "GOT ELEMENT");

                NodeList nodeList = element.getElementsByTagName("item");
                Log.d(TAG, "GOT NODELIST");

                if (nodeList.getLength() > 0) {
                    for (int i = 0; i < nodeList.getLength(); i++) {
                        Element entry = (Element) nodeList.item(i);

                        Log.d(TAG, "GOT ENTRY " + i);

                        Element titleElement = (Element) entry.getElementsByTagName("title").item(0);
                        Log.d(TAG, "got title element");
                        Element descriptionElement = (Element) entry.getElementsByTagName("description").item(0);
                        Log.d(TAG, "got descr element");
                        Element fullTextElement = (Element) entry.getElementsByTagName("yandex:full-text").item(0);
                        Log.d(TAG, "got full text element");
                        if (fullTextElement == null) {
                            fullTextElement = (Element) entry.getElementsByTagName("full-text").item(0);
                            Log.d(TAG, "got alter full text element");
                        }



                        Element linkElement = (Element) entry.getElementsByTagName("link").item(0);
                        Log.d(TAG, "got link element");
                        Element pictureElement = (Element) entry.getElementsByTagName("enclosure").item(0);
                        Log.d(TAG, "got pic element");
                        Element categoryElement = (Element) entry.getElementsByTagName("category").item(0);
                        Log.d(TAG, "got category element");
                        Element pubDateElement = (Element) entry.getElementsByTagName("pubDate").item(0);
                        Log.d(TAG, "got pubdate element");

                        String title = titleElement.getFirstChild().getNodeValue();
                        String description = descriptionElement.getFirstChild().getNodeValue();
                        String fullText = fullTextElement == null ? "No full text provided." :
                                fullTextElement.getFirstChild().getNodeValue();
                        String link = linkElement.getFirstChild().getNodeValue();
                        String picture = pictureElement.getAttribute("url");
                        String category = categoryElement == null ? "Other" :
                                categoryElement.getFirstChild().getNodeValue();
                        Date pubDate = new Date(pubDateElement.getFirstChild().getNodeValue());

                        News newNews = new News(title, description, fullText, link, picture,
                                category, pubDate);
                        news.add(newNews);
                    }
                }
            }
        }
        catch(Exception e) {
            e.printStackTrace();
        }

        return news;
    }

}
