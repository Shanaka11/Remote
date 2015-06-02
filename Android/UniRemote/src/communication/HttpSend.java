package communication;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.apache.http.HttpResponse;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.content.Intent;
import android.net.Uri;
import android.util.Log;

public class HttpSend extends Thread{

	String url;
	String ret = "notWorking";
	public HttpSend(String url_i) {
		this.url = url_i;
	}
	
	public String ret(){
		return this.ret;
	}

	public void run() {
		
		//Uri url = Uri.parse("192.168.43.72");
		//Intent launchBrowser = new Intent(Intent.ACTION_VIEW,url);
		//startActivity(launchBrowser);
		//InputStream inputStream = null;
		//String result = "";
		try {
	
			HttpClient httpclient = new DefaultHttpClient();
			
			// ResponseHandler<String> responseHandler = new BasicResponseHandler();
			HttpResponse response = httpclient.execute(new HttpGet(url));
			InputStream inputstream = response.getEntity().getContent();
			
			ret = convertStreamToString(inputstream);
		} catch (Exception e) {
			Log.d("InputStream", e.getLocalizedMessage());
		}

		// return result;
	}


	private String convertStreamToString(InputStream is) {
	    String line = "";
	    StringBuilder total = new StringBuilder();
	    BufferedReader rd = new BufferedReader(new InputStreamReader(is));
	    try {
	        while ((line = rd.readLine()) != null) {
	            total.append(line);
	        }
	    } catch (Exception e) {
	      //  Toast.makeText(this, "Stream Exception", Toast.LENGTH_SHORT).show();
	    }
	    return total.toString();
	}

}
