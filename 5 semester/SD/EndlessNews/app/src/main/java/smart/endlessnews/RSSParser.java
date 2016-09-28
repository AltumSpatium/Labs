package smart.endlessnews;

import android.os.AsyncTask;

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

class RSSParser extends AsyncTask<String, Void, ArrayList<News>> {
    private String rssURL;

    RSSParser(String url) {
        this.rssURL = url;
    }

    protected ArrayList<News> doInBackground(String... urls) {
        ArrayList<News> news = new ArrayList<>();

        try {
            URL url = new URL(rssURL);
            HttpURLConnection connection = (HttpURLConnection) url.openConnection();
            if (connection.getResponseCode() == HttpURLConnection.HTTP_OK) {
                InputStream in = connection.getInputStream();
                DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
                DocumentBuilder db = dbf.newDocumentBuilder();

                Document document = db.parse(in);
                Element element = document.getDocumentElement();

                NodeList nodeList = element.getElementsByTagName("item");

                if (nodeList.getLength() > 0) {
                    for (int i = 0; i < nodeList.getLength(); i++) {
                        Element entry = (Element) nodeList.item(i);

                        Element titleElement = (Element) entry.getElementsByTagName("title").item(0);
                        Element descriptionElement = (Element) entry.getElementsByTagName("description").item(0);
                        Element fullTextElement = (Element) entry.getElementsByTagName("yandex:full-text").item(0);
                        if (fullTextElement == null) {
                            fullTextElement = (Element) entry.getElementsByTagName("full-text").item(0);
                        }

                        Element linkElement = (Element) entry.getElementsByTagName("link").item(0);
                        Element pictureElement = (Element) entry.getElementsByTagName("enclosure").item(0);
                        Element categoryElement = (Element) entry.getElementsByTagName("category").item(0);
                        Element pubDateElement = (Element) entry.getElementsByTagName("pubDate").item(0);

                        String title = titleElement.getFirstChild().getNodeValue();
                        String description = descriptionElement.getFirstChild().getNodeValue();
                        String fullText = fullTextElement == null ? "No full text provided." :
                                fullTextElement.getFirstChild().getNodeValue();
                        String link = linkElement.getFirstChild().getNodeValue();
                        String picture;
                        if (pictureElement != null)
                            picture = pictureElement.getAttribute("url");
                        else picture = "";
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
