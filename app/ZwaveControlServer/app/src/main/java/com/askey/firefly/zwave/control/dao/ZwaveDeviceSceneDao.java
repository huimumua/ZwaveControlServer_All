package com.askey.firefly.zwave.control.dao;

import android.database.Cursor;
import android.database.sqlite.SQLiteStatement;

import org.greenrobot.greendao.AbstractDao;
import org.greenrobot.greendao.Property;
import org.greenrobot.greendao.internal.DaoConfig;
import org.greenrobot.greendao.database.Database;
import org.greenrobot.greendao.database.DatabaseStatement;

// THIS CODE IS GENERATED BY greenDAO, DO NOT EDIT.
/** 
 * DAO for table "ZWAVE_DEVICE_SCENE".
*/
public class ZwaveDeviceSceneDao extends AbstractDao<ZwaveDeviceScene, Long> {

    public static final String TABLENAME = "ZWAVE_DEVICE_SCENE";

    /**
     * Properties of entity ZwaveDeviceScene.<br/>
     * Can be used for QueryBuilder and for referencing column names.
     */
    public static class Properties {
        public final static Property SceneId = new Property(0, Long.class, "sceneId", true, "_id");
        public final static Property Scene = new Property(1, String.class, "scene", false, "scene");
        public final static Property Condition = new Property(2, String.class, "condition", false, "condition");
    }


    public ZwaveDeviceSceneDao(DaoConfig config) {
        super(config);
    }
    
    public ZwaveDeviceSceneDao(DaoConfig config, DaoSession daoSession) {
        super(config, daoSession);
    }

    @Override
    protected final void bindValues(DatabaseStatement stmt, ZwaveDeviceScene entity) {
        stmt.clearBindings();
 
        Long sceneId = entity.getSceneId();
        if (sceneId != null) {
            stmt.bindLong(1, sceneId);
        }
 
        String scene = entity.getScene();
        if (scene != null) {
            stmt.bindString(2, scene);
        }
 
        String condition = entity.getCondition();
        if (condition != null) {
            stmt.bindString(3, condition);
        }
    }

    @Override
    protected final void bindValues(SQLiteStatement stmt, ZwaveDeviceScene entity) {
        stmt.clearBindings();
 
        Long sceneId = entity.getSceneId();
        if (sceneId != null) {
            stmt.bindLong(1, sceneId);
        }
 
        String scene = entity.getScene();
        if (scene != null) {
            stmt.bindString(2, scene);
        }
 
        String condition = entity.getCondition();
        if (condition != null) {
            stmt.bindString(3, condition);
        }
    }

    @Override
    public Long readKey(Cursor cursor, int offset) {
        return cursor.isNull(offset + 0) ? null : cursor.getLong(offset + 0);
    }    

    @Override
    public ZwaveDeviceScene readEntity(Cursor cursor, int offset) {
        ZwaveDeviceScene entity = new ZwaveDeviceScene( //
            cursor.isNull(offset + 0) ? null : cursor.getLong(offset + 0), // sceneId
            cursor.isNull(offset + 1) ? null : cursor.getString(offset + 1), // scene
            cursor.isNull(offset + 2) ? null : cursor.getString(offset + 2) // condition
        );
        return entity;
    }
     
    @Override
    public void readEntity(Cursor cursor, ZwaveDeviceScene entity, int offset) {
        entity.setSceneId(cursor.isNull(offset + 0) ? null : cursor.getLong(offset + 0));
        entity.setScene(cursor.isNull(offset + 1) ? null : cursor.getString(offset + 1));
        entity.setCondition(cursor.isNull(offset + 2) ? null : cursor.getString(offset + 2));
     }
    
    @Override
    protected final Long updateKeyAfterInsert(ZwaveDeviceScene entity, long rowId) {
        entity.setSceneId(rowId);
        return rowId;
    }
    
    @Override
    public Long getKey(ZwaveDeviceScene entity) {
        if(entity != null) {
            return entity.getSceneId();
        } else {
            return null;
        }
    }

    @Override
    public boolean hasKey(ZwaveDeviceScene entity) {
        return entity.getSceneId() != null;
    }

    @Override
    protected final boolean isEntityUpdateable() {
        return true;
    }
    
}
