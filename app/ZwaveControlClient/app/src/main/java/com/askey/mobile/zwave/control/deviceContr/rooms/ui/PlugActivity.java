package com.askey.mobile.zwave.control.deviceContr.rooms.ui;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.support.v4.content.ContextCompat;
import android.util.Log;
import android.view.View;
import android.widget.CheckBox;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.askey.mobile.zwave.control.R;
import com.askey.mobile.zwave.control.application.ZwaveClientApplication;
import com.askey.mobile.zwave.control.data.CloudIotData;
import com.askey.mobile.zwave.control.data.LocalMqttData;
import com.askey.mobile.zwave.control.deviceContr.localMqtt.IotMqttManagement;
import com.askey.mobile.zwave.control.deviceContr.localMqtt.IotMqttMessageCallback;
import com.askey.mobile.zwave.control.deviceContr.localMqtt.MQTTManagement;
import com.askey.mobile.zwave.control.deviceContr.localMqtt.MqttMessageArrived;
import com.askey.mobile.zwave.control.home.activity.HomeActivity;
import com.askey.mobile.zwave.control.util.Const;
import com.askey.mobile.zwave.control.util.Logg;
import com.askeycloud.sdk.device.response.IoTDeviceInfoResponse;
import com.askeycloud.webservice.sdk.iot.MqttService;
import com.askeycloud.webservice.sdk.iot.callback.ShadowReceiveListener;
import com.askeycloud.webservice.sdk.iot.message.builder.MqttDesiredJStrBuilder;
import com.askeycloud.webservice.sdk.service.device.AskeyIoTDeviceService;
import com.askeycloud.webservice.sdk.service.iot.AskeyIoTService;

import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by skysoft on 2017/10/18.
 */

public class PlugActivity extends BaseDeviceActivity {
    private static final String TAG = PlugActivity.class.getSimpleName();
    private LinearLayout llPlug;
    private TextView tvActiveMode,tvTriggerDevice,tvModes,tvDetail,tvEnergyOption,tvAlarms,tvScheduler,tvVacation,tvTrigger;
    private CheckBox mOnOff;
    private Context mContext;
    private TextView scheduleExplain,notifyExplain, vacationExplain;
    private IoTDeviceInfoResponse ioTDeviceInfoResponse;
    private  String shadowTopic="";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_plug);

        mContext = this;
        llPlug = (LinearLayout) findViewById(R.id.activity_plug);
        tvActiveMode = (TextView) findViewById(R.id.tv_active_mode);
        tvTriggerDevice = (TextView) findViewById(R.id.tv_trigger_device);
        tvModes = (TextView) findViewById(R.id.tv_modes);
        tvDetail = (TextView) findViewById(R.id.tv_detail);
        scheduleExplain = (TextView) findViewById(R.id.tv_scheduler_explain);
        notifyExplain = (TextView) findViewById(R.id.tv_notify_explain);
        vacationExplain = (TextView) findViewById(R.id.tv_vacation_explain);
//        tvEnergyOption = (TextView) findViewById(R.id.tv_energy_option);
//        tvAlarms = (TextView) findViewById(R.id.tv_alarm);
//        tvScheduler = (TextView) findViewById(R.id.tv_scheduler);
//        tvVacation = (TextView) findViewById(R.id.tv_vacation);
//        tvTrigger = (TextView) findViewById(R.id.tv_trigger);
        mOnOff = (CheckBox) findViewById(R.id.cb_on_off);
        mOnOff.setOnClickListener(this);

        initView();
        initSetted();
        deviceName.setText("Plug");

        nodeId = getIntent().getStringExtra("nodeId");
        type = getIntent().getStringExtra("type");
        name = getIntent().getStringExtra("displayName");
        room = getIntent().getStringExtra("room");

        if(Const.isRemote){
            remoteIotComm(Const.subscriptionTopic+"Zwave"+nodeId,CloudIotData.getSwitchStatus(nodeId));
        }else{
            MQTTManagement.getSingInstance().rigister(mMqttMessageArrived);
            //获取灯泡状态
//            MQTTManagement.getSingInstance().publishMessage(Const.subscriptionTopic+nodeId,"mobile_zwave:getBasic:" + nodeId );
            MQTTManagement.getSingInstance().publishMessage(Const.subscriptionTopic+"Zwave"+nodeId, LocalMqttData.getSwitchStatus(nodeId));

        }
    }


    private void remoteIotComm(final String topic, final String comm) {
        initIotMqttMessage();
            new Thread(new Runnable() {
                @Override
                public void run() {
                    try {
                        Context mContext = ZwaveClientApplication.getInstance();
                        if(ioTDeviceInfoResponse==null){
                            ioTDeviceInfoResponse = AskeyIoTDeviceService.getInstance(mContext).lookupIoTDevice(Const.DEVICE_MODEL, topic );
                            shadowTopic = ioTDeviceInfoResponse.getShadowTopic();
                        }

                        AskeyIoTService.getInstance(mContext).subscribeMqtt(shadowTopic);
                        MqttDesiredJStrBuilder builder = new MqttDesiredJStrBuilder(topic);
                        builder.setJsonString(comm);

                        Log.i(TAG,"===publishDesiredMessage====setJsonString===" +comm);
                        Log.i(TAG,"===publishDesiredMessage====topic===" + topic);
                        Log.i(TAG,"===publishDesiredMessage====shadowTopic===" +shadowTopic);
                        AskeyIoTService.getInstance(mContext).publishDesiredMessage(shadowTopic, builder);
                    }catch (Exception e){
                        Logg.e(TAG,"remoteIotComm = >Exception " +e.getMessage());
                    }
                }
            }).start();
    }

    private void initIotMqttMessage() {
               //以下这句为注册监听
        AskeyIoTService.getInstance(this).setShadowReceiverListener(new ShadowReceiveListener() {
            @Override
            public void receiveShadowDocument(String s, String s1, String s2) {
                Logg.i(TAG, "==IotMqttMessageCallback====setShadowReceiverListener==s=" + s);
                Logg.i(TAG, "==IotMqttMessageCallback====setShadowReceiverListener==s1=" + s1);
                Logg.i(TAG, "==IotMqttMessageCallback====setShadowReceiverListener==s2=" + s2);
                IotMqttManagement.getInstance().receiveMqttMessage(s,s1,s2);
                if(s2.contains("desired")){
                    return;
                }


            }
        });
    }

    MqttMessageArrived mMqttMessageArrived = new MqttMessageArrived() {
        @Override
        public void mqttMessageArrived(String topic, MqttMessage message) {
            final String result = new String(message.getPayload());
            Logg.i(TAG,"=mqttMessageArrived=>=topic="+topic);
            Logg.i(TAG,"=mqttMessageArrived=>=message="+result);

            if(result.contains("desired")){
                return;
            }
            mqttMessageResult(result);
        }
    };

    //mqtt调用返回结果
    private void mqttMessageResult(String result) {
        try {
            JSONObject jsonObject = new JSONObject(result);
            String reported = jsonObject.optString("reported");
            JSONObject reportedObject = new JSONObject(reported);
            String Interface = reportedObject.optString("Interface");
            if(Interface.equals("getSwitchStatus")){
                String switchStatus = reportedObject.optString("switchStatus");
                setSwitchUiStatus(switchStatus);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    private void setSwitchUiStatus(final String switchStatus) {
        ((Activity)mContext).runOnUiThread(new Runnable() {
            @Override
            public void run() {
                if(switchStatus.equals("on")){
                    mOnOff.setChecked(true);
                }else{
                    mOnOff.setChecked(false);
                }
            }
        });
    }
    @Override
    public void setting() {
        super.setting();
    }

    @Override
    public void initSetted() {
        super.initSetted();
    }

    @Override
    public void consume() {
        super.consume();
    }

    @Override
    public void notifyMessage() {
        super.notifyMessage();
    }

    @Override
    public void schedule() {
        super.schedule();
    }

    @Override
    public void vacation() {
        super.vacation();
    }

    @Override
    public void trigger() {
        super.trigger();
    }

    @Override
    public void info() {
        super.info();
        if (isDetailStatus) {

            llPlug.setBackgroundColor(ContextCompat.getColor(this, R.color.white));
            tvActiveMode.setVisibility(View.INVISIBLE);
            tvTriggerDevice.setVisibility(View.INVISIBLE);
            tvModes.setVisibility(View.INVISIBLE);
            tvDetail.setVisibility(View.INVISIBLE);

            scheduleExplain.setVisibility(View.INVISIBLE);
            notifyExplain.setVisibility(View.INVISIBLE);
            vacationExplain.setVisibility(View.INVISIBLE);


//            tvEnergyOption.setVisibility(View.INVISIBLE);
//            tvAlarms.setVisibility(View.INVISIBLE);
//            tvScheduler.setVisibility(View.INVISIBLE);
//            tvVacation.setVisibility(View.INVISIBLE);
//            tvTrigger.setVisibility(View.INVISIBLE);
        } else {

            llPlug.setBackgroundColor(ContextCompat.getColor(this, R.color.gray));
            tvActiveMode.setVisibility(View.VISIBLE);
            tvTriggerDevice.setVisibility(View.VISIBLE);
            tvModes.setVisibility(View.VISIBLE);
            tvDetail.setVisibility(View.VISIBLE);

            scheduleExplain.setVisibility(View.VISIBLE);
            notifyExplain.setVisibility(View.VISIBLE);
            vacationExplain.setVisibility(View.VISIBLE);

//            tvEnergyOption.setVisibility(View.VISIBLE);
//            tvAlarms.setVisibility(View.VISIBLE);
//            tvScheduler.setVisibility(View.VISIBLE);
//            tvVacation.setVisibility(View.VISIBLE);
//            tvTrigger.setVisibility(View.VISIBLE);
        }

    }

    @Override
    public void onClick(View view) {
        super.onClick(view);
        switch (view.getId()) {
            case R.id.cb_on_off:
                if (mOnOff.isChecked()) {
                    //需验证ff 和 00  set无返回
                    if (Const.isRemote) {
                        remoteIotComm(Const.subscriptionTopic+"Zwave"+nodeId,CloudIotData.setSwitch(nodeId,"on"));
                    } else {
                        MQTTManagement.getSingInstance().publishMessage(Const.subscriptionTopic+"Zwave"+nodeId, LocalMqttData.setSwitch(nodeId,"on"));
//                    MQTTManagement.getSingInstance().publishMessage(Const.subscriptionTopic + nodeId, "mobile_zwave:setBasic:" + nodeId + ":FF");
                    }
                } else {
                    if (Const.isRemote) {
                        remoteIotComm(Const.subscriptionTopic+"Zwave"+nodeId,CloudIotData.setSwitch(nodeId,"off"));
                    } else {
//                    MQTTManagement.getSingInstance().publishMessage(Const.subscriptionTopic+nodeId,"mobile_zwave:setBasic:" + nodeId + ":00" );
                        MQTTManagement.getSingInstance().publishMessage(Const.subscriptionTopic+"Zwave"+nodeId, LocalMqttData.setSwitch(nodeId, "off"));
                    }
                }
                break;

        }
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        unrigister();
    }

    private void unrigister() {
        if(mMqttMessageArrived!=null){
            MQTTManagement.getSingInstance().unrigister(mMqttMessageArrived);
        }
    }
}
