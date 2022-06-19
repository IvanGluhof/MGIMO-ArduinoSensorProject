package com.example.arduino_control;

import android.annotation.SuppressLint;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;

import com.google.android.material.floatingactionbutton.FloatingActionButton;
import com.google.android.material.snackbar.Snackbar;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.Toolbar;
import androidx.core.app.NotificationCompat;
import androidx.core.app.NotificationManagerCompat;

import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.Menu;
import android.view.MenuItem;
import android.view.autofill.AutofillValue;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.zip.CheckedOutputStream;

public class MainActivity extends AppCompatActivity {
    private static final float LIMIT_C = (float) 29.60; /* Temperature pseudo limit    */
    /* Necessary classes                                                               */
    ToggleButton    toggleBT_Connect; /* Button class                                  */
    BluetoothSocket BT_socket;        /* BluetoothSocket class to connect with Arduino */
    /* Incoming data objects */
    String          BT_received; /* Array to store data      */
    TextView        BT_data;     /* Text view to output data */
    /* Incoming data parse string */
    String          BT_C; /* Temperature C value string */
    String          BT_K; /* Temperature K value string */
    String          BT_F; /* Temperature F value string */
    String          BT_H; /* Humidity(%)   value string */
    /* Incoming data parse float */
    float          V_BT_C; /* Temperature C value float */
    float          V_BT_K; /* Temperature K value float */
    float          V_BT_F; /* Temperature F value float */
    float          V_BT_H; /* Humidity(%)   value float */
    /* Notifications                                                        */
    NotificationCompat.Builder builder; /* Builder for notification         */
    NotificationManagerCompat  notificationManager; /* Notification manager */
    /* Handler for notification                                             */
    private static final int WHAT = 1;          /* Handler do command       */
    private static final int TIME_TO_WAIT = 50; /* Handler wait time        */
    Handler regularHandler = new Handler(new Handler.Callback() {
        @SuppressLint("SetTextI18n")
        public boolean handleMessage(Message msg) {
            /* Fill text view (dynamic update) */
            if( BT_received != "" ) {
                BT_data.setText(BT_C + " " + BT_K + " " + BT_F + " " + BT_H );
            } else {
                BT_data.setText("No data");
            }
            /* Check pseudo temperature exceed and nortfy user*/
            if( V_BT_C > LIMIT_C ) {
                builder = new NotificationCompat.Builder(MainActivity.this, "60000")
                        .setSmallIcon(R.drawable.ic_launcher_foreground)
                        .setContentTitle("Temperature notification")
                        .setContentText("Warning! Huge temperature! These values are exceeding normal! " + BT_C + BT_K + BT_F + BT_H)
                        .setStyle(new NotificationCompat.BigTextStyle().bigText("These values are exceeding normal! " + BT_C + BT_K + BT_F + BT_H))
                        .setPriority(NotificationCompat.PRIORITY_MAX);

                notificationManager.notify(60001, builder.build());
            }

            regularHandler.sendEmptyMessageDelayed(msg.what, TIME_TO_WAIT);

            return true;
        }
    });

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        createNotificationChannel();
        notificationManager = NotificationManagerCompat.from(this);


        FloatingActionButton fab = findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Snackbar.make(view, "Replace with your own action", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
            }
        });


        BT_data = (TextView) findViewById(R.id.BT_data);
        /* Button setting */
        toggleBT_Connect = (ToggleButton) findViewById(R.id.toggleBT_Connect); /* Link class to my button */
        /* Set button listener     */
        toggleBT_Connect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

            }
        });

        /* This enables bluetooth (if enabled, nothing to do) */
        String enableBT = BluetoothAdapter.ACTION_REQUEST_ENABLE;
        startActivityForResult(new Intent(enableBT), 0);

        /* Use default bt adapter */
        BluetoothAdapter BT_adapter = BluetoothAdapter.getDefaultAdapter();
        if(BT_adapter!=null)
        {
            /* Bluetooth OK */
        } else {
            Log.d("BLUETOOTH", "ADAPTER ERROR");
            System.console().printf("Bluetooth error");
        }
        /* Common try-catch procedure */
        try {
            /* Create device class and connect to MAC address */
            String mac_address = "98:D3:71:FD:91:FC";
            BluetoothDevice device = BT_adapter.getRemoteDevice(mac_address);

            /* Initiate connection */
            Method m = device.getClass().getMethod("createRfcommSocket",
                                                    new Class[] {int.class});

            BT_socket = (BluetoothSocket) m.invoke(device,1);
            BT_socket.connect();
        /* Exception handling */
        } catch( IOException               e ) { Log.d("BLUETOOTH", e.getMessage());
        } catch( SecurityException         e ) { Log.d("BLUETOOTH", e.getMessage());
        } catch( NoSuchMethodException     e ) { Log.d("BLUETOOTH", e.getMessage());
        } catch( IllegalArgumentException  e ) { Log.d("BLUETOOTH", e.getMessage());
        } catch( IllegalAccessException    e ) { Log.d("BLUETOOTH", e.getMessage());
        } catch( InvocationTargetException e ) { Log.d("BLUETOOTH", e.getMessage());
        }

        /* Output successful connection message */
        Toast.makeText(getApplicationContext(), "CONNECTED", Toast.LENGTH_LONG).show();

        Toolbar toolbar = findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        /* Thread to handle data read */
        Thread thread_data_read = new Thread(null, backgroundThreadProcessing_ReadData, "Background");
        thread_data_read.start();
    }

    private void createNotificationChannel() {
        // Create the NotificationChannel, but only on API 26+ because
        // the NotificationChannel class is new and not in the support library
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            CharSequence  name = "Arduino channel";
            String description = "Arduino data channel";
            int importance = NotificationManager.IMPORTANCE_DEFAULT;
            NotificationChannel channel = new NotificationChannel("60000", name, importance);
            channel.setDescription(description);
            // Register the channel with the system; you can't change the importance
            // or other notification behaviors after this
            NotificationManager notificationManager = getSystemService(NotificationManager.class);
            notificationManager.createNotificationChannel(channel);
        }
    }


    @Override
    public void onResume() {
        super.onResume();
        regularHandler.sendEmptyMessageDelayed(WHAT, TIME_TO_WAIT);
    }

    @Override
    public void onPause() {
        super.onPause();
        regularHandler.removeMessages(WHAT);
    }

    /* Runnable object that launches background thread */
    private Runnable backgroundThreadProcessing_ReadData = new Runnable() {
        public void run() {
        try                   { backgroundProcessing_ReadData(); }
        catch (IOException e) { e.printStackTrace();             }
        }
    };

    /* Method to receive data in separate thread */
    private void backgroundProcessing_ReadData() throws IOException {
        InputStream inputStream = null; /* Input stream to receive data */
        byte[] buffer = new byte[1024]; /* Input buffer                 */
        byte[] zero   = new byte[1024]; /* Zero  buffer                 */
        int length = 0;                 /* Number of bytes read         */

        /* Try open socket */
        try                   { inputStream = BT_socket.getInputStream(); }
        catch (IOException e) { e.printStackTrace();                      }
        /* Try read some data */
        try                   { length = inputStream.read(buffer); }
        catch (IOException e) { e.printStackTrace();               }

        /* Init data read - read continuously in background */
        while ((length = inputStream.read(buffer)) != -1) {
            /* Concat incoming data */
            BT_received += new String(buffer, 0, length);
            /* ;; = message end */
            if( BT_received.contains(";;") ) {
                /* Parse string */
                String[] Values = BT_received.replace(";;", "").split(":");
                BT_C = Values[0];
                BT_K = Values[1];
                BT_F = Values[2];
                BT_H = Values[3];
                /* Convert received vauues to float */
                try {
                    V_BT_C = Float.parseFloat(BT_C.replaceAll("[^0-9.]", ""));
                    V_BT_K = Float.parseFloat(BT_K.replaceAll("[^0-9.]", ""));
                    V_BT_F = Float.parseFloat(BT_F.replaceAll("[^0-9.]", ""));
                    V_BT_H = Float.parseFloat(BT_H.replaceAll("[^0-9.]", ""));
                } catch( NumberFormatException ex ) {
                    Log.d("BT_ERR", ex.getMessage());
                }
                /* DEBUG log */
                Log.d("BLUETOOTH", BT_C );
                Log.d("BLUETOOTH", BT_K );
                Log.d("BLUETOOTH", BT_F );
                Log.d("BLUETOOTH", BT_H );
                /* Set buffer to zero */
                BT_received = new String(zero, 0, 0);
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
