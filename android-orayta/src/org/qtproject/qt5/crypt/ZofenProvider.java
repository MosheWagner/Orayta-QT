package org.qtproject.qt5.crypt;

//Added for kookita
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.content.ComponentName;
import android.content.ServiceConnection;
import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Bundle;

import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;

import android.util.Log;


public class ZofenProvider implements IZofenProvider
{
    private static String zofen = null;
    boolean connected = false;
    
//    //Added for Orayta sdcard location hack
//    private static String sdcard_location = "";


    Activity mActivity;

    public final static int DECRYPT_REQUEST_CODE = 42738492;
    public final static String ZOFEN= "zofen";


    public ZofenProvider(Activity activity)
    {
        mActivity = activity;
    }
    
    public String getZofen() 
    {
    if (isProviderAvailable()  && zofen != null)
        return zofen;
    return "";
    }


    public boolean isProviderAvailable() 
    {
        if (!connected) doServiceConnect();
        return connected;
    }

    private Messenger mService = null;
    private boolean mBound;
    private ServiceConnection mConnection = new ServiceConnection() {
            @Override
            public void onServiceConnected(ComponentName className, IBinder service) {
                    Log.d("IZAR","on service connected");
                    mService = new Messenger(service);
                    mBound = true;
            }
            @Override
            public void onServiceDisconnected(ComponentName className) {
                    Log.d("IZAR","discenncted");
                    mService = null;
                    mBound = false;

            }
    };

    class crypterHandler extends Handler{
            @Override
            public void handleMessage(Message msg){
                    Log.d("IZAR","qt recieved message");
                    if (msg.what==DECRYPT_REQUEST_CODE){
                            try{
                                    Log.d("IZAR","got zofen?");
                                    zofen = msg.getData().getString(ZOFEN);
                            } catch (Exception e){
                                    e.printStackTrace();
                            }
                            return;
                    }
                    super.handleMessage(msg);
            }
    }
    private final Messenger mMessenger = new Messenger(new crypterHandler());

    public void sendZfnRequest(){
            Log.d("IZAR","client sending zfn request. bound? "+mBound);
            if (!mBound) return;

            Message msg= Message.obtain();
            Bundle bundle = new Bundle();
            String requestId= "zfnreq";
            bundle.putString("msgStr", requestId);

            msg.setData(bundle);

            try {
                    msg.replyTo = mMessenger;
                    mService.send(msg);
            } catch (RemoteException e) {
                    e.printStackTrace();
            }
    }

    public void doServiceConnect(){
            Intent remote = new Intent("org.Orayta.kukayta.paid.DeCrypter");
            connected = mActivity.bindService(remote, mConnection, 1);
            sendZfnRequest();
    }

    private void doServiceDisconnect(){
            if(mBound){
                    mActivity.unbindService(mConnection);
                    mBound= false;
            }
    }
	

}
