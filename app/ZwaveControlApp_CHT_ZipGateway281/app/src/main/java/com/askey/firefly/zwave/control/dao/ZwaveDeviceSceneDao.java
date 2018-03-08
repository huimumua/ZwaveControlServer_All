package com.askey.firefly.zwave.control.dao;

import java.util.List;
import android.database.Cursor;
import android.database.sqlite.SQLiteStatement;

import org.greenrobot.greendao.AbstractDao;
import org.greenrobot.greendao.Property;
import org.greenrobot.greendao.internal.DaoConfig;
import org.greenrobot.greendao.database.Database;
import org.greenrobot.greendao.database.DatabaseStatement;
import org.greenrobot.greendao.query.Query;
import org.greenrobot.greendao.query.QueryBuilder;

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
        public final static Property DeviceSceneId = new Property(0, Long.class, "deviceSceneId", true, "_id");
        public final static Property SceneName = new Property(1, String.class, "sceneName", false, "sceneName");
        public final static Property NodeId = new Property(2, Integer.class, "nodeId", false, "nodeId");
        public final static Property White = new Property(3, String.class, "white", false, "white");
        public final static Property Warm = new Property(4, String.class, "warm", false, "warm");
        public final static Property Rgb = new Property(5, String.class, "rgb", false, "rgb");
        public final static Property VariableValue = new Property(6, Integer.class, "variableValue", false, "variableValue");
        public final static Property TimerTime = new Property(7, String.class, "timerTime", false, "timerTime");
    }

    private Query<ZwaveDeviceScene> zwaveScene_ZwaveDeviceSceneListQuery;

    public ZwaveDeviceSceneDao(DaoConfig config) {
        super(config);
    }
    
    public ZwaveDeviceSceneDao(DaoConfig config, DaoSession daoSession) {
        super(config, daoSession);
    }

    @Override
    protected final void bindValues(DatabaseStatement stmt, ZwaveDeviceScene entity) {
        stmt.clearBindings();
 
        Long deviceSceneId = entity.getDeviceSceneId();
        if (deviceSceneId != null) {
            stmt.bindLong(1, deviceSceneId);
        }
 
        String sceneName = entity.getSceneName();
        if (sceneName != null) {
            stmt.bindString(2, sceneName);
        }
 
        Integer nodeId = entity.getNodeId();
        if (nodeId != null) {
            stmt.bindLong(3, nodeId);
        }
 
        String white = entity.getWhite();
        if (white != null) {
            stmt.bindString(4, white);
        }
 
        String warm = entity.getWarm();
        if (warm != null) {
            stmt.bindString(5, warm);
        }
 
        String rgb = entity.getRgb();
        if (rgb != null) {
            stmt.bindString(6, rgb);
        }
 
        Integer variableValue = entity.getVariableValue();
        if (variableValue != null) {
            stmt.bindLong(7, variableValue);
        }
 
        String timerTime = entity.getTimerTime();
        if (timerTime != null) {
            stmt.bindString(8, timerTime);
        }
    }

    @Override
    protected final void bindValues(SQLiteStatement stmt, ZwaveDeviceScene entity) {
        stmt.clearBindings();
 
        Long deviceSceneId = entity.getDeviceSceneId();
        if (deviceSceneId != null) {
            stmt.bindLong(1, deviceSceneId);
        }
 
        String sceneName = entity.getSceneName();
        if (sceneName != null) {
            stmt.bindString(2, sceneName);
        }
 
        Integer nodeId = entity.getNodeId();
        if (nodeId != null) {
            stmt.bindLong(3, nodeId);
        }
 
        String white = entity.getWhite();
        if (white != null) {
            stmt.bindString(4, white);
        }
 
        String warm = entity.getWarm();
        if (warm != null) {
            stmt.bindString(5, warm);
        }
 
        String rgb = entity.getRgb();
        if (rgb != null) {
            stmt.bindString(6, rgb);
        }
 
        Integer variableValue = entity.getVariableValue();
        if (variableValue != null) {
            stmt.bindLong(7, variableValue);
        }
 
        String timerTime = entity.getTimerTime();
        if (timerTime != null) {
            stmt.bindString(8, timerTime);
        }
    }

    @Override
    public Long readKey(Cursor cursor, int offset) {
        return cursor.isNull(offset + 0) ? null : cursor.getLong(offset + 0);
    }    

    @Override
    public ZwaveDeviceScene readEntity(Cursor cursor, int offset) {
        ZwaveDeviceScene entity = new ZwaveDeviceScene( //
            cursor.isNull(offset + 0) ? null : cursor.getLong(offset + 0), // deviceSceneId
            cursor.isNull(offset + 1) ? null : cursor.getString(offset + 1), // sceneName
            cursor.isNull(offset + 2) ? null : cursor.getInt(offset + 2), // nodeId
            cursor.isNull(offset + 3) ? null : cursor.getString(offset + 3), // white
            cursor.isNull(offset + 4) ? null : cursor.getString(offset + 4), // warm
            cursor.isNull(offset + 5) ? null : cursor.getString(offset + 5), // rgb
            cursor.isNull(offset + 6) ? null : cursor.getInt(offset + 6), // variableValue
            cursor.isNull(offset + 7) ? null : cursor.getString(offset + 7) // timerTime
        );
        return entity;
    }
     
    @Override
    public void readEntity(Cursor cursor, ZwaveDeviceScene entity, int offset) {
        entity.setDeviceSceneId(cursor.isNull(offset + 0) ? null : cursor.getLong(offset + 0));
        entity.setSceneName(cursor.isNull(offset + 1) ? null : cursor.getString(offset + 1));
        entity.setNodeId(cursor.isNull(offset + 2) ? null : cursor.getInt(offset + 2));
        entity.setWhite(cursor.isNull(offset + 3) ? null : cursor.getString(offset + 3));
        entity.setWarm(cursor.isNull(offset + 4) ? null : cursor.getString(offset + 4));
        entity.setRgb(cursor.isNull(offset + 5) ? null : cursor.getString(offset + 5));
        entity.setVariableValue(cursor.isNull(offset + 6) ? null : cursor.getInt(offset + 6));
        entity.setTimerTime(cursor.isNull(offset + 7) ? null : cursor.getString(offset + 7));
     }
    
    @Override
    protected final Long updateKeyAfterInsert(ZwaveDeviceScene entity, long rowId) {
        entity.setDeviceSceneId(rowId);
        return rowId;
    }
    
    @Override
    public Long getKey(ZwaveDeviceScene entity) {
        if(entity != null) {
            return entity.getDeviceSceneId();
        } else {
            return null;
        }
    }

    @Override
    public boolean hasKey(ZwaveDeviceScene entity) {
        return entity.getDeviceSceneId() != null;
    }

    @Override
    protected final boolean isEntityUpdateable() {
        return true;
    }
    
    /** Internal query to resolve the "zwaveDeviceSceneList" to-many relationship of ZwaveScene. */
    public List<ZwaveDeviceScene> _queryZwaveScene_ZwaveDeviceSceneList(String sceneName) {
        synchronized (this) {
            if (zwaveScene_ZwaveDeviceSceneListQuery == null) {
                QueryBuilder<ZwaveDeviceScene> queryBuilder = queryBuilder();
                queryBuilder.where(Properties.SceneName.eq(null));
                zwaveScene_ZwaveDeviceSceneListQuery = queryBuilder.build();
            }
        }
        Query<ZwaveDeviceScene> query = zwaveScene_ZwaveDeviceSceneListQuery.forCurrentThread();
        query.setParameter(0, sceneName);
        return query.list();
    }

}
