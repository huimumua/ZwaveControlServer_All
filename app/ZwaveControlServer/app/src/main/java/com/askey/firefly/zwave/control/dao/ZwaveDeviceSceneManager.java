package com.askey.firefly.zwave.control.dao;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;

import com.askey.firefly.zwave.control.utils.Const;

import org.greenrobot.greendao.query.QueryBuilder;

import java.util.List;

/**
 * Created by edison_chang on 10/27/2017.
 */

public class ZwaveDeviceSceneManager {

    private static ZwaveDeviceSceneManager instance;
    private Context context;
    private DaoMaster.DevOpenHelper openHelper;

    public ZwaveDeviceSceneManager(Context context) {
        this.context = context;
        this.openHelper = new DaoMaster.DevOpenHelper(context, Const.DBPATH, null);
    }

    public static ZwaveDeviceSceneManager getInstance(Context context) {
        if (instance == null) {
            synchronized (ZwaveDeviceSceneManager.class) {
                if (instance == null) {
                    instance = new ZwaveDeviceSceneManager(context);
                }
            }
        }
        return instance;
    }

    public void insertDeviceScene(ZwaveDeviceScene zwaveDeviceScene) {
        ZwaveDeviceSceneDao zwaveDeviceSceneDao = getZwaveDeviceSceneDao();
        zwaveDeviceSceneDao.insert(zwaveDeviceScene);
    }

    public void insertDeviceSceneList(List<ZwaveDeviceScene> zwaveDeviceSceneList) {
        if (zwaveDeviceSceneList == null || zwaveDeviceSceneList.isEmpty()) return;
        ZwaveDeviceSceneDao zwaveDeviceSceneDao = getZwaveDeviceSceneDao();
        zwaveDeviceSceneDao.insertInTx(zwaveDeviceSceneList);
    }

    public void updateScene(ZwaveDeviceScene zwaveDeviceScene, String scene) {
        ZwaveDeviceSceneDao zwaveDeviceSceneDao = getZwaveDeviceSceneDao();
        ZwaveDeviceScene updateZwaveDeviceScene =
                zwaveDeviceSceneDao.queryBuilder().where(ZwaveDeviceSceneDao.Properties.Scene.eq(scene)).build().unique();
        if (updateZwaveDeviceScene != null) {
            updateZwaveDeviceScene.setScene(scene);
            updateZwaveDeviceScene.setCondition(zwaveDeviceScene.getCondition());
            updateZwaveDeviceScene.setSceneId(zwaveDeviceScene.getSceneId());
            zwaveDeviceSceneDao.update(updateZwaveDeviceScene);
        }
    }

    public void  deleteScene(ZwaveDeviceScene zwaveDeviceScene) {
        ZwaveDeviceSceneDao zwaveDeviceSceneDao = getZwaveDeviceSceneDao();
        zwaveDeviceSceneDao.deleteInTx(zwaveDeviceScene);
    }

    public List<ZwaveDeviceScene> getScene() {
        ZwaveDeviceSceneDao zwaveDeviceSceneDao = getZwaveDeviceSceneDao();
        QueryBuilder<ZwaveDeviceScene> qb = zwaveDeviceSceneDao.queryBuilder();
        List<ZwaveDeviceScene> list = qb.list();
        return list;
    }

    public ZwaveDeviceScene getScene(String sceneName) {
        ZwaveDeviceSceneDao zwaveDeviceSceneDao = getZwaveDeviceSceneDao();
        QueryBuilder<ZwaveDeviceScene> qb = zwaveDeviceSceneDao.queryBuilder();
        qb.where(ZwaveDeviceSceneDao.Properties.Scene.eq(sceneName));
        if (qb.list().isEmpty()) return null;
        ZwaveDeviceScene zwaveDeviceScene = qb.list().get(0);
        return zwaveDeviceScene;
    }

    public ZwaveDeviceSceneDao getZwaveDeviceSceneDao() {
        DaoMaster daoMaster = new DaoMaster(getWritableDatabase());
        DaoSession daoSession = daoMaster.newSession();
        ZwaveDeviceSceneDao zwaveDeviceSceneDao = daoSession.getZwaveDeviceSceneDao();
        return zwaveDeviceSceneDao;
    }

    private SQLiteDatabase getReadableDatabase() {
        if (openHelper == null) {
            openHelper = new DaoMaster.DevOpenHelper(context,  Const.DBPATH, null);
        }
        SQLiteDatabase db = openHelper.getReadableDatabase();
        return db;
    }

    public SQLiteDatabase getWritableDatabase() {
        if (openHelper == null) {
            openHelper = new DaoMaster.DevOpenHelper(context,  Const.DBPATH, null);
        }
        SQLiteDatabase db = openHelper.getWritableDatabase();
        return db;
    }
}
