package com.example.uniremote;

import communication.HttpRecv;
import communication.HttpSend;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.ToggleButton;

public class MainActivity extends Activity {

	static Button b1, b2, b3;
	static ToggleButton b4;
	boolean remote_mode = true;
	//String b1_val = "1", b2_val = "2", b3_val = "3";
	static TextView tv1;
	
	String url_s;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
	}

	protected void onStart() {
		super.onStart();

		b1 = (Button) findViewById(R.id.button1);
		b2 = (Button) findViewById(R.id.button2);
		b3 = (Button) findViewById(R.id.button3);
		b4 = (ToggleButton) findViewById(R.id.toggleButton1);
		tv1 = (TextView) findViewById(R.id.textView1);
	
		
		tv1.setText("Remote Mode");
		
		
		//final String tempSignal = "1234";

		b4.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				if (remote_mode) {
					remote_mode = false;
					tv1.setText("Programmer Mode");
				} else {
					remote_mode = true;
					tv1.setText("Remote Mode");
				}
			}
		});

		b1.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				if (remote_mode) {
					url_s = "http://192.168.43.72/send";
					HttpSend send = new HttpSend(url_s);
					send.start();
					tv1.setText("Sending Signal");
					//To debug
					try {
						Thread.sleep(2000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					tv1.setText(send.ret());
				} else {
					url_s = "http://192.168.43.72/recv";
					HttpRecv recv = new HttpRecv(url_s);
					recv.start();
					tv1.setText("Reciving signal");
					try {
						Thread.sleep(2000);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					tv1.setText(recv.ret());
				}
			}
		});

		b2.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				
				/**if (remote_mode) {
					url = "http://" + tf1.getText() + "/" + b1_val;
					GET(url);
					// client.sendReq(b2_val);
				} else {
					b2_val = client.recReq();
				}**/
			}
		});

		b3.setOnClickListener(new OnClickListener() {
			public void onClick(View v) {
				// do something
				// Send a signal if remote mode is on else save the received
				// signal on this
				/**if (remote_mode) {
					url = "http://" + tf1.getText() + "/" + b1_val;
					GET(url);
					// client.sendReq(b3_val);
				} else {
					b3_val = client.recReq();
				}**/
			}
		});

	}

	//GET Method
	
}
