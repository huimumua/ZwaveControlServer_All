package com.askey.mobile.zwave.control.home.activity.addDevice;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import com.askey.mobile.zwave.control.R;
import com.askey.mobile.zwave.control.base.BaseActivity;
import com.askey.mobile.zwave.control.deviceContr.localMqtt.MQTTManagement;
import com.askey.mobile.zwave.control.deviceContr.localMqtt.MqttMessageArrived;
import com.askey.mobile.zwave.control.deviceContr.net.SocketTransceiver;
import com.askey.mobile.zwave.control.deviceContr.net.TCPReceive;
import com.askey.mobile.zwave.control.deviceContr.net.TcpClient;
import com.askey.mobile.zwave.control.interf.DeleteDeviceListener;
import com.askey.mobile.zwave.control.util.Const;
import com.askey.mobile.zwave.control.util.Logg;
import com.askey.mobile.zwave.control.util.ToastShow;

import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Timer;
import java.util.TimerTask;

public class DeleteDeviceActivity extends BaseActivity implements View.OnClickListener {
    public static String LOG_TAG = "DeleteDeviceActivity";
    private TextView step_title, step_notify, step_notify1;
    private ImageView step_anim, step_iv;
    private static DeleteDeviceListener deleteDeviceListener;
    private String nodeId;
    private String roomName;
    private Timer timer;
    private DeleteDevice deleteCallback;
    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case Const.TCP_TIMEOUT:
                    timerCancel();
                    if (TcpClient.getInstance().isConnected()) {
                        TcpClient.getInstance().getTransceiver().send("mobile_zwave:stopRemoveDevice:Zwave");
                    }
                    if (!DeleteDeviceActivity.this.isFinishing()) {
                        showFailedAddZaveDialog(getResources().getString(R.string.fail_del_notify));
                    }
                    break;
            }
        }
    };

    private void timerCancel() {
        if(timer!=null){
            timer.cancel();
            timer=null;
        }
    }
    class RemoteTimerTask extends TimerTask {
        public void run() {
            Message message = new Message();
            message.what = Const.TCP_TIMEOUT;
            handler.sendMessage(message);
            timerCancel();
        }
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_delete_device);

        initView();
    }

    @Override
    protected void onStart() {
        super.onStart();
        MQTTManagement.getSingInstance().rigister(mMqttMessageArrived);
        TcpClient.getInstance().rigister(tcpReceive);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();

    }

    @Override
    protected void onStop() {
        super.onStop();
        Logg.i(LOG_TAG,"===onStop=====");
        unrigister();
    }

    private void unrigister() {
        if(tcpReceive!=null){
            TcpClient.getInstance().unrigister(tcpReceive);
        }
        if(mMqttMessageArrived!=null){
            MQTTManagement.getSingInstance().unrigister(mMqttMessageArrived);
        }
    }

    private void initView() {
        step_title = (TextView) findViewById(R.id.step_title);
        step_notify = (TextView) findViewById(R.id.step_notify);
        step_notify1 = (TextView) findViewById(R.id.step_notify1);

        step_anim = (ImageView) findViewById(R.id.step_anim);
        step_iv = (ImageView) findViewById(R.id.step_iv);
        step_iv.setOnClickListener(this);

        nodeId = getIntent().getStringExtra("deviceId");
        roomName = getIntent().getStringExtra("roomName");

    }

    public void showFailedAddZaveDialog(final String titleStr) {
        final android.support.v7.app.AlertDialog.Builder addDialog = new android.support.v7.app.AlertDialog.Builder(mContext);
        LayoutInflater layoutInflater = LayoutInflater.from(mContext);
        View view = layoutInflater.inflate(R.layout.dialog_normal_layout, null);
        addDialog.setView(view);
        final android.support.v7.app.AlertDialog alertDialog = addDialog.create();
        //设置dialog背景透明，目的是为了实现弹窗的圆角
        alertDialog.getWindow().setBackgroundDrawableResource(R.color.transparent);
        TextView title = (TextView) view.findViewById(R.id.title);
        TextView message = (TextView) view.findViewById(R.id.message);
        title.setText("Removing time out");
        message.setText(titleStr);
        TextView positiveButton = (TextView) view.findViewById(R.id.positiveButton);
        TextView negativeButton = (TextView) view.findViewById(R.id.negativeButton);
        positiveButton.setText("Again");
        positiveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //预留的接口mqtt
                if (TcpClient.getInstance().isConnected()) {
                    Logg.i("DeleteDeviceActivity", "=removeDevice=" + "mobile_zwave:removeDevice:Zwave:"+ nodeId);
                    TcpClient.getInstance().getTransceiver().send("mobile_zwave:removeDevice:Zwave:"+ nodeId);
                    timer = new Timer(true);
                    timer.schedule(new RemoteTimerTask(),Const.TCP_TIMER_TIMEOUT); //延时1000ms后执行，1000ms执行一次
                    alertDialog.dismiss();
                }
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


    MqttMessageArrived mMqttMessageArrived = new MqttMessageArrived() {
        @Override
        public void mqttMessageArrived(String topic, MqttMessage message) {
            final String result = new String(message.getPayload());
            Logg.i(LOG_TAG,"=mqttMessageArrived=>=topic="+topic);
            Logg.i(LOG_TAG,"=mqttMessageArrived=>=message="+result);
            ((Activity) mContext).runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    try {
                        JSONObject jsonObject = new JSONObject(result);
                        String reported = jsonObject.optString("reported");
                        JSONObject reportedObject = new JSONObject(reported);
                        String Interface = reportedObject.optString("Interface");
                        if(Interface.equals("removeDevice")){
                            String result = reportedObject.optString("Result");
                            String NodeId = reportedObject.optString("NodeId");
                            if(result.equals("true")){

                                //删除成功则返回主页，否则提示删除失败，返回设备管理界面
                                    Log.d("DeleteDeviceActivity","Result:ture");
                                deleteSuccess();
                            } else if (result.equals("fail")) {
                                timerCancel();
                                Toast.makeText(mContext,"Remove Device Failed ! ",Toast.LENGTH_LONG).show();
                                finish();
                            }
                        }

                    } catch (JSONException e) {
                        e.printStackTrace();
                        Logg.i(LOG_TAG,"errorJson------>"+result);
                    }
                }
            });
        }
    };

    private void deleteSuccess(){
        timerCancel();
        DeleteDevice.deleteSuccess(roomName);
        Const.setIsDataChange(true);
        startActivity(new Intent(mContext, DeleteDeviceSuccessActivity.class));
        finish();
    }

    TCPReceive tcpReceive = new TCPReceive() {
        @Override
        public void onConnect(SocketTransceiver transceiver) {

        }

        @Override
        public void onConnectFailed() {

        }

        @Override
        public void receiveMessage(SocketTransceiver transceiver, String tcpMassage) {
            //处理结果
            removeDeviceResult(tcpMassage);

        }

        @Override
        public void onDisconnect(SocketTransceiver transceiver) {

        }

    };

    private void removeDeviceResult(final String result) {
        Logg.i(LOG_TAG,"=====TCP:removeDeviceResult===="+result);
        if(result.contains("mobile_zwave:removeDevice:Zwave")){
            return;
        }else if(result.contains("firefly_zwaver:removeDevice:other")){
            timerCancel();
            final String addResult = mContext.getResources().getString(R.string.add_device_result);
            ToastShow.showToastOnUiThread(mContext,addResult);
            if (TcpClient.getInstance().isConnected()) {
                TcpClient.getInstance().getTransceiver().send("mobile_zwave:stopRemoveDevice:Zwave");
            }
//                finish();
        }else if(result.contains("firefly_zwaver:removeDevice:Zwave:fail")){
            ToastShow.showToastOnUiThread(mContext,getResources().getString(R.string.remove_no_exist_device));
            finish();
            return;
        }else if(result.contains("removeDevice:Zwave:")){
            return;
        }else{
            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    try {
                        final JSONObject jsonObject = new JSONObject(result);
                        String messageType = jsonObject.optString("MessageType");

                        // "MessageType": "Node Remove Status",
                        //"Status": "Learn Ready"
                        if (messageType.equals("Node Remove Status")) {
                            String status = jsonObject.optString("Status");
                            Log.d("DeleteDeviceActivity",status);
                            if ("Success".equals(status)) {
                                Log.d("DeleteDeviceActivity","Node Remove Status : Success");
//                                //删除成功则返回主页，否则提示删除失败，返回设备管理界面
//                                if (null != deleteDeviceListener) {
//                                    Log.d("DeleteDeviceActivity","suc");
//                                    deleteDeviceListener.deleteSuccess(roomName);
//                                    startActivity(new Intent(mContext, DeleteDeviceSuccessActivity.class));
//                                    //数据有变化，通知favorite界面刷新
//                                    Const.setIsDataChange(true);
//                                    finish();
//                                }
                                //deleteSuccess();
                            } else if("Learn Ready".equals(status)){
                                //提示用户按按钮
                                step_notify1.setText("Please press the trigger button of the device");
                            } else if("Failed".equals(status)){
                                timerCancel();
                                Toast.makeText(mContext,"Remove Device Failed ! ",Toast.LENGTH_LONG).show();
                                finish();
                            } else {
                                step_notify1.setText(status);
                            }
                        }
                    } catch (JSONException e) {
                        e.printStackTrace();
                        Logg.i(LOG_TAG,"errorJson------>"+result);
                    }
                }
            });
        }
    }


    @Override
    public void onClick(View v) {

        switch (v.getId()) {
            case R.id.step_iv:
                step_title.setText("Removing");
                step_notify.setText("");
                step_notify1.setText(getResources().getString(R.string.please_wait_a_moment));

                step_anim.setImageResource(R.drawable.ic_loading);
                step_iv.setImageResource(R.drawable.ic_next_solid1108);
                step_iv.setClickable(false);

                if (TcpClient.getInstance().isConnected()) {
                    Logg.i("DeleteDeviceActivity", "=removeDevice=" + "mobile_zwave:removeDevice:Zwave:"+ nodeId);
                    TcpClient.getInstance().getTransceiver().send("mobile_zwave:removeDevice:Zwave:" + nodeId);
                    timer = new Timer(true);
                    timer.schedule(new RemoteTimerTask(),Const.TCP_TIMER_TIMEOUT);
                }
                break;
        }

    }


    /**
     * 监听Back键按下事件,方法2:
     * 注意:
     * 返回值表示:是否能完全处理该事件
     * 在此处返回false,所以会继续传播该事件.
     * 在具体项目中此处的返回值视情况而定.
     */
    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if ((keyCode == KeyEvent.KEYCODE_BACK)) {
            if (TcpClient.getInstance().isConnected()) {
                TcpClient.getInstance().getTransceiver().send("mobile_zwave:stopRemoveDevice:Zwave");
            }
            finish();
            return false;
        }else {
            return super.onKeyDown(keyCode, event);
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (MotionEvent.ACTION_OUTSIDE == event.getAction()) {
            Logg.i(LOG_TAG,"onTouchEvent=====");
            return true;
        }
        return true;
    }

}
