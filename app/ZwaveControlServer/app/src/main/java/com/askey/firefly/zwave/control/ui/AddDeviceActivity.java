package com.askey.firefly.zwave.control.ui;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.ProgressBar;
import android.widget.Spinner;
import android.widget.TextView;

import com.askey.firefly.zwave.control.R;
import com.askey.firefly.zwave.control.jni.ZwaveControlHelper;
import com.askey.firefly.zwave.control.service.ZwaveControlService;
import com.askey.firefly.zwave.control.utils.DeviceInfo;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Timer;
import java.util.TimerTask;

/**
 * Created by chiapin on 2017/9/28.
 */

public class AddDeviceActivity extends BaseActivity implements View.OnClickListener {

    private static String LOG_TAG = AddDeviceActivity.class.getSimpleName();
    private ImageView ivBack;
    private ProgressBar proBar;
    private Button btnCancel;
    private TextView tvStatus;
    private Timer timer;
    private ZwaveControlService zwaveService;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_add_device);

        ZwaveControlHelper.ZwController_AddDevice();

        ivBack = (ImageView) findViewById(R.id.iv_back);
        proBar = (ProgressBar) findViewById(R.id.proBar);
        tvStatus = (TextView) findViewById(R.id.tv_status);
        tvStatus.setText("Please wait a moment...");
        proBar.setIndeterminate(true);
        btnCancel = (Button) findViewById(R.id.btn_cancel);
        btnCancel.setOnClickListener(this);
        ivBack.setOnClickListener(this);

        timer = new Timer(true);
        timer.schedule(new AddDeviceActivity.mTimerTask(), 1000 * 45); //延时1000ms后执行，1000ms执行一次

        // bind service
        Intent serviceIntent = new Intent(this, ZwaveControlService.class);
        this.bindService(serviceIntent, conn, Context.BIND_AUTO_CREATE);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.iv_back:
                finish();
                break;
            case R.id.btn_cancel:
                showStopAddDialog();
                break;
        }
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
            Log.i(LOG_TAG,"onServiceConnected....");
            zwaveService = ((ZwaveControlService.MyBinder)iBinder).getService();
            //register mCallback
            if (zwaveService != null) {
                zwaveService.register(mCallback);

                zwaveService.addDevice();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            zwaveService = null;
        }
    };


    private Handler mHandler = new Handler() {
        // 重写handleMessage()方法，此方法在UI线程运行
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 2001:
                    hideProgressDialog();
                    timerCancel();
                    showFailedAddZaveDialog("Add Device Timeout");
                    break;
                case 2002:
                    Log.i(LOG_TAG,"2002");
                    hideProgressDialog();
                    timerCancel();
                    showAddDialog(msg.getData().getString("homeId"),msg.getData().getString("nodeId"));
                    break;
            }

        }
    };

    class mTimerTask extends TimerTask {
        public void run() {
            Log.d(LOG_TAG,"timer on schedule");
            Message message = new Message();
            message.what = 2001;
            mHandler.sendMessage(message);
            timerCancel();
        }
    }

    private void timerCancel() {
        if(timer!=null){
            timer.cancel();
            timer=null;
        }
    }

    public void showFailedAddZaveDialog(final String titleStr) {
        final android.support.v7.app.AlertDialog.Builder addDialog = new android.support.v7.app.AlertDialog.Builder(mContext);
        LayoutInflater layoutInflater = LayoutInflater.from(mContext);
        View view = layoutInflater.inflate(R.layout.dialog_normal_layout, null);
        addDialog.setView(view);
        final android.support.v7.app.AlertDialog alertDialog = addDialog.create();

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
                //点击重试，返回添加设备界面，再次执行添加设备
                zwaveService.addDevice();
            }
        });

        negativeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                alertDialog.dismiss();
            }
        });
        alertDialog.show();
    }

    private void showAddDialog(final String homeId ,final String nodeId) {
        Log.i(LOG_TAG,"===showAddDialog===");

        final AlertDialog.Builder addDialog = new AlertDialog.Builder(AddDeviceActivity.this);
        LayoutInflater layoutInflater = LayoutInflater.from(AddDeviceActivity.this);
        View view = layoutInflater.inflate(R.layout.dialog_add_layout, null);
        addDialog.setView(view);

        final AlertDialog alertDialog = addDialog.create();
        alertDialog.show();
        TextView title = (TextView) view.findViewById(R.id.title);
        title.setText("Add Device Success");
        final EditText message = (EditText) view.findViewById(R.id.message);
        final Spinner spDevType = (Spinner) view.findViewById(R.id.spDevType);
        Button positiveButton = (Button) view.findViewById(R.id.positiveButton);
        Button negativeButton = (Button) view.findViewById(R.id.negativeButton);

        ArrayAdapter<String> devTypeList = new ArrayAdapter<>(this,
                android.R.layout.simple_spinner_dropdown_item,
                DeviceInfo.deviceType);

        spDevType.setAdapter(devTypeList);
        spDevType.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
            }
            @Override
            public void onNothingSelected(AdapterView<?> parent) {
            }
        });

        message.setText(nodeId);
        positiveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                reName(homeId , Integer.parseInt(nodeId),message.getText().toString(),
                        spDevType.getSelectedItem().toString());
                alertDialog.dismiss();
                backToHomeActivity();
            }
        });

        negativeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                alertDialog.dismiss();
                finish();
            }
        });

        alertDialog.show();

    }

    private void reName(String homeId ,int nodeId, String newName,String devType) {
        ZwaveControlHelper.ZwController_RemoveDevice();
        zwaveService.reNameDevice(homeId,nodeId,newName,devType);
        //DeviceInfo.memberList.add(new zwNodeMember(nodeId, homeId, devType, newName,""));
    }

    private void showAddFailDialog() {
        final AlertDialog.Builder addDialog = new AlertDialog.Builder(this);
        LayoutInflater layoutInflater = LayoutInflater.from(this);
        View view = layoutInflater.inflate(R.layout.dialog_normal_layout, null);

        final AlertDialog alertDialog = addDialog.create();
        addDialog.setView(view);

        TextView title = (TextView) view.findViewById(R.id.title);
        TextView message = (TextView) view.findViewById(R.id.message);
        title.setText("Prompt");
        message.setText("Add faild");
        Button positiveButton = (Button) view.findViewById(R.id.positiveButton);
        Button negativeButton = (Button) view.findViewById(R.id.negativeButton);
        positiveButton.setText("retry");
        positiveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //点击重试，返回添加设备界面，再次执行添加设备
                proBar.setIndeterminate(true);

                zwaveService.addDevice();
                alertDialog.dismiss();
            }
        });

        negativeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //点击取消，返回主页

                backToHomeActivity();
            }
        });

        alertDialog.show();
    }

    private void showStopAddDialog() {
        final AlertDialog.Builder addDialog = new AlertDialog.Builder(this);
        LayoutInflater layoutInflater = LayoutInflater.from(this);
        View view = layoutInflater.inflate(R.layout.dialog_normal_layout, null);
        addDialog.setView(view);

        final AlertDialog alertDialog = addDialog.create();
        alertDialog.show();
        TextView title = (TextView) view.findViewById(R.id.title);
        TextView message = (TextView) view.findViewById(R.id.message);
        title.setText("Prompt");
        message.setText("Stop Add Device ?");
        Button positiveButton = (Button) view.findViewById(R.id.positiveButton);
        Button negativeButton = (Button) view.findViewById(R.id.negativeButton);
        positiveButton.setText("OK");
        positiveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                timerCancel();
                if (zwaveService!=null) {
                    zwaveService.stopAddDevice();
                }
                alertDialog.dismiss();

                backToHomeActivity();

            }
        });

        negativeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                alertDialog.dismiss();

            }
        });
    }

    //mqtt调用返回结果
    private void addDeviceResult(String result) {
        try {

            final JSONObject jsonObject = new JSONObject(result);
//            final String nodeId = jsonObject.optString("NodeID");
                ((Activity) mContext).runOnUiThread(new Runnable() {
                    @Override
                    public void run() {

                    String messageType = jsonObject.optString("MessageType");
                    String status = jsonObject.optString("Status");
                    if ("Node Add Status".equals(messageType)) {
                        if ("Success".equals(status)) {
                            //Toast.makeText(mContext,"add Success",Toast.LENGTH_SHORT).show();
                            Log.i(LOG_TAG,"=====result=="+"Success");
                            tvStatus.setText("Success, Please wait a moment to rename");
                            proBar.setIndeterminate(false);
                        } else if ("Failed".equals(status)) {
                            Log.i(LOG_TAG,"=====result=="+"Fail");
                            showAddFailDialog();
                            proBar.setIndeterminate(false);
                        } else if("Learn Ready".equals(status)){
                            tvStatus.setText("Please press the trigger button of the device");
                            timerCancel();
                        }else{
                            Log.i(LOG_TAG,"=====result=="+status);
                            tvStatus.setText(status);
                        }
                    }
                    }
                });
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    public ZwaveControlService.zwaveCallBack mCallback = new ZwaveControlService.zwaveCallBack() {

        @Override
        public void zwaveControlResultCallBack(String className, String result) {

            if (className.equals("addDevice")){
                Log.i(LOG_TAG,"===addDeviceCallBack==="+result);
                if (result.contains("addDevice:")){
                    String[] tokens = result.split(":");
                    if (tokens.length<3){
                        Log.i(LOG_TAG,"AIDLResult addDevice : wrong format "+result);
                    } else {
                        String tHomeId = tokens[1];
                        String tNodeId = tokens[2];
                        Log.i(LOG_TAG,"HomeId = "+tHomeId+" | NodeId="+tNodeId);

                        Message message = new Message();
                        Bundle data = new Bundle();
                        data.putString("nodeId", tNodeId);
                        data.putString("homeId", tHomeId);
                        message.setData(data);
                        message.what = 2002;
                        mHandler.sendMessage(message);
                    }
                }else {
                    addDeviceResult(result);
                }
            }
        }
    };

}