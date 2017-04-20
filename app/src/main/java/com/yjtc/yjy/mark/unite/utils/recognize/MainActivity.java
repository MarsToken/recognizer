package com.yjtc.yjy.mark.unite.utils.recognize;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        TextView tv = (TextView) findViewById(R.id.sample_text);
        tv.setText(recognizer.stringFromJNI());
        boolean s = recognizer.recognizer_init();
        Log.e("tag", "s=" + s);
    }
}
