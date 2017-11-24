package com.askey.mobile.zwave.control.guideSetting.ui;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.amazonaws.mobileconnectors.iot.AWSIotMqttClientStatusCallback;
import com.askey.mobile.zwave.control.R;
import com.askey.mobile.zwave.control.base.BaseActivity;
import com.askey.mobile.zwave.control.deviceContr.localMqtt.MQTTManagement;
import com.askey.mobile.zwave.control.deviceContr.localMqtt.MqttMessageArrived;
import com.askey.mobile.zwave.control.deviceContr.model.DeviceList;
import com.askey.mobile.zwave.control.deviceContr.net.SocketTransceiver;
import com.askey.mobile.zwave.control.deviceContr.net.TCPReceive;
import com.askey.mobile.zwave.control.deviceContr.net.TcpClient;
import com.askey.mobile.zwave.control.home.activity.HomeActivity;
import com.askey.mobile.zwave.control.login.ui.LogInActivity;
import com.askey.mobile.zwave.control.util.Const;
import com.askey.mobile.zwave.control.util.Logg;
import com.askey.mobile.zwave.control.util.PreferencesUtils;
import com.askeycloud.sdk.device.response.AWSIoTCertResponse;
import com.askeycloud.sdk.device.response.IoTDeviceInfoResponse;
import com.askeycloud.webservice.sdk.iot.AskeyIoTUtils;
import com.askeycloud.webservice.sdk.iot.callback.MqttConnectionCallback;
import com.askeycloud.webservice.sdk.iot.callback.MqttServiceConnectedCallback;
import com.askeycloud.webservice.sdk.model.ServicePreference;
import com.askeycloud.webservice.sdk.model.auth.v3.DeviceProvidersQueryOptions;
import com.askeycloud.webservice.sdk.service.device.AskeyIoTDeviceService;
import com.askeycloud.webservice.sdk.service.iot.AskeyIoTService;
import com.askeycloud.webservice.sdk.service.web.AskeyWebService;
import com.askeycloud.webservice.sdk.task.DeviceOAuthApiCallback;
import com.google.gson.Gson;

import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.util.List;
import java.util.Timer;


public class DeviceGuideHomeActivity extends BaseActivity {
    private static String TAG = "DeviceGuideHomeActivity";
    private Timer timer;

    private AlertDialog alertDialog;
    private String  mqttResult;
    private String userId,cert,pk;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_guide_home);
        if(Const.isRemote){
            deviceAuth();
        }else{
            TcpClient.getInstance().rigister(tcpReceive);
            tcpConnect(Const.SERVER_IP,Const.TCP_PORT);
            initLocalMqtt();
        }

    }

    MqttMessageArrived mqttMessageArrived = new MqttMessageArrived() {
        @Override
        public void mqttMessageArrived(String topic, MqttMessage message) {
            mqttResult = new String(message.getPayload());
            Logg.i(TAG,"=mqttMessageArrived=>=topic="+topic);
            Logg.i(TAG,"=mqttMessageArrived=>=message="+mqttResult);
            //解析数据  将数据装入data后展示出来

            getDeviceResult(mqttResult);
            MQTTManagement.getSingInstance().unrigister(mqttMessageArrived);
        }
    };


    TCPReceive tcpReceive = new TCPReceive() {
        @Override
        public void onConnect(SocketTransceiver transceiver) {
            Logg.i(TAG,"=%%%%%%%%%%%%%%%%%%%%%=onConnect=");

        }

        @Override
        public void onConnectFailed() {
            Logg.i(TAG,"=%%%%%%%%%%%%%%%%%%%%%=onConnectFailed=");
        }

        @Override
        public void receiveMessage(SocketTransceiver transceiver, String tcpMassage) {
            Logg.i(TAG,"=TCPReceive=>=receiveMessage="+tcpMassage);
            //在这里处理结果
            if(tcpMassage.contains("setDefault:0")){
                Logg.i(TAG,"====setDefault:0====");
/*                ((Activity) mContext).runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
//                        MQTTManagement.getSingInstance().publishMessage(Const.subscriptionTopic,"mobile_zwave:getDevices");
                        MQTTManagement.getSingInstance().publishMessage(Const.subscriptionTopic, LocalMqttData.getDeviceListCommand("All"));
                    }
                });*/

            }
        }

        @Override
        public void onDisconnect(SocketTransceiver transceiver) {
            Logg.i(TAG,"=%%%%%%%%%%%%%%%%%%%%%=onDisconnect=");
        }


    };

    //mqtt调用返回结果
    private void getDeviceResult(String result) {
        DeviceList deviceList = new Gson().fromJson(result, DeviceList.class);
        List<DeviceList.NodeInfoList> temp =  deviceList.getNodeList();
        for (DeviceList.NodeInfoList nodeInfoTemp : temp) {
            String nodeId = nodeInfoTemp.getNodeId();
            String nodeTopic = Const.subscriptionTopic +"Zwave"+ nodeId;
            // 订阅新设备的topic为 sn + nodeId
            MQTTManagement.getSingInstance().subscribeToTopic(nodeTopic,null);
        }

        Logg.i(TAG,"====getDeviceResult===result=="+result);

    }


    //tcp连接
    private void tcpConnect(String tcpServer, int tcpPort) {
        try {
            TcpClient.getInstance().connect(tcpServer, tcpPort);
        } catch (NumberFormatException e) {
            e.printStackTrace();
        }
    }

    private void initLocalMqtt() {
        MQTTManagement mqttManagement = MQTTManagement.getSingInstance();
        mqttManagement.initMqttCallback(Const.clientId, Const.serverUri, new MQTTManagement.initMqttCallback() {
            @Override
            public void initMQTT(boolean result) {
                if (result) {
                    Intent intent = new Intent();
                    intent.setClass(mContext, HomeActivity.class);
                    mContext.startActivity(intent);
                    finish();
                } else {
                    //目前先不管出错情况
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            showFailedConnectMQTTDialog();
                        }
                    });
                }
            }
        });
    }

    private void showFailedConnectMQTTDialog() {
        if(alertDialog == null){
            AlertDialog.Builder addDialog = new AlertDialog.Builder(mContext);
            LayoutInflater layoutInflater = LayoutInflater.from(mContext);
            View view = layoutInflater.inflate(R.layout.dialog_retry_layout, null);
            addDialog.setView(view);
            alertDialog = addDialog.create();

            TextView title = (TextView) view.findViewById(R.id.title);
            TextView message = (TextView) view.findViewById(R.id.message);
            title.setText("Prompt");
            message.setText("MQTT init faild");
            Button positiveButton = (Button) view.findViewById(R.id.positiveButton);
            Button negativeButton = (Button) view.findViewById(R.id.negativeButton);
            Button go_wan = (Button) view.findViewById(R.id.go_wan);
            positiveButton.setText("retry");
            positiveButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    //点击重试，返回添加设备界面，再次执行添加设备
                    showProgressDialog(mContext, "Initializing，Create an MQTT link...");
                    initLocalMqtt();
                    alertDialogCancel();
                }
            });
            go_wan.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    Logg.i(TAG,"=showFailedConnectMQTTDialog=goWan=");
                    hideProgressDialog();
                    showProgressDialog(mContext, "Initializing，Create an MQTT link...");
                    Const.isRemote = true;
                    alertDialogCancel();
                }
            });
            negativeButton.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View view) {
                    //点击取消，返回主页
                    finish();
                    alertDialogCancel();
                }
            });
            alertDialog.show();
        }
    }

    private void alertDialogCancel() {
        if(alertDialog!=null){
            alertDialog.dismiss();
            alertDialog=null;
        }
    }

    //登录之后调用
    private void deviceAuth() {
       boolean result = ServicePreference.isAuthV3UserDataExist(mContext);
        Logg.i(TAG,"===deviceAuth===isAuthV3UserDataExist=="+result);
        String tutk_uuid = (String) PreferencesUtils.get(mContext, Const.TUTK_TUUID_TAG, "");

        DeviceProvidersQueryOptions options = new DeviceProvidersQueryOptions();
        Logg.i(TAG,"==deviceAuth====Const.AUTH_APP_ID===" + Const.AUTH_APP_ID);
        Logg.i(TAG,"==deviceAuth====DEVICE_MODEL===" + Const.DEVICE_MODEL);
        Logg.i(TAG,"==deviceAuth====tutk_uuid===" + tutk_uuid);
        options.setDeviceAuthAppId(Const.AUTH_APP_ID);
        options.setDeviceModel(Const.DEVICE_MODEL);
        options.setDeviceAuthUniqueId(tutk_uuid/*"be31eb33253d1cc7"*/);//只要是唯一的字串參數就好

        DeviceOAuthApiCallback deviceOAuthApiCallback = new DeviceOAuthApiCallback() {
            @Override
            public void bindingDeviceSuccess(int i) {
                Logg.i(TAG,"===deviceOAuthApiCallback===success===" + i);

                lookupIoTDevice(mContext);
            }

            @Override
            public void bindingDeviceError(int i, String s, String s1) {
                Logg.i(TAG,"======success===" + " i=" + i + " s=" + s + " s1= " + s1);
            }
        };
        AskeyWebService.getInstance(this).activeDeviceV3(options, Const.AUTH_APP_ID, deviceOAuthApiCallback);
    }



    /**
     * 检查设备是否存在
     * */
    public  void lookupIoTDevice(final Context mcontext) {
        new Thread(new Runnable() {
            @Override
            public void run() {
                String deviceId = (String) PreferencesUtils.get(mContext, Const.TOPIC_TAG, "");
//                String tutk_uuid = (String) PreferencesUtils.get(mContext, Const.TUTK_TUUID_TAG, "");
//                Logg.i(TAG, "=====lookupIoTDevice===deviceId====="+deviceId);
                IoTDeviceInfoResponse ioTDeviceInfoResponse = AskeyIoTDeviceService.getInstance(mcontext).lookupIoTDevice(Const.DEVICE_MODEL, deviceId );
                if(ioTDeviceInfoResponse!=null && !"".equals(ioTDeviceInfoResponse)){
                    int code = ioTDeviceInfoResponse.getCode();
                    Logg.i(TAG, "===lookupIoTDevice===DEVICE_MODEL====" + Const.DEVICE_MODEL );
                    Logg.i(TAG, "===lookupIoTDevice===deviceId====" + Const.subscriptionTopic );
                    Logg.i(TAG, "===lookupIoTDevice===getCode====" + ioTDeviceInfoResponse.getCode() );
                    Logg.i(TAG, "===lookupIoTDevice===getMessage====" + ioTDeviceInfoResponse.getMessage() );
                    Logg.i(TAG, "===lookupIoTDevice===getAddtionMessage====" + ioTDeviceInfoResponse.getAddtionMessage() );

                    if(400005 == code || 403 == code){
                        //设备不存在  进行创建
                        try {
                            Thread.sleep(3000);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                        lookupIoTDevice(mcontext);
                    }else if(400006 == code ){
                        ((Activity) mContext).runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                showLookupIoTDialog();
                            }
                        });
                    }else if(200 == code){
                        //存在成功
                        getCertificateKey(ioTDeviceInfoResponse);
                    }

                }

            }
        }).start();

    }



    public  void getCertificateKey(final IoTDeviceInfoResponse ioTDeviceInfoResponse) {
        AWSIoTCertResponse response = AskeyIoTDeviceService.getInstance(appContext).getIotCert();
        Logg.i(TAG, "==getCertificateKey====response=" + response);
        final MqttServiceConnectedCallback mqttServiceConnectedCallback = new MqttServiceConnectedCallback() {
            @Override
            public void onMqttServiceConnectedSuccess() {
                Logg.i(TAG, "===MqttServiceConnectedCallback===onMqttServiceConnectedSuccess===");
                if(!"".equals(userId) && !"".equals(cert) && !"".equals(pk)){
                    AskeyIoTService.getInstance(appContext).connectToAWSIot(userId, cert, pk,
                            new MqttConnectionCallback() {
                                @Override
                                public void onConnected() {
                                    Logg.i(TAG, "====MqttConnectionCallback==onConnected===");
                                    if (ioTDeviceInfoResponse != null) {
                                        HomeActivity.shadowTopic = ioTDeviceInfoResponse.getShadowTopic();
//                                        MqttService mqttService = MqttService.getInstance();
//                                        mqttService.subscribeMqttTopic(ioTDeviceInfoResponse.getShadowTopic());
//                                        AskeyIoTService.getInstance(getApplicationContext()).subscribeMqtt(Const.subscriptionTopic);
                                        AskeyIoTService.getInstance(getApplicationContext()).subscribeMqtt(HomeActivity.shadowTopic);
//                                        AskeyIoTService.getInstance(getApplicationContext()).subscribeMqtt(ioTDeviceInfoResponse.getShadowTopic());
//                                        AskeyIoTService.getInstance(getApplicationContext()).subscribeMqttDelta(ioTDeviceInfoResponse.getShadowTopic());
                                        if (Const.isRemote) {
                                            Intent intent = new Intent(mContext, HomeActivity.class);
                                            startActivity(intent);
                                        }
                                    }
                                }

                                @Override
                                public void unConnected(AWSIotMqttClientStatusCallback.AWSIotMqttClientStatus awsIotMqttClientStatus) {
                                    Logg.i(TAG, "===MqttConnectionCallback===unConnected ===");

                                }
                            });
                }

            }

            @Override
            public void onMqttServiceConnectedError() {
                Logg.i(TAG, "======onMqttServiceConnectedError===");
            }
        };


        if(response != null){
            response = AskeyIoTDeviceService.getInstance(appContext).getIotCert();
            cert = response.getCertificatePem();
            pk = response.getPrivateKey();
            userId = response.getUserid();
            Logg.i(TAG, "==getCertificateKey===getCertificatePem==" + cert);
            Logg.i(TAG, "==getCertificateKey====getPrivateKey==" + pk);
            Logg.i(TAG, "==getCertificateKey====getUserid==" + userId);
        }

        new Thread(new Runnable() {
            @Override
            public void run() {
                if (ioTDeviceInfoResponse != null) {
                    Logg.i(TAG, "=ioTDeviceInfoResponse.getRestEndpoint()==" + ioTDeviceInfoResponse.getRestEndpoint());
                    if (ioTDeviceInfoResponse.getRestEndpoint() != null) {
                        AskeyIoTService.getInstance(appContext).configAWSIot(
                                AskeyIoTUtils.translatMqttUseEndpoint(ioTDeviceInfoResponse.getRestEndpoint()),
                                mqttServiceConnectedCallback
                        );
                    }
                }
            }
        }).start();


    }

    private void showLookupIoTDialog() {
        final AlertDialog.Builder addDialog = new AlertDialog.Builder(this);
        LayoutInflater layoutInflater = LayoutInflater.from(this);
        View view = layoutInflater.inflate(R.layout.dialog_normal_layout, null);
        addDialog.setView(view);
        final AlertDialog alertDialog = addDialog.create();
        alertDialog.getWindow().setBackgroundDrawableResource(R.color.transparent);

        TextView title = (TextView) view.findViewById(R.id.title);
        TextView message = (TextView) view.findViewById(R.id.message);
        title.setText("Prompt");
        message.setText("This user has no permission, please use other account login");
        TextView positiveButton = (TextView) view.findViewById(R.id.positiveButton);
        TextView negativeButton = (TextView) view.findViewById(R.id.negativeButton);
        positiveButton.setText("OK");
        positiveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //重新登录
                Intent intent = new Intent(mContext, LogInActivity.class);
                startActivity(intent);
                alertDialog.dismiss();
            }
        });

        negativeButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                //点击取消，返回主页

                alertDialog.dismiss();
                finish();
                System.exit(0);

            }
        });

        alertDialog.show();
    }


    @Override
    protected void onStop() {
        super.onStop();
        Logg.i(TAG,"===onStop=====");
        unrigister();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Logg.i(TAG,"===onDestroy=====");
    }

    private void unrigister() {
        if(tcpReceive!=null){
            TcpClient.getInstance().unrigister(tcpReceive);
        }
        if(mqttMessageArrived!=null){
            MQTTManagement.getSingInstance().unrigister(mqttMessageArrived);
        }
    }



}
