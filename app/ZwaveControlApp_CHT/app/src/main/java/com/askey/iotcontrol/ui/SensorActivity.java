package com.askey.iotcontrol.ui;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import com.askey.iotcontrol.R;
import com.askey.iotcontrol.service.ZwaveControlService;

import org.json.JSONException;
import org.json.JSONObject;

public class SensorActivity extends BaseActivity {
    private static String LOG_TAG = SensorActivity.class.getSimpleName();
    private String nodeInfo;
    int nodeId;

    private TextView txName1,txName2,txName3,txName4,txName5,txName6,txName7,
            txValue1,txValue2,txValue3,txValue4,txValue5,txValue6,txValue7;

    private ZwaveControlService zwaveService;
    private JSONObject jsonObject = null;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_sensor);

        setTopLayout(true, "Sensor Manage");

        // bind service
        Intent serviceIntent = new Intent(this, ZwaveControlService.class);
        this.bindService(serviceIntent, conn, Context.BIND_AUTO_CREATE);

        initView();
        Intent intent = getIntent();
        nodeId = Integer.parseInt(intent.getStringExtra("NodeId"));
        nodeInfo = intent.getStringExtra("NodeInfoList");
        Log.i(LOG_TAG, "sensor nodeId = " + nodeId + " | nodeInfo = " + nodeInfo );

        try {
            jsonObject = new JSONObject(nodeInfo);

            if (jsonObject.getString("Product id").equals("001F")) {
                txName3.setVisibility(View.GONE);
                txValue3.setVisibility(View.GONE);
                txName5.setVisibility(View.GONE);
                txValue5.setVisibility(View.GONE);
                txName6.setVisibility(View.GONE);
                txValue6.setVisibility(View.GONE);
            } else if (jsonObject.getString("Product id").equals("000C")) {
                txName2.setVisibility(View.GONE);
                txValue2.setVisibility(View.GONE);
                txName4.setVisibility(View.GONE);
                txValue4.setVisibility(View.GONE);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    private void initView() {
        txName1 = (TextView) findViewById(R.id.txName1);
        txName2 = (TextView) findViewById(R.id.txName2);
        txName3 = (TextView) findViewById(R.id.txName3);
        txName4 = (TextView) findViewById(R.id.txName4);
        txName5 = (TextView) findViewById(R.id.txName5);
        txName6 = (TextView) findViewById(R.id.txName6);
        txName7 = (TextView) findViewById(R.id.txName7);
        txValue1 = (TextView) findViewById(R.id.txValue1);
        txValue2 = (TextView) findViewById(R.id.txValue2);
        txValue3 = (TextView) findViewById(R.id.txValue3);
        txValue4 = (TextView) findViewById(R.id.txValue4);
        txValue5 = (TextView) findViewById(R.id.txValue5);
        txValue6 = (TextView) findViewById(R.id.txValue6);
        txValue7 = (TextView) findViewById(R.id.txValue7);
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        zwaveService.unregister(mCallback);

        try {
            this.unbindService(conn);
        } catch (Exception e) {
            Log.d(LOG_TAG, e.toString());
        }
    }

    // bind service with zwave control service
    private ServiceConnection conn = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {

            zwaveService = ((ZwaveControlService.MyBinder) iBinder).getService();
            //register mCallback
            if (zwaveService != null) {
                zwaveService.register(mCallback);

            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            zwaveService = null;
        }
    };

    public ZwaveControlService.zwaveCallBack mCallback = new ZwaveControlService.zwaveCallBack() {

        @Override
        public void zwaveControlResultCallBack(String className, String result) {
            Log.d("className", className);
            Log.d("result", result);

            ((Activity) mContext).runOnUiThread(new CallbackRunnable(nodeId, result));
        }
    };

    private class CallbackRunnable implements Runnable {

        private int nodeId;
        private String result;

        public CallbackRunnable(int nodeId, String result) {
            this.nodeId = nodeId;
            this.result = result;
        }

        @Override
        public void run() {
            try {
                //Gson gson = new Gson();
                //ArrayList messageList = gson.fromJson(result, new TypeToken<ArrayList>(){}.getType());

                JSONObject jsonObject = new JSONObject(result);
                String messageType = jsonObject.optString("MessageType");
                int nodeId = jsonObject.optInt("Node id");
                if (this.nodeId == nodeId) {
                    if (messageType.equals("Sensor Info Report")) {
                        String txType = jsonObject.optString("type");
                        String txValue = jsonObject.optString("value");
                        String txUnit = jsonObject.optString("unit");
                        if (txType.equals("Temperature sensor")) {
                            if (txUnit.contains("Celsius")){
                                float temper = (Float.valueOf(txValue));
                                txValue1.setText(String.format("%.02f", temper) + " " + (char) 0x00B0 + "C");
                            }else{
                                float temper = (Float.valueOf(txValue) - 32) * 5 / 9;
                                txValue1.setText(String.format("%.02f", temper) + " " + (char) 0x00B0 + "C");
                            }
                        } else if (txType.equals("Relative humidity sensor")) {
                            txValue2.setText(txValue + " %");
                        } else if (txType.equals("Luminance sensor")) {
                            txValue3.setText(txValue + " %");
                        }
                    }
                    if (messageType.equals("Notification Get Information")) {
                        String notificationType = jsonObject.optString("Notification-type");
                        String notificationEvent = jsonObject.optString("Notification-event");
                        if (notificationType.equals("Water alarm")) {
                            if (notificationEvent.contains("detected")){
                                txValue4.setText("Water leak detected");
                            }
                            else{
                                txValue4.setText("State idle");
                            }

                        } else if (notificationType.equals("Home security")) {
                            if (!notificationEvent.equals("Tampering. Product covering removed")) {
                                txValue5.setText(notificationEvent);
                            }
                        } else if (notificationType.equals("Access control")) {
                            txValue6.setText(notificationEvent);
                        } else if (notificationType.equals("Smoke alarm")) {
                            txValue6.setText(notificationEvent);
                        }
                    }
                    if (messageType.equals("Node Battery Value")) {
                        String batteryValue = jsonObject.optString("Battery Value");
                        txValue7.setText(batteryValue + " %");
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
}