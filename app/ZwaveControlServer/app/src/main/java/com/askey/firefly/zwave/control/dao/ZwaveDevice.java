package com.askey.firefly.zwave.control.dao;

import android.os.Parcel;
import android.os.Parcelable;

import org.greenrobot.greendao.annotation.Entity;
import org.greenrobot.greendao.annotation.Id;
import org.greenrobot.greendao.annotation.Property;
import org.greenrobot.greendao.annotation.Generated;

/**
 * Created by skysoft on 2017/8/7.
 */

@Entity(createInDb=false)
public class ZwaveDevice implements Parcelable {
    @Id(autoincrement = true)
    private Long zwaveId;
    @Property(nameInDb = "homeId")
    private String homeId;
    @Property(nameInDb = "nodeId")
    private Integer nodeId;
    @Property(nameInDb = "name")
    private String name;
    @Property(nameInDb = "nodeInfo")
    private String nodeInfo;
    @Property(nameInDb = "devType")
    private String devType;
    @Property(nameInDb = "scene")
    private String scene;

    @Generated(hash = 597948381)
    public ZwaveDevice(Long zwaveId, String homeId, Integer nodeId, String name,
            String nodeInfo, String devType, String scene) {
        this.zwaveId = zwaveId;
        this.homeId = homeId;
        this.nodeId = nodeId;
        this.name = name;
        this.nodeInfo = nodeInfo;
        this.devType = devType;
        this.scene = scene;
    }

    @Generated(hash = 565410297)
    public ZwaveDevice() {
    }


    protected ZwaveDevice(Parcel in) {
        zwaveId = in.readLong();
        homeId = in.readString();
        name = in.readString();
        nodeInfo = in.readString();
        devType = in.readString();
        scene = in.readString();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeLong(zwaveId);
        dest.writeString(homeId);
        dest.writeString(name);
        dest.writeString(nodeInfo);
        dest.writeString(devType);
        dest.writeString(scene);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<ZwaveDevice> CREATOR = new Creator<ZwaveDevice>() {
        @Override
        public ZwaveDevice createFromParcel(Parcel in) {
            return new ZwaveDevice(in);
        }

        @Override
        public ZwaveDevice[] newArray(int size) {
            return new ZwaveDevice[size];
        }
    };

    public String getScene() {
        return this.scene;
    }

    public void setScene(String scene) {
        this.scene = scene;
    }

    public String getNodeInfo() {
        return this.nodeInfo;
    }

    public void setNodeInfo(String nodeInfo) {
        this.nodeInfo = nodeInfo;
    }

    public String getName() {
        return this.name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public Integer getNodeId() {
        return this.nodeId;
    }

    public void setNodeId(Integer nodeId) {
        this.nodeId = nodeId;
    }

    public String getHomeId() {
        return this.homeId;
    }

    public void setHomeId(String homeId) {
        this.homeId = homeId;
    }

    public String getDevType() {
        return this.devType;
    }

    public void setDevType(String devType) {
        this.devType = devType;
    }

    public Long getZwaveId() {
        return this.zwaveId;
    }

    public void setZwaveId(Long zwaveId) {
        this.zwaveId = zwaveId;
    }

}
