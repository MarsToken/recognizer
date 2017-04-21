package com.yjtc.yjy.mark.unite.utils.recognize;

import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

import com.yjtc.yjy.mark.unite.utils.recognize.interfaces.ShowResultListener;

public class MainActivity extends AppCompatActivity implements ShowResultListener {
    private TextView tv;
    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            String score = (String) msg.obj;
            tv.setText(score);
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tv = (TextView) findViewById(R.id.sample_text);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        recognizer.recognizer_deInit();
    }

    @Override
    public void showResult(final String score) {
        Message msg = Message.obtain();
        msg.obj = score;
        mHandler.sendMessage(msg);
    }
}
