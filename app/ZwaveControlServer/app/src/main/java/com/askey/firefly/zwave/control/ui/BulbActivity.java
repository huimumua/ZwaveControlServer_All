package com.askey.firefly.zwave.control.ui;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.graphics.Color;
import android.os.Bundle;
import android.os.IBinder;
import android.support.annotation.Nullable;
import android.util.Log;
import android.view.View;
import android.widget.CheckBox;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.RelativeLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import com.askey.firefly.zwave.control.R;
import com.askey.firefly.zwave.control.service.ZwaveControlService;
import com.askey.firefly.zwave.control.utils.Utils;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * 项目名称：ZwaveControlClient
 * 类描述：
 * 创建人：skysoft  charles.bai
 * 创建时间：2017/8/15 9:40
 * 修改人：chiapin
 * 修改时间：2017/10/20
 * 修改备注：
 */
public class BulbActivity extends BaseActivity implements View.OnClickListener {
    private static String LOG_TAG = BulbActivity.class.getSimpleName();
    private CheckBox cbSwitch;
    private int nodeId,basicValue;
    private String nodeInfo;
    private TextView brightness;
    private SeekBar brightness_change;
    private int brightnessLevel = 0;
    private boolean SeekBarFlg = false;

    private TextView txtColor;
    private ColorPickView myView;
    private LinearLayout Colorlayout;
    private RelativeLayout colorPickerLayout;
    private RadioButton warmButton,coldButton,rgbButton;
    private RadioGroup rGroup;

    private ZwaveControlService zwaveService;



    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_bulb);
        setTopLayout(true, "Bulb Manage");

        Colorlayout = (LinearLayout) findViewById(R.id.colorLinear);

        // bind service
        Intent serviceIntent = new Intent(this, ZwaveControlService.class);
        this.bindService(serviceIntent, conn, Context.BIND_AUTO_CREATE);

        initView();
        Intent intent = getIntent();
        nodeId = Integer.parseInt(intent.getStringExtra("NodeId"));
        nodeInfo = intent.getStringExtra("NodeInfoList");

        if (!nodeInfo.contains("COMMAND_CLASS_SWITCH_COLOR")){
            Log.i(LOG_TAG,"not support change color !");
            Colorlayout.setVisibility(View.GONE);
        }else {
            enableColorPicker();
        }

        cbSwitch = (CheckBox) findViewById(R.id.cb_switch);
        cbSwitch.setOnClickListener(this);

    }

    private RadioGroup.OnCheckedChangeListener listener = new RadioGroup.OnCheckedChangeListener() {

        @Override
        public void onCheckedChanged(RadioGroup group, int checkedId) {
            // TODO Auto-generated method stub
            int count = group.getChildCount();
            switch (checkedId) {
                case R.id.warmWhite:
                    //set the color to warmwhite
                    zwaveService.setSwitchColor(nodeId,0x00,255);
                    colorPickerLayout.setVisibility(View.GONE);
                    break;
                case R.id.coldWite:
                    // enable colde color
                    zwaveService.setSwitchColor(nodeId,0x01,255);
                    //disable warmwhite color
                    zwaveService.setSwitchColor(nodeId,0x00,0);
                    colorPickerLayout.setVisibility(View.GONE);
                    break;
                case R.id.RGBColor:
                    // enable color picker view
                    colorPickerLayout.setVisibility(View.VISIBLE);
                    break;
            }
        }
    };

    private void initView() {

        colorPickerLayout = (RelativeLayout) findViewById(R.id.colorPickerLayout);
        myView = (ColorPickView) findViewById(R.id.color_picker_view);
        txtColor = (TextView) findViewById(R.id.txt_color);

        brightness = (TextView) findViewById(R.id.brightness);
        brightness_change = (SeekBar) findViewById(R.id.brightness_change);

        warmButton = (RadioButton) findViewById(R.id.warmWhite);
        coldButton = (RadioButton) findViewById(R.id.coldWite);
        rgbButton = (RadioButton) findViewById(R.id.RGBColor);
        rGroup = (RadioGroup) findViewById(R.id.radiogroup);

        colorPickerLayout.setVisibility(View.GONE);

        rGroup.setOnCheckedChangeListener(listener);


        brightness_change.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                brightnessLevel = i;
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                zwaveService.setSwitchMultiLevel(nodeId, brightnessLevel, 1);
                if (brightnessLevel!=0 && cbSwitch.isChecked()==false){
                    cbSwitch.setChecked(true);
                }

                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                zwaveService.getBasic(nodeId);
            }
        });
    }

    private void enableColorPicker(){

        myView.setOnColorChangedListener(new ColorPickView.OnColorChangedListener() {

            @Override
            public void onColorChange(int color) {
                txtColor.setTextColor(color);

                zwaveService.setSwitchColor(nodeId,0x02,Color.red(color));
                zwaveService.setSwitchColor(nodeId,0x03,Color.green(color));
                zwaveService.setSwitchColor(nodeId,0x04,Color.blue(color));
                zwaveService.setSwitchColor(nodeId,0x00,0);
                zwaveService.setSwitchColor(nodeId,0x01,0);

            }

        });
    }

    //zwave callback result
    private void zwCBResult(final String result) {

        if (Utils.isGoodJson(result)) {

            try {
                final JSONObject jsonObject = new JSONObject(result);

                //((Activity) mContext).runOnUiThread(new Runnable() {
                //    @Override
                //    public void run() {
                        String messageType = jsonObject.optString("MessageType");

                        if ("Basic Information".equals(messageType)) {
                            String value = jsonObject.optString("value");
                            brightness.setText("Brightness : " + value);

                            if (value.equals("00h")) {
                                //turn off
                                cbSwitch.setChecked(false);
                            } else {
                                //turn on
                                cbSwitch.setChecked(true);

                                //change Hex string to Interger
                                String tmpValue = value.substring(0,value.length()-1);
                                int basicValue = Integer.valueOf(tmpValue,16);

                                if (!SeekBarFlg && value!="00h"){
                                    brightness_change.setProgress(basicValue);
                                    SeekBarFlg = true;
                                }
                            }
                        } else if ("Switch Color Report".equals(messageType)){
                            String txParamater = jsonObject.optString("component id");
                            String txValue = jsonObject.optString("value");

                            Log.i(LOG_TAG,"Parameter = "+txParamater+" | value = "+txValue);
                            /*
                            if (txParamater.equals("Warm Write") && (!txValue.equals("0"))){
                                rGroup.check(R.id.warmWhite);
                            }
                            else if (txParamater.equals("Cold Write") && (!txValue.equals("0"))){
                                rGroup.check(R.id.coldWite);
                            }
                            else{
                                rGroup.check(R.id.RGBColor);
                                colorPickerLayout.setVisibility(View.VISIBLE);
                            }
                            */
                        }
                //    }
                //});
            } catch (JSONException e) {
                e.printStackTrace();
            }
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

    @Override
    public void onClick(View v) {
        switch (v.getId()) {

            case R.id.cb_switch:
                if (cbSwitch.isChecked()) {
                    //zwaveService.setSwitchAllOn(nodeId);
                    zwaveService.setBasic(nodeId,basicValue);
                    zwaveService.getBasic(nodeId);
                } else {
                    //zwaveService.setSwitchAllOff(nodeId);
                    zwaveService.setBasic(nodeId,0);
                    brightness.setText("Brightness : 00h");
                    brightness_change.setProgress(0);
                    SeekBarFlg=false;
                }
                break;
            default:

                break;
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

                zwaveService.getBasic(nodeId);
                zwaveService.getSwitchColor(nodeId,0x00);
                zwaveService.getSwitchColor(nodeId,0x01);
                zwaveService.getSwitchColor(nodeId,0x02);
                zwaveService.getSwitchColor(nodeId,0x03);
                zwaveService.getSwitchColor(nodeId,0x04);
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

        if (className.equals("setSwitchMultiLevel") || className.equals("setBasic")
            || className.equals("getBasic") ||className.equals("setSwitchAllOn")
            || className.equals("setSwitchAllOff") || className.equals("setSwitchColor")
            || className.equals("getSwitchColor")){

            zwCBResult(result);
        }
        }
    };
}