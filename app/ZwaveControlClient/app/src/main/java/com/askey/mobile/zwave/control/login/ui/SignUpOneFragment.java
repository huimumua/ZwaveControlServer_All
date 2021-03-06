package com.askey.mobile.zwave.control.login.ui;

import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ImageView;
import android.widget.TextView;

import com.askey.mobile.zwave.control.R;
import com.askey.mobile.zwave.control.interf.FragmentCallback;
import com.askey.mobile.zwave.control.interf.FragmentPage;

import java.util.HashMap;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by skysoft on 2017/10/10.
 */

public class SignUpOneFragment extends Fragment implements View.OnClickListener{
    private EditText etEmailAdress,tvPassword;
    private Map<String,String> account;
    private ImageView emailIcon,passwordIcon,left;
    private SignUpActivity signUpActivity;
    private View view;
    private TextView passwordStrong;
    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        view = inflater.inflate(R.layout.fragment_sign_up_one, null);

        initView();

        FragmentPage.getInstance().setPageCallback(new FragmentCallback() {
            @Override
            public void handlePage() {

                if((signUpActivity.isNextPage)) {
                    signUpActivity.goNextPage(null);
                }

            }
        });

        etEmailAdress.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                if (isEmail(etEmailAdress.getText().toString())) {
                    emailIcon.setVisibility(View.VISIBLE);
                } else {
                    emailIcon.setVisibility(View.INVISIBLE);;
                }

                if (emailIcon.getVisibility() == View.VISIBLE && passwordIcon.getVisibility() == View.VISIBLE) {
                    signUpActivity.isNextPage = true;
                    signUpActivity.right.setImageResource(R.drawable.vector_drawable_ic_next_solid);
                } else {
                    signUpActivity.isNextPage = false;
                    signUpActivity.right.setImageResource(R.drawable.vector_drawable_ic_66);
                }
            }
        });

        tvPassword.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                if (editable.length() >= 6) {
                    passwordStrong.setText(getResources().getString(R.string.password_strong));
                    passwordStrong.setTextColor(Color.RED);
                    passwordIcon.setVisibility(View.VISIBLE);
                } else {
                    passwordIcon.setVisibility(View.INVISIBLE);
                    if (editable.length() <= 0) {
                        passwordStrong.setText(getResources().getString(R.string.password_start));
                        passwordStrong.setTextColor(Color.GRAY);
                    } else {
                        passwordStrong.setText(getResources().getString(R.string.password_weak));
                        passwordStrong.setTextColor(Color.RED);
                    }
                }

                if (emailIcon.getVisibility() == View.VISIBLE && passwordIcon.getVisibility() == View.VISIBLE) {
                    signUpActivity.isNextPage = true;
                    signUpActivity.right.setImageResource(R.drawable.vector_drawable_ic_next_solid);
                } else {
                    signUpActivity.isNextPage = false;
                    signUpActivity.right.setImageResource(R.drawable.vector_drawable_ic_66);
                }

            }
        });

        return view;
    }

    private void initView() {
        signUpActivity =  ((SignUpActivity) getActivity());
        passwordStrong = (TextView) view.findViewById(R.id.tv_password_strong);
        etEmailAdress = (EditText) view.findViewById(R.id.et_email_adress);
        tvPassword = (EditText) view.findViewById(R.id.et_password);
        emailIcon = (ImageView) view.findViewById(R.id.iv_enter_email);
        passwordIcon = (ImageView) view.findViewById(R.id.iv_enter_password);
        left = (ImageView) view.findViewById(R.id.iv_left);
        left.setOnClickListener(this);
        account = new HashMap<>();
    }

    public Map<String,String> getAccount() {
       String email = etEmailAdress.getText().toString();
       String password = tvPassword.getText().toString();
        account.put("email",email);
        account.put("password",password);
        return account;
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.iv_left:
                signUpActivity.onBackPressed();
                break;
        }
    }

    public static boolean isEmail(String email){
        if (null==email || "".equals(email)) return false;
        //Pattern p = Pattern.compile("\\w+@(\\w+.)+[a-z]{2,3}"); //简单匹配
        Pattern p =  Pattern.compile("\\w+([-+.]\\w+)*@\\w+([-.]\\w+)*\\.\\w+([-.]\\w+)*");//复杂匹配
        Matcher m = p.matcher(email);
        return m.matches();
    }
}
