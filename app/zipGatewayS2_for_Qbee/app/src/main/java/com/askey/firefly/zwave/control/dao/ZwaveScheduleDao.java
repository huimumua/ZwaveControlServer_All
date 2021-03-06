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
 * DAO for table "ZWAVE_SCHEDULE".
*/
public class ZwaveScheduleDao extends AbstractDao<ZwaveSchedule, Long> {

    public static final String TABLENAME = "ZWAVE_SCHEDULE";

    /**
     * Properties of entity ZwaveSchedule.<br/>
     * Can be used for QueryBuilder and for referencing column names.
     */
    public static class Properties {
        public final static Property ScheduleId = new Property(0, Long.class, "scheduleId", true, "_id");
        public final static Property NodeId = new Property(1, Integer.class, "nodeId", false, "nodeId");
        public final static Property JobId = new Property(2, Integer.class, "jobId", false, "jobId");
        public final static Property VariableValueStart = new Property(3, Integer.class, "variableValueStart", false, "variableValueStart");
        public final static Property VariableValueEnd = new Property(4, Integer.class, "variableValueEnd", false, "variableValueEnd");
        public final static Property StartTime = new Property(5, String.class, "startTime", false, "startTime");
        public final static Property EndTime = new Property(6, String.class, "endTime", false, "endTime");
        public final static Property Day = new Property(7, String.class, "day", false, "day");
        public final static Property Active = new Property(8, String.class, "active", false, "active");
    }


    public ZwaveScheduleDao(DaoConfig config) {
        super(config);
    }
    
    public ZwaveScheduleDao(DaoConfig config, DaoSession daoSession) {
        super(config, daoSession);
    }

    @Override
    protected final void bindValues(DatabaseStatement stmt, ZwaveSchedule entity) {
        stmt.clearBindings();
 
        Long scheduleId = entity.getScheduleId();
        if (scheduleId != null) {
            stmt.bindLong(1, scheduleId);
        }
 
        Integer nodeId = entity.getNodeId();
        if (nodeId != null) {
            stmt.bindLong(2, nodeId);
        }
 
        Integer jobId = entity.getJobId();
        if (jobId != null) {
            stmt.bindLong(3, jobId);
        }
 
        Integer variableValueStart = entity.getVariableValueStart();
        if (variableValueStart != null) {
            stmt.bindLong(4, variableValueStart);
        }
 
        Integer variableValueEnd = entity.getVariableValueEnd();
        if (variableValueEnd != null) {
            stmt.bindLong(5, variableValueEnd);
        }
 
        String startTime = entity.getStartTime();
        if (startTime != null) {
            stmt.bindString(6, startTime);
        }
 
        String endTime = entity.getEndTime();
        if (endTime != null) {
            stmt.bindString(7, endTime);
        }
 
        String day = entity.getDay();
        if (day != null) {
            stmt.bindString(8, day);
        }
 
        String active = entity.getActive();
        if (active != null) {
            stmt.bindString(9, active);
        }
    }

    @Override
    protected final void bindValues(SQLiteStatement stmt, ZwaveSchedule entity) {
        stmt.clearBindings();
 
        Long scheduleId = entity.getScheduleId();
        if (scheduleId != null) {
            stmt.bindLong(1, scheduleId);
        }
 
        Integer nodeId = entity.getNodeId();
        if (nodeId != null) {
            stmt.bindLong(2, nodeId);
        }
 
        Integer jobId = entity.getJobId();
        if (jobId != null) {
            stmt.bindLong(3, jobId);
        }
 
        Integer variableValueStart = entity.getVariableValueStart();
        if (variableValueStart != null) {
            stmt.bindLong(4, variableValueStart);
        }
 
        Integer variableValueEnd = entity.getVariableValueEnd();
        if (variableValueEnd != null) {
            stmt.bindLong(5, variableValueEnd);
        }
 
        String startTime = entity.getStartTime();
        if (startTime != null) {
            stmt.bindString(6, startTime);
        }
 
        String endTime = entity.getEndTime();
        if (endTime != null) {
            stmt.bindString(7, endTime);
        }
 
        String day = entity.getDay();
        if (day != null) {
            stmt.bindString(8, day);
        }
 
        String active = entity.getActive();
        if (active != null) {
            stmt.bindString(9, active);
        }
    }

    @Override
    public Long readKey(Cursor cursor, int offset) {
        return cursor.isNull(offset + 0) ? null : cursor.getLong(offset + 0);
    }    

    @Override
    public ZwaveSchedule readEntity(Cursor cursor, int offset) {
        ZwaveSchedule entity = new ZwaveSchedule( //
            cursor.isNull(offset + 0) ? null : cursor.getLong(offset + 0), // scheduleId
            cursor.isNull(offset + 1) ? null : cursor.getInt(offset + 1), // nodeId
            cursor.isNull(offset + 2) ? null : cursor.getInt(offset + 2), // jobId
            cursor.isNull(offset + 3) ? null : cursor.getInt(offset + 3), // variableValueStart
            cursor.isNull(offset + 4) ? null : cursor.getInt(offset + 4), // variableValueEnd
            cursor.isNull(offset + 5) ? null : cursor.getString(offset + 5), // startTime
            cursor.isNull(offset + 6) ? null : cursor.getString(offset + 6), // endTime
            cursor.isNull(offset + 7) ? null : cursor.getString(offset + 7), // day
            cursor.isNull(offset + 8) ? null : cursor.getString(offset + 8) // active
        );
        return entity;
    }
     
    @Override
    public void readEntity(Cursor cursor, ZwaveSchedule entity, int offset) {
        entity.setScheduleId(cursor.isNull(offset + 0) ? null : cursor.getLong(offset + 0));
        entity.setNodeId(cursor.isNull(offset + 1) ? null : cursor.getInt(offset + 1));
        entity.setJobId(cursor.isNull(offset + 2) ? null : cursor.getInt(offset + 2));
        entity.setVariableValueStart(cursor.isNull(offset + 3) ? null : cursor.getInt(offset + 3));
        entity.setVariableValueEnd(cursor.isNull(offset + 4) ? null : cursor.getInt(offset + 4));
        entity.setStartTime(cursor.isNull(offset + 5) ? null : cursor.getString(offset + 5));
        entity.setEndTime(cursor.isNull(offset + 6) ? null : cursor.getString(offset + 6));
        entity.setDay(cursor.isNull(offset + 7) ? null : cursor.getString(offset + 7));
        entity.setActive(cursor.isNull(offset + 8) ? null : cursor.getString(offset + 8));
     }
    
    @Override
    protected final Long updateKeyAfterInsert(ZwaveSchedule entity, long rowId) {
        entity.setScheduleId(rowId);
        return rowId;
    }
    
    @Override
    public Long getKey(ZwaveSchedule entity) {
        if(entity != null) {
            return entity.getScheduleId();
        } else {
            return null;
        }
    }

    @Override
    public boolean hasKey(ZwaveSchedule entity) {
        return entity.getScheduleId() != null;
    }

    @Override
    protected final boolean isEntityUpdateable() {
        return true;
    }
    
}
