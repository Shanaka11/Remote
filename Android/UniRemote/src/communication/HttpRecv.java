package communication;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.StatusLine;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.DefaultHttpClient;

import android.util.Log;

public class HttpRecv extends Thread {
	String url;
	String ret = "notWorking : Programmer Mode";
	String dec;
	
	public HttpRecv(String url_i) {
		this.url = url_i;
	}

	public String ret() {
		return this.ret;
	}

	public void run() {

	
		try {

			HttpClient httpclient = new DefaultHttpClient();

			// ResponseHandler<String> responseHandler = new
			// BasicResponseHandler();
			HttpResponse response = httpclient.execute(new HttpGet(url));
			//InputStream inputstream = response.getEntity().getContent();

			ret = response.toString();
			//***********new************//
			
			    //****************8/****//8/
			//ret = convertStreamToString(inputstream);
			//ret = DecodeReq(ret);
			
		} catch (Exception e) {
			Log.d("InputStream", e.getLocalizedMessage());
		}

		// return result;
	}
	
	
	
	private String DecodeReq(String in){
		String[] temp = in.split("\n");
		String returns = "";
		
		if(temp[0].equalsIgnoreCase("1")){
			returns = returns + "NEC ";
		}else{
			returns = "1 ";
		}
		returns = returns + temp[1];
		return returns;
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
			// Toast.makeText(this, "Stream Exception",
			// Toast.LENGTH_SHORT).show();
		}
		return total.toString();
	}
}
