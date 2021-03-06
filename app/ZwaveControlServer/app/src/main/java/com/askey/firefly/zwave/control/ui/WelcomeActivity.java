package com.askey.firefly.zwave.control.ui;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.RemoteException;
import android.support.annotation.Nullable;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.askey.firefly.zwave.control.R;
import com.askey.firefly.zwave.control.dao.ZwaveDevice;
import com.askey.firefly.zwave.control.dao.ZwaveDeviceManager;
import com.askey.firefly.zwave.control.service.MQTTBroker;
import com.askey.firefly.zwave.control.service.ZwaveControlService;
import com.askey.firefly.zwave.control.thirdparty.usbserial.driver.UsbSerialDriver;
import com.askey.firefly.zwave.control.thirdparty.usbserial.driver.UsbSerialPort;
import com.askey.firefly.zwave.control.thirdparty.usbserial.driver.UsbSerialProber;
import com.askey.firefly.zwave.control.utils.DeviceInfo;

import org.json.JSONObject;

import java.util.List;
import java.util.Timer;
import java.util.TimerTask;


/**
 * Created by chiapin on 2017/9/7.
 */

public class WelcomeActivity extends BaseActivity{

    private static String LOG_TAG = WelcomeActivity.class.getSimpleName();

    private AlertDialog alertDialog;
    private Timer timer = new Timer(true);

    private ZwaveControlService zwaveService;
    private ZwaveDeviceManager zwDevManager;

    private BroadcastReceiver usbReceiver = null;
    private static final String ACTION_USB_PERMISSION = "com.android.example.USB_PERMISSION";

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_welcome);
        mContext = this;

        showProgressDialog(mContext, "Initializing，Open Zwave Controller...");

        Intent MqttIntent = new Intent(WelcomeActivity.this, MQTTBroker.class);
        startService(MqttIntent);

        zwDevManager = ZwaveDeviceManager.getInstance(this);

        // bind service
        Intent serviceIntent = new Intent(this, ZwaveControlService.class);
        this.bindService(serviceIntent, conn, Context.BIND_AUTO_CREATE);

        new Thread(checkInitStatus).start();
    }

    public Runnable checkInitStatus = new Runnable() {
        @Override
        public void run() {
            while (!DeviceInfo.isZwaveInitFinish || !DeviceInfo.isMQTTInitFinish) {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            //try {
            //    Thread.sleep(1500);
            //} catch (InterruptedException e) {
            //    e.printStackTrace();
            //}
            initSensorfunc();
            initZwave();
        }
    };

    private void initZwave() {
        ((Activity) mContext).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                timerCancel();
                Intent intent = new Intent();
                intent.setClass(mContext, HomeActivity.class);
                hideProgressDialog();
                mContext.startActivity(intent);
                finish();
            }
        });
    }

    private void showZwaveControlTimeOutDialog(String titleStr) {
        if(alertDialog == null) {
            AlertDialog.Builder addDialog = new AlertDialog.Builder(mContext);
            LayoutInflater layoutInflater = LayoutInflater.from(mContext);
            View view = layoutInflater.inflate(R.layout.dialog_retry_layout, null);
            addDialog.setView(view);
            alertDialog = addDialog.create();
            alertDialog.show();

            TextView title = (TextView) view.findViewById(R.id.title);
            TextView message = (TextView) view.findViewById(R.id.message);
            title.setText("Prompt");
            message.setText(titleStr);
            Button positiveButton = (Button) view.findViewById(R.id.positiveButton);
            Button negativeButton = (Button) view.findViewById(R.id.negativeButton);

            positiveButton.setText("retry");
            positiveButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    //retry to opencontroller
                    openController();
                    showProgressDialog(mContext, "OpenController....");
                    alertDialogCancel();
                }
            });

            negativeButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    //tap cancel button and exit to main screen
                    finish();
                    timerCancel();

                    alertDialogCancel();
                    DeviceInfo.isMQTTInitFinish = false;
                    DeviceInfo.isOpenControllerFinish = false;
                    DeviceInfo.isZwaveInitFinish = false;
                    System.exit(0);
                }
            });
        }
    }

    public Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 2001:
                    timerCancel();
                    hideProgressDialog();
                    showZwaveControlTimeOutDialog("Zwave OpenController Timeout");
                    break;
            }
        }
    };

    class mTimerTask extends TimerTask {
        public void run() {
            zwaveService.closeController();
            Log.d(LOG_TAG,"timer on schedule");
            Message message = new Message();
            message.what = 2001;
            mHandler.sendMessage(message);
            timerCancel();
        }
    }

    private void alertDialogCancel() {
        if(alertDialog!=null){
            alertDialog.dismiss();
            alertDialog=null;
        }
    }

    private void timerCancel() {
        if (timer != null) {
            timer.cancel();
            timer = null;
        }
    }

    // bind service with zwave control service
    private ServiceConnection conn = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName componentName, IBinder iBinder) {
            Log.i(LOG_TAG,"onServiceConnected....");
            zwaveService = ((ZwaveControlService.MyBinder)iBinder).getService();
            //register mCallback
            if (zwaveService != null) {
                zwaveService.register(mCallback);
                requestControlUSBPermission();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            zwaveService = null;
        }
    };

    @Override
    protected void onDestroy() {
        super.onDestroy();
        zwaveService.unregister(mCallback);
        if (usbReceiver != null)
            unregisterReceiver(usbReceiver);
        try {
            this.unbindService(conn);
        } catch (Exception e) {
            Log.d(LOG_TAG, e.toString());
        }
    }

    public ZwaveControlService.zwaveCallBack mCallback = new ZwaveControlService.zwaveCallBack() {

        @Override
        public void zwaveControlResultCallBack(String className, String result) {

        }
    };

    private void requestControlUSBPermission() {
        UsbManager usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
        final List<UsbSerialDriver> drivers = UsbSerialProber.getDefaultProber().findAllDrivers(usbManager);

        boolean found = false;

        for (final UsbSerialDriver driver : drivers) {
            if (found) break;

            final List<UsbSerialPort> ports = driver.getPorts();

            for (final UsbSerialPort port : ports) {
                if(!usbManager.hasPermission(port.getDriver().getDevice())) {
                    PendingIntent mPendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
                    usbManager.requestPermission(port.getDriver().getDevice(), mPendingIntent);
                    IntentFilter filter = new IntentFilter(ACTION_USB_PERMISSION);
                    usbReceiver = new usbReceiver();
                    registerReceiver(usbReceiver, filter);
                } else {
                    openController();
                }
                found = true;
                break;
            }
        }
    }

    private class usbReceiver extends BroadcastReceiver {

        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (ACTION_USB_PERMISSION.equals(action)) {
                synchronized (this) {
                    UsbDevice device = (UsbDevice) intent.getParcelableExtra(UsbManager.EXTRA_DEVICE);
                    if (intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        if (device != null) {
                            openController();
                        }
                    } else {
                        Log.d("USB", "permission denied for device " + device);
                        progressDialog.cancel();
                        finish();
                        System.exit(0);
                    }
                }
            }
        }
    }

    private void openController() {
        timer.schedule(new mTimerTask(), 1000 * 120);
        String openResult = zwaveService.openController();
        if (openResult.contains(":0")){
            DeviceInfo.isOpenControllerFinish = true;
        }
    }

    private void initSensorfunc() {

        List<ZwaveDevice> list = zwDevManager.queryZwaveDeviceList();

        for (int idx = 1; idx < list.size(); idx++) {

            /*
            Log.i(LOG_TAG,"*** NodeId = "+list.get(idx).getNodeId()+" | HomeID = "+list.get(idx).getHomeId()
                    +"| devType="+list.get(idx).getDevType()+" | Name="+list.get(idx).getName()
                    +"| roomName="+list.get(idx).getScene()+"| nodeInfo = "+list.get(idx).getNodeInfo());
            */
            int nodeId = list.get(idx).getNodeId();
            String devType = list.get(idx).getDevType();

            if (devType.equals("SENSOR")) {
                String devNodeInfo = list.get(idx).getNodeInfo();

                if (devNodeInfo.contains("COMMAND_CLASS_BATTERY")) {
                    Log.i(LOG_TAG, "BATTERY");
                    zwaveService.getDeviceBattery(nodeId);
                }

                if (devNodeInfo.contains("COMMAND_CLASS_NOTIFICATION")) {
                    try {
                        JSONObject jsonObject = new JSONObject(devNodeInfo);
                        if (jsonObject.getString("Product id").equals("001F")) {
                            //Water
                            zwaveService.getSensorNotification(nodeId, 0x00, 0x05, 0x00);
                        } else if (jsonObject.getString("Product id").equals("000C")) {
                            //Motion
                            zwaveService.getSensorNotification(nodeId, 0x00, 0x07, 0x00);
                            //Door/Window
                            zwaveService.getSensorNotification(nodeId, 0x00, 0x06, 0x00);
                        } else if (jsonObject.getString("Product id").equals("0036")) {
                            //Door/Window
                            zwaveService.getSensorNotification(nodeId, 0x00, 0x06, 0x00);
                        } else if (jsonObject.getString("Product id").equals("001E")) {
                            //SMOKE
                            zwaveService.getSensorNotification(nodeId, 0x00, 0x01, 0x00);
                        } else if (jsonObject.getString("Product id").equals("0050")) {
                            //Motion
                            zwaveService.getSensorNotification(nodeId, 0x00, 0x07, 0x00);
                        }
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
                if (devNodeInfo.contains("COMMAND_CLASS_SENSOR_MULTILEVEL")) {
                    try {
                        zwaveService.getSensorMultiLevel(nodeId);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
                zwaveService.getMeterSupported(nodeId);
            }
        }
    }
}
