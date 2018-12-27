package com.bosma.jnithread;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    private ThreadDemo threadDemo;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        threadDemo = new ThreadDemo();
        threadDemo.setOnErrorListener(new ThreadDemo.OnErrorListener() {
            @Override
            public void onError(int code, String msg) {
                Log.e(MainActivity.class.getSimpleName(), "onError call code: " + code + " msg : " + msg);
            }
        });
    }

    public void normalThread(View view) {
        threadDemo.normalThread();
    }

    public void mutexThread(View view) {
        threadDemo.mutexThread();
    }

    public void stopMutexThread(View view) {
        threadDemo.stopMutexThread();
    }

    public void javaFromC(View view) {
        threadDemo.callBackFromC();

    }
}
