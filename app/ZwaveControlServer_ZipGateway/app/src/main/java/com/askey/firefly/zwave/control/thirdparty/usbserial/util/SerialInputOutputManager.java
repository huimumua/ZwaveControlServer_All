package com.askey.firefly.zwave.control.thirdparty.usbserial.util;

import android.hardware.usb.UsbRequest;
import android.util.Log;

import com.askey.firefly.zwave.control.thirdparty.usbserial.driver.UsbSerialPort;

import java.io.IOException;
import java.nio.ByteBuffer;

/**
 * Utility class which services a {@link UsbSerialPort} in its {@link #run()}
 * method.
 *
 * @author mike wakerly (opensource@hoho.com)
 */
public class SerialInputOutputManager implements Runnable {

    private static final String TAG = SerialInputOutputManager.class.getSimpleName();
    private static final boolean DEBUG = false;

    private static final int READ_WAIT_MILLIS = 100;
    private static final int BUFSIZ = 4096;

    private final UsbSerialPort mDriver;

    private final ByteBuffer mReadBuffer = ByteBuffer.allocate(BUFSIZ);

    // Synchronized by 'mWriteBuffer'
    private final ByteBuffer mWriteBuffer = ByteBuffer.allocate(BUFSIZ);

    private enum State {
        STOPPED,
        RUNNING,
        STOPPING
    }

    // Synchronized by 'this'
    private State mState = State.STOPPED;

    // Synchronized by 'this'
    private Listener mListener;

    public interface Listener {
        /**
         * Called when new incoming data is available.
         */
        public void onNewData(byte[] data);

        /**
         * Called when {@link SerialInputOutputManager#run()} aborts due to an
         * error.
         */
        public void onRunError(Exception e);
    }

    /**
     * Creates a new instance with no listener.
     */
    public SerialInputOutputManager(UsbSerialPort driver) {
        this(driver, null);
    }

    /**
     * Creates a new instance with the provided listener.
     */
    public SerialInputOutputManager(UsbSerialPort driver, Listener listener) {
        mDriver = driver;
        mListener = listener;
    }

    public synchronized void setListener(Listener listener) {
        mListener = listener;
    }

    public synchronized Listener getListener() {
        return mListener;
    }

    public void writeAsync(byte[] data) {
        synchronized (mWriteBuffer) {
            mWriteBuffer.put(data);
        }
    }

    public synchronized void stop() {
        if (getState() == State.RUNNING) {
            Log.i(TAG, "Stop requested");
            mState = State.STOPPING;
        }
    }

    private synchronized State getState() {
        return mState;
    }

    /**
     * Continuously services the read and write buffers until {@link #stop()} is
     * called, or until a driver exception is raised.
     *
     * NOTE(mikey): Uses inefficient read/write-with-timeout.
     * TODO(mikey): Read asynchronously with {@link UsbRequest#queue(ByteBuffer, int)}
     */
    @Override
    public void run() {
        synchronized (this) {
            if (getState() != State.STOPPED) {
                throw new IllegalStateException("Already running.");
            }
            mState = State.RUNNING;
        }

        Log.i(TAG, "Running ..");
        try {
            while (true) {
                if (getState() != State.RUNNING) {
                    Log.i(TAG, "Stopping mState=" + getState());
                    break;
                }
                step();
            }
        } catch (Exception e) {
            Log.w(TAG, "Run ending due to exception: " + e.getMessage(), e);
            final Listener listener = getListener();
            if (listener != null) {
              listener.onRunError(e);
            }
        } finally {
            synchronized (this) {
                mState = State.STOPPED;
                Log.i(TAG, "Stopped.");
            }
        }
    }

    private void step() throws IOException {
        // Handle incoming data.
        int len = mDriver.read(mReadBuffer.array(), READ_WAIT_MILLIS);
        if (len > 0) {
            if (DEBUG) Log.d(TAG, "Read data len=" + len);
            final Listener listener = getListener();
            if (listener != null) {
                final byte[] data = new byte[len];
                mReadBuffer.get(data, 0, len);
                listener.onNewData(data);
            }
            mReadBuffer.clear();
        }

        // Handle outgoing data.
        byte[] outBuff = null;
        synchronized (mWriteBuffer) {
            len = mWriteBuffer.position();
            if (len > 0) {
                outBuff = new byte[len];
                mWriteBuffer.rewind();
                mWriteBuffer.get(outBuff, 0, len);
                mWriteBuffer.clear();
            }
        }
        if (outBuff != null) {
            if (DEBUG) {
                Log.d(TAG, "Writing data len=" + len);
            }
            mDriver.write(outBuff, READ_WAIT_MILLIS);
        }
    }

}
