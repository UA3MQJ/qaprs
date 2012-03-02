/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */ 

//версия с работы

package jAPRS;

import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

import javax.microedition.lcdui.Display;

import javax.microedition.io.file.FileConnection;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.DataOutputStream;

import javax.microedition.io.Connector;
import javax.microedition.io.SocketConnection;

import java.util.*;

import java.io.*;

import javax.microedition.rms.*;
import org.netbeans.microedition.lcdui.WaitScreen;
import org.netbeans.microedition.lcdui.pda.FileBrowser;
import org.netbeans.microedition.util.SimpleCancellableTask;

/**
 * @author user
 */


public class jAPRS extends MIDlet implements Runnable, CommandListener {

    private boolean midletPaused = false;

    static final String DBNAME = "APRSPS";
    static final String APRSCALL = "APRS";
    RecordStore recordStore = null;

    public Thread th;

    Timer       timer = new Timer();
    MyTimerTask ttask = new MyTimerTask();
    

  //<SERVER1>aprswest.aprs2.net:14580</SERVER1>
  //<SERVER2>argentina.aprs2.net:14580</SERVER2>
  //<SERVER3>australia.aprs2.net:14580</SERVER3>
  //<filter>p/ISS/R/U/LY/YL/ES/EU/EW/ER/4X/4Z/</filter>
    String APRS_SERVER = new String("russia.aprs2.net");
    String APRS_PORT   = new String("14580");
    String APRS_USER   = new String("CALL");//UA3MQJ //CALL
    String APRS_PASS   = new String("-1");//17572 //-1
    //String APRS_FILTER = new String("p/ISS/R/U/LY/YL/ES/EU/EW/ER/4X/4Z/");
    String APRS_FILTER = new String("/");

    String APRS_STATION_NAME = new String("STCALL"); //STCALL
    String APRS_STATION_SYM  = new String("/I");

    String APRS_BEACON_PERIOD  = new String("1800"); //1800 sec = 30 min
    public int    timerCounter = 1800; //обратный таймер

    String lastPosition  = new String("? ?");
    String lastStatus    = new String("?");

    String log           = new String("");
    String Messages      = new String("");


    SocketConnection sc = null;
    OutputStream     os = null;
    InputStream      is = null;
    boolean          SRVConnected = false; //признак наличия соединения с сервером





    //<editor-fold defaultstate="collapsed" desc=" Generated Fields ">//GEN-BEGIN:|fields|0|
    private Command exitCommand;
    private Command itemCommand;
    private Command itemCommand1;
    private Command itemCommand2;
    private Command itemCommand3;
    private Command itemCommand4;
    private Command itemCommand5;
    private Command okCommand;
    private Command exitCommand1;
    private Command No;
    private Command Yes;
    private Command itemCommand6;
    private Command stopCommand;
    private Command cancelCommand;
    private Command okCommand1;
    private Command okCommand2;
    private Command cancelCommand1;
    private Command itemCommand7;
    private Command itemCommand8;
    private Command cancelCommand2;
    private Command itemCommand9;
    private Command itemCommand10;
    private Command exitCommand2;
    private Command okCommand3;
    private Form form;
    private Spacer spacer5;
    private Spacer spacer2;
    private TextField textField14;
    private TextField textField12;
    private TextField textField13;
    private TextField textField10;
    private TextField textField11;
    private TextField textField16;
    private TextBox textBox;
    private FileBrowser fileBrowser;
    private WaitScreen waitScreen;
    private List list;
    private WaitScreen waitScreen1;
    private Form form1;
    private ChoiceGroup choiceGroup;
    private TextField textField1;
    private TextField textField;
    private TextField textField15;
    private Form form2;
    private TextField textField2;
    private TextField textField3;
    private TextField textField4;
    private TextField textField5;
    private TextField textField6;
    private List list1;
    private WaitScreen waitScreen2;
    private Form form3;
    private TextField textField7;
    private TextField textField9;
    private TextField textField8;
    private TextBox textBox1;
    private SimpleCancellableTask task;
    private SimpleCancellableTask task1;
    private SimpleCancellableTask task2;
    //</editor-fold>//GEN-END:|fields|0|

    /**
     * The HelloMIDlet constructor.
     */
    public jAPRS() {

    }



    class MyTimerTask extends TimerTask {
        public void run(){
            //System.out.println( "Передача координат по таймеру" );
            //readFile();
            timerCounter = timerCounter - 1;
            System.out.println( timerCounter );

            textField16.setString( Integer.toString( timerCounter ) );

            if ( timerCounter == 0 ) {

                System.out.println( "Передача координат по таймеру" );
                readFile();

                timerCounter =  Integer.parseInt(textField15.getString());


            };

        }
    }

    //<editor-fold defaultstate="collapsed" desc=" Generated Methods ">//GEN-BEGIN:|methods|0|
    //</editor-fold>//GEN-END:|methods|0|

    //<editor-fold defaultstate="collapsed" desc=" Generated Method: initialize ">//GEN-BEGIN:|0-initialize|0|0-preInitialize
    /**
     * Initilizes the application.
     * It is called only once when the MIDlet is started. The method is called before the <code>startMIDlet</code> method.
     */
    private void initialize() {//GEN-END:|0-initialize|0|0-preInitialize
        // write pre-initialize user code here
//GEN-LINE:|0-initialize|1|0-postInitialize
        // write post-initialize user code here
// Открываем хранилище данных

        log = log + "Initial\n";
        //if (textBox != null) { textBox.setString( log ); }

        boolean storeNotExist = true;

        String[] names = RecordStore.listRecordStores();
        //log = log + "Store names:\n";
        for( int i = 0;
             names != null && i < names.length;
             ++i ) {
            log = log + "Name - " + names[i] + "\n";
            if ( names[i].equalsIgnoreCase( "APRSPS" ) ) { storeNotExist = false; };
        }

        if ( storeNotExist ) {
            log = log + "Store APRSPS not exist\n";
        } else {
            log = log + "Store APRSPS exist\n";
        }

        try {
              recordStore = RecordStore.openRecordStore( "APRSPS" , storeNotExist);

              log = log + "recordStore.getNextRecordID()=" + recordStore.getNextRecordID() + "\n";
              log = log + "recordStore.getNumRecords()=" + recordStore.getNumRecords() + "\n";

              if( recordStore.getNextRecordID() != 1){
                    // проверка хранилища на наличие записей. если их там нет, то и читать нечего
                  System.out.println("Start - Data in RMS");
                  log = log + "Start - Data in RMS\n";
                  //if (textBox != null) { textBox.setString( log ); }

                  byte[] record = recordStore.getRecord(1);

                  ByteArrayInputStream bais = new ByteArrayInputStream(record);

                  DataInputStream dis = new DataInputStream(bais);

                  APRS_SERVER        = dis.readUTF();
                  APRS_PORT          = dis.readUTF();
                  APRS_USER          = dis.readUTF();
                  APRS_PASS          = dis.readUTF();
                  APRS_FILTER        = dis.readUTF();
                  APRS_STATION_NAME  = dis.readUTF();
                  APRS_STATION_SYM   = dis.readUTF();
                  APRS_BEACON_PERIOD = dis.readUTF();
                  timerCounter = Integer.parseInt(APRS_BEACON_PERIOD);

            } else {
                System.out.println("Start - No data in RMS");
                log = log + "Start - No data in RMS\n";
                //if (textBox != null) { textBox.setString( log ); }

                try {

                              //recordStore = RecordStore.openRecordStore(DBNAME, true);

                              ByteArrayOutputStream baos = new ByteArrayOutputStream();

                              DataOutputStream dos = new DataOutputStream(baos);

                              //dos.writeUTF(stringForWrite);
                              dos.writeUTF(APRS_SERVER);
                              dos.writeUTF(APRS_PORT);
                              dos.writeUTF(APRS_USER);
                              dos.writeUTF(APRS_PASS);
                              dos.writeUTF(APRS_FILTER);
                              dos.writeUTF(APRS_STATION_NAME);
                              dos.writeUTF(APRS_STATION_SYM);
                              dos.writeUTF(APRS_BEACON_PERIOD);


                              byte[] record = baos.toByteArray();

                              if( recordStore.getNextRecordID() == 1 ){
                                    // проверка хранилища на наличие записей. если их там нет, то и читать нечего
                                  System.out.println("No Data in RMS - insert");
                                    log = log + "No Data in RMS - insert\n";
                                    //if (textBox != null) { textBox.setString( log ); }

                                  int    recordID;

                                  recordID = recordStore.addRecord(record, 0, record.length);
                                  log = log + "New record id = " + recordID + "\n";


                              } else {
                                  System.out.println("Data in RMS - update");
                                    log = log + "Data in RMS - update\n";
                                    //if (textBox != null) { textBox.setString( log ); }


                                  recordStore.setRecord( 1, record, 0, record.length );


                              }

                          } catch (Exception e) {

                              System.out.println("Stop - Exception: " + e);
                                    log = log + "Stop - Exception: " + e + "\n";
                                    //if (textBox != null) { textBox.setString( log ); }

                          }

            }

         } catch (Exception e) {

              System.out.println("Exception in loading data: " + e);
              log = log + "Exception in loading data: " + e + "\n";
              //if (textBox != null) { textBox.setString( log ); }

         }

         System.gc();

        getTextBox1();

        //запуск процесса
        th = new Thread( this );
        th.start();

    }//GEN-BEGIN:|0-initialize|2|
    //</editor-fold>//GEN-END:|0-initialize|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Method: startMIDlet ">//GEN-BEGIN:|3-startMIDlet|0|3-preAction
    /**
     * Performs an action assigned to the Mobile Device - MIDlet Started point.
     */
    public void startMIDlet() {//GEN-END:|3-startMIDlet|0|3-preAction
        // write pre-action user code here
        switchDisplayable(null, getForm());//GEN-LINE:|3-startMIDlet|1|3-postAction
        // write post-action user code here
        screenUpdate();
    }//GEN-BEGIN:|3-startMIDlet|2|
    //</editor-fold>//GEN-END:|3-startMIDlet|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Method: resumeMIDlet ">//GEN-BEGIN:|4-resumeMIDlet|0|4-preAction
    /**
     * Performs an action assigned to the Mobile Device - MIDlet Resumed point.
     */
    public void resumeMIDlet() {//GEN-END:|4-resumeMIDlet|0|4-preAction
        // write pre-action user code here
//GEN-LINE:|4-resumeMIDlet|1|4-postAction
        // write post-action user code here
    }//GEN-BEGIN:|4-resumeMIDlet|2|
    //</editor-fold>//GEN-END:|4-resumeMIDlet|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Method: switchDisplayable ">//GEN-BEGIN:|5-switchDisplayable|0|5-preSwitch
    /**
     * Switches a current displayable in a display. The <code>display</code> instance is taken from <code>getDisplay</code> method. This method is used by all actions in the design for switching displayable.
     * @param alert the Alert which is temporarily set to the display; if <code>null</code>, then <code>nextDisplayable</code> is set immediately
     * @param nextDisplayable the Displayable to be set
     */
    public void switchDisplayable(Alert alert, Displayable nextDisplayable) {//GEN-END:|5-switchDisplayable|0|5-preSwitch
        // write pre-switch user code here
        Display display = getDisplay();//GEN-BEGIN:|5-switchDisplayable|1|5-postSwitch
        if (alert == null) {
            display.setCurrent(nextDisplayable);
        } else {
            display.setCurrent(alert, nextDisplayable);
        }//GEN-END:|5-switchDisplayable|1|5-postSwitch
        // write post-switch user code here
    }//GEN-BEGIN:|5-switchDisplayable|2|
    //</editor-fold>//GEN-END:|5-switchDisplayable|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Method: commandAction for Displayables ">//GEN-BEGIN:|7-commandAction|0|7-preCommandAction
    /**
     * Called by a system to indicated that a command has been invoked on a particular displayable.
     * @param command the Command that was invoked
     * @param displayable the Displayable where the command was invoked
     */
    public void commandAction(Command command, Displayable displayable) {//GEN-END:|7-commandAction|0|7-preCommandAction
        // write pre-action user code here
        if (displayable == fileBrowser) {//GEN-BEGIN:|7-commandAction|1|45-preAction
            if (command == FileBrowser.SELECT_FILE_COMMAND) {//GEN-END:|7-commandAction|1|45-preAction
                // write pre-action user code here
                switchDisplayable(null, getList());//GEN-LINE:|7-commandAction|2|45-postAction
                // write post-action user code here
            } else if (command == exitCommand) {//GEN-LINE:|7-commandAction|3|47-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|4|47-postAction
                // write post-action user code here
                screenUpdate();
            }//GEN-BEGIN:|7-commandAction|5|19-preAction
        } else if (displayable == form) {
            if (command == exitCommand) {//GEN-END:|7-commandAction|5|19-preAction
                // write pre-action user code here
                switchDisplayable(null, getList1());//GEN-LINE:|7-commandAction|6|19-postAction
                // write post-action user code here
            } else if (command == itemCommand) {//GEN-LINE:|7-commandAction|7|23-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm2());//GEN-LINE:|7-commandAction|8|23-postAction
                // write post-action user code here
                textField2.setString( APRS_SERVER );
                textField3.setString( APRS_PORT );
                textField4.setString( APRS_USER );
                textField5.setString( APRS_PASS );
                textField6.setString( APRS_FILTER );
                textField15.setString( APRS_BEACON_PERIOD );

            } else if (command == itemCommand1) {//GEN-LINE:|7-commandAction|9|25-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm1());//GEN-LINE:|7-commandAction|10|25-postAction
                // write post-action user code here
                textField.setString( APRS_STATION_NAME );
                textField1.setString( APRS_STATION_SYM );
                textField15.setString( APRS_BEACON_PERIOD );
                if ( APRS_STATION_SYM.equals ("/I" ) ) {
                     choiceGroup.setSelectedFlags(new boolean[] { true, false, false, false, false, false });
                } else if ( APRS_STATION_SYM.equals ("/[") ) {
                     choiceGroup.setSelectedFlags(new boolean[] { false, true, false, false, false, false });
                } else if ( APRS_STATION_SYM.equals ("/>") ) {
                     choiceGroup.setSelectedFlags(new boolean[] { false, false, true, false, false, false });
                } else if ( APRS_STATION_SYM.equals ("/U") ) {
                     choiceGroup.setSelectedFlags(new boolean[] { false, false, false, true, false, false });
                } else if ( APRS_STATION_SYM.equals ("/=") ) {
                     choiceGroup.setSelectedFlags(new boolean[] { false, false, false, false, true, false });
                } else {
                     choiceGroup.setSelectedFlags(new boolean[] { false, false, false, false, false, true });
                }
            } else if (command == itemCommand2) {//GEN-LINE:|7-commandAction|11|27-preAction
                // write pre-action user code here
                switchDisplayable(null, getWaitScreen1());//GEN-LINE:|7-commandAction|12|27-postAction
                // write post-action user code here
            } else if (command == itemCommand3) {//GEN-LINE:|7-commandAction|13|29-preAction
                // write pre-action user code here
                switchDisplayable(null, getFileBrowser());//GEN-LINE:|7-commandAction|14|29-postAction
                // write post-action user code here
            } else if (command == itemCommand4) {//GEN-LINE:|7-commandAction|15|31-preAction
                // write pre-action user code here
                switchDisplayable(null, getTextBox());//GEN-LINE:|7-commandAction|16|31-postAction
                // write post-action user code here
            } else if (command == itemCommand5) {//GEN-LINE:|7-commandAction|17|33-preAction
                // write pre-action user code here
                switchDisplayable(null, getList1());//GEN-LINE:|7-commandAction|18|33-postAction
                // write post-action user code here
            } else if (command == itemCommand6) {//GEN-LINE:|7-commandAction|19|160-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm3());//GEN-LINE:|7-commandAction|20|160-postAction
                // write post-action user code here
                textField8.setString( APRS_STATION_NAME );
                textField9.setString( "" );
            } else if (command == itemCommand9) {//GEN-LINE:|7-commandAction|21|188-preAction
                // write pre-action user code here
                switchDisplayable(null, getTextBox1());//GEN-LINE:|7-commandAction|22|188-postAction
                // write post-action user code here
            }//GEN-BEGIN:|7-commandAction|23|124-preAction
        } else if (displayable == form1) {
            if (command == cancelCommand) {//GEN-END:|7-commandAction|23|124-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|24|124-postAction
                // write post-action user code here
            } else if (command == okCommand1) {//GEN-LINE:|7-commandAction|25|122-preAction
                // write pre-action user code here
                APRS_STATION_NAME = textField.getString().toUpperCase();
                APRS_BEACON_PERIOD = textField15.getString().toUpperCase();
                timerCounter =  Integer.parseInt(textField15.getString());
                textField16.setString( Integer.toString( timerCounter ) );

                int sInd = choiceGroup.getSelectedIndex();
                switch (sInd) {
                    case 0: APRS_STATION_SYM = "/I";break;
                    case 1: APRS_STATION_SYM = "/[";break;
                    case 2: APRS_STATION_SYM = "/>";break;
                    case 3: APRS_STATION_SYM = "/U";break;
                    case 4: APRS_STATION_SYM = "/=";break;
                    case 5: APRS_STATION_SYM = textField1.getString();
                }
                screenUpdate();

                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|26|122-postAction
                // write post-action user code here
            }//GEN-BEGIN:|7-commandAction|27|140-preAction
        } else if (displayable == form2) {
            if (command == cancelCommand1) {//GEN-END:|7-commandAction|27|140-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|28|140-postAction
                // write post-action user code here
            } else if (command == okCommand2) {//GEN-LINE:|7-commandAction|29|138-preAction
                // write pre-action user code here
                APRS_SERVER = textField2.getString();
                APRS_PORT   = textField3.getString();
                APRS_USER   = textField4.getString().toUpperCase();
                APRS_PASS   = textField5.getString();
                APRS_FILTER = textField6.getString();



                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|30|138-postAction
                // write post-action user code here
                screenUpdate();
            }//GEN-BEGIN:|7-commandAction|31|168-preAction
        } else if (displayable == form3) {
            if (command == cancelCommand2) {//GEN-END:|7-commandAction|31|168-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|32|168-postAction
                // write post-action user code here
            } else if (command == itemCommand7) {//GEN-LINE:|7-commandAction|33|171-preAction
                // write pre-action user code here
                switchDisplayable(null, getWaitScreen2());//GEN-LINE:|7-commandAction|34|171-postAction
                // write post-action user code here
            }//GEN-BEGIN:|7-commandAction|35|74-preAction
        } else if (displayable == list) {
            if (command == List.SELECT_COMMAND) {//GEN-END:|7-commandAction|35|74-preAction
                // write pre-action user code here
                listAction();//GEN-LINE:|7-commandAction|36|74-postAction
                // write post-action user code here
            }//GEN-BEGIN:|7-commandAction|37|152-preAction
        } else if (displayable == list1) {
            if (command == List.SELECT_COMMAND) {//GEN-END:|7-commandAction|37|152-preAction
                // write pre-action user code here
                list1Action();//GEN-LINE:|7-commandAction|38|152-postAction
                // write post-action user code here
            }//GEN-BEGIN:|7-commandAction|39|38-preAction
        } else if (displayable == textBox) {
            if (command == exitCommand1) {//GEN-END:|7-commandAction|39|38-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|40|38-postAction
                // write post-action user code here
            } else if (command == okCommand) {//GEN-LINE:|7-commandAction|41|40-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|42|40-postAction
                // write post-action user code here
            }//GEN-BEGIN:|7-commandAction|43|193-preAction
        } else if (displayable == textBox1) {
            if (command == exitCommand2) {//GEN-END:|7-commandAction|43|193-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|44|193-postAction
                // write post-action user code here
            } else if (command == itemCommand10) {//GEN-LINE:|7-commandAction|45|200-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm3());//GEN-LINE:|7-commandAction|46|200-postAction
                // write post-action user code here
                textField8.setString( APRS_STATION_NAME );
                textField9.setString( "" );
            }//GEN-BEGIN:|7-commandAction|47|65-preAction
        } else if (displayable == waitScreen) {
            if (command == WaitScreen.FAILURE_COMMAND) {//GEN-END:|7-commandAction|47|65-preAction
                // write pre-action user code here
                switchDisplayable(null, getFileBrowser());//GEN-LINE:|7-commandAction|48|65-postAction
                // write post-action user code here
            } else if (command == WaitScreen.SUCCESS_COMMAND) {//GEN-LINE:|7-commandAction|49|64-preAction
                // write pre-action user code here
                switchDisplayable(null, getFileBrowser());//GEN-LINE:|7-commandAction|50|64-postAction
                // write post-action user code here
            }//GEN-BEGIN:|7-commandAction|51|105-preAction
        } else if (displayable == waitScreen1) {
            if (command == WaitScreen.FAILURE_COMMAND) {//GEN-END:|7-commandAction|51|105-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|52|105-postAction
                // write post-action user code here
            } else if (command == WaitScreen.SUCCESS_COMMAND) {//GEN-LINE:|7-commandAction|53|104-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|54|104-postAction
                // write post-action user code here
            }//GEN-BEGIN:|7-commandAction|55|177-preAction
        } else if (displayable == waitScreen2) {
            if (command == WaitScreen.FAILURE_COMMAND) {//GEN-END:|7-commandAction|55|177-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|7-commandAction|56|177-postAction
                // write post-action user code here
            } else if (command == WaitScreen.SUCCESS_COMMAND) {//GEN-LINE:|7-commandAction|57|176-preAction
                // write pre-action user code here
                switchDisplayable(null, getTextBox1());//GEN-LINE:|7-commandAction|58|176-postAction
                // write post-action user code here
            }//GEN-BEGIN:|7-commandAction|59|7-postCommandAction
        }//GEN-END:|7-commandAction|59|7-postCommandAction
        // write post-action user code here
    }//GEN-BEGIN:|7-commandAction|60|
    //</editor-fold>//GEN-END:|7-commandAction|60|



    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: exitCommand ">//GEN-BEGIN:|18-getter|0|18-preInit
    /**
     * Returns an initiliazed instance of exitCommand component.
     * @return the initialized component instance
     */
    public Command getExitCommand() {
        if (exitCommand == null) {//GEN-END:|18-getter|0|18-preInit
            // write pre-init user code here
            exitCommand = new Command("Exit", Command.EXIT, 0);//GEN-LINE:|18-getter|1|18-postInit
            // write post-init user code here
        }//GEN-BEGIN:|18-getter|2|
        return exitCommand;
    }
    //</editor-fold>//GEN-END:|18-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: form ">//GEN-BEGIN:|14-getter|0|14-preInit
    /**
     * Returns an initiliazed instance of form component.
     * @return the initialized component instance
     */
    public Form getForm() {
        if (form == null) {//GEN-END:|14-getter|0|14-preInit
            // write pre-init user code here
            form = new Form("APRS Inet Sender", new Item[] { getSpacer2(), getTextField10(), getTextField12(), getTextField11(), getSpacer5(), getTextField13(), getTextField14(), getTextField16() });//GEN-BEGIN:|14-getter|1|14-postInit
            form.addCommand(getExitCommand());
            form.addCommand(getItemCommand());
            form.addCommand(getItemCommand1());
            form.addCommand(getItemCommand2());
            form.addCommand(getItemCommand3());
            form.addCommand(getItemCommand6());
            form.addCommand(getItemCommand9());
            form.addCommand(getItemCommand4());
            form.addCommand(getItemCommand5());
            form.setCommandListener(this);//GEN-END:|14-getter|1|14-postInit
            // write post-init user code here
        }//GEN-BEGIN:|14-getter|2|
        return form;
    }
    //</editor-fold>//GEN-END:|14-getter|2|



    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand ">//GEN-BEGIN:|22-getter|0|22-preInit
    /**
     * Returns an initiliazed instance of itemCommand component.
     * @return the initialized component instance
     */
    public Command getItemCommand() {
        if (itemCommand == null) {//GEN-END:|22-getter|0|22-preInit
            // write pre-init user code here
            itemCommand = new Command("Connection Pars", Command.ITEM, 0);//GEN-LINE:|22-getter|1|22-postInit
            // write post-init user code here
        }//GEN-BEGIN:|22-getter|2|
        return itemCommand;
    }
    //</editor-fold>//GEN-END:|22-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand1 ">//GEN-BEGIN:|24-getter|0|24-preInit
    /**
     * Returns an initiliazed instance of itemCommand1 component.
     * @return the initialized component instance
     */
    public Command getItemCommand1() {
        if (itemCommand1 == null) {//GEN-END:|24-getter|0|24-preInit
            // write pre-init user code here
            itemCommand1 = new Command("Station Pars", Command.ITEM, 0);//GEN-LINE:|24-getter|1|24-postInit
            // write post-init user code here
        }//GEN-BEGIN:|24-getter|2|
        return itemCommand1;
    }
    //</editor-fold>//GEN-END:|24-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand2 ">//GEN-BEGIN:|26-getter|0|26-preInit
    /**
     * Returns an initiliazed instance of itemCommand2 component.
     * @return the initialized component instance
     */
    public Command getItemCommand2() {
        if (itemCommand2 == null) {//GEN-END:|26-getter|0|26-preInit
            // write pre-init user code here
            itemCommand2 = new Command("Conn/Disconnect", Command.ITEM, 0);//GEN-LINE:|26-getter|1|26-postInit
            // write post-init user code here
        }//GEN-BEGIN:|26-getter|2|
        return itemCommand2;
    }
    //</editor-fold>//GEN-END:|26-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand3 ">//GEN-BEGIN:|28-getter|0|28-preInit
    /**
     * Returns an initiliazed instance of itemCommand3 component.
     * @return the initialized component instance
     */
    public Command getItemCommand3() {
        if (itemCommand3 == null) {//GEN-END:|28-getter|0|28-preInit
            // write pre-init user code here
            itemCommand3 = new Command("Send Position", Command.ITEM, 0);//GEN-LINE:|28-getter|1|28-postInit
            // write post-init user code here
        }//GEN-BEGIN:|28-getter|2|
        return itemCommand3;
    }
    //</editor-fold>//GEN-END:|28-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand4 ">//GEN-BEGIN:|30-getter|0|30-preInit
    /**
     * Returns an initiliazed instance of itemCommand4 component.
     * @return the initialized component instance
     */
    public Command getItemCommand4() {
        if (itemCommand4 == null) {//GEN-END:|30-getter|0|30-preInit
            // write pre-init user code here
            itemCommand4 = new Command("Help", Command.ITEM, 0);//GEN-LINE:|30-getter|1|30-postInit
            // write post-init user code here
        }//GEN-BEGIN:|30-getter|2|
        return itemCommand4;
    }
    //</editor-fold>//GEN-END:|30-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand5 ">//GEN-BEGIN:|32-getter|0|32-preInit
    /**
     * Returns an initiliazed instance of itemCommand5 component.
     * @return the initialized component instance
     */
    public Command getItemCommand5() {
        if (itemCommand5 == null) {//GEN-END:|32-getter|0|32-preInit
            // write pre-init user code here
            itemCommand5 = new Command("Exit", Command.ITEM, 0);//GEN-LINE:|32-getter|1|32-postInit
            // write post-init user code here
        }//GEN-BEGIN:|32-getter|2|
        return itemCommand5;
    }
    //</editor-fold>//GEN-END:|32-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: exitCommand1 ">//GEN-BEGIN:|37-getter|0|37-preInit
    /**
     * Returns an initiliazed instance of exitCommand1 component.
     * @return the initialized component instance
     */
    public Command getExitCommand1() {
        if (exitCommand1 == null) {//GEN-END:|37-getter|0|37-preInit
            // write pre-init user code here
            exitCommand1 = new Command("Exit", Command.EXIT, 0);//GEN-LINE:|37-getter|1|37-postInit
            // write post-init user code here
        }//GEN-BEGIN:|37-getter|2|
        return exitCommand1;
    }
    //</editor-fold>//GEN-END:|37-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: okCommand ">//GEN-BEGIN:|39-getter|0|39-preInit
    /**
     * Returns an initiliazed instance of okCommand component.
     * @return the initialized component instance
     */
    public Command getOkCommand() {
        if (okCommand == null) {//GEN-END:|39-getter|0|39-preInit
            // write pre-init user code here
            okCommand = new Command("Ok", Command.OK, 0);//GEN-LINE:|39-getter|1|39-postInit
            // write post-init user code here
        }//GEN-BEGIN:|39-getter|2|
        return okCommand;
    }
    //</editor-fold>//GEN-END:|39-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textBox ">//GEN-BEGIN:|35-getter|0|35-preInit
    /**
     * Returns an initiliazed instance of textBox component.
     * @return the initialized component instance
     */
    public TextBox getTextBox() {
        if (textBox == null) {//GEN-END:|35-getter|0|35-preInit
            // write pre-init user code here
            textBox = new TextBox("Help", "\u041F\u0440\u043E\u0433\u0440\u0430\u043C\u043C\u0430 \u043F\u0440\u0435\u0434\u043D\u0430\u0437\u043D\u0430\u0447\u0435\u043D\u0430 \u0434\u043B\u044F \u043E\u0442\u043F\u0440\u0430\u0432\u043A\u0438 \u0441\u0432\u043E\u0435\u0433\u043E \u043C\u0435\u0441\u0442\u043E\u043F\u043E\u043B\u043E\u0436\u0435\u043D\u0438\u044F \u0432 \u043D\u0443\u0436\u043D\u044B\u0439 \u043C\u043E\u043C\u0435\u043D\u0442 \u043F\u043E \u0437\u0430\u0434\u0430\u043D\u043D\u044B\u043C \u0437\u0430\u0440\u0430\u043D\u0435\u0435 \u043A\u043E\u043E\u0440\u0434\u0438\u043D\u0430\u0442\u0430\u043C.\n\u0412\u044B\u0431\u0435\u0440\u0438\u0442\u0435 \u0442\u0435\u043A\u0441\u0442\u043E\u0432\u044B\u0439 \u0444\u0430\u0439\u043B \u0432 \u0432\u0430\u0448\u0435\u0439 \u0444\u0430\u0439\u043B\u043E\u0432\u043E\u0439 \u0441\u0438\u0441\u0442\u0435\u043C\u0435. \u0424\u043E\u0440\u043C\u0430\u0442 \u0444\u0430\u0439\u043B\u0430:\n\u0428\u0438\u0440\u043E\u0442\u0430 \u0414\u043E\u043B\u0433\u043E\u0442\u0430 \u041A\u043E\u043C\u043C\u0435\u043D\u0442\u0430\u0440\u0438\u0439 \u043C\u0430\u044F\u043A\u0430.\n\u041F\u0440\u0438\u043C\u0435\u0440: 58.01.83N 038.51.13E Fly e135 - inet \"At home\"{jAPRS}\n\nThe program is designed to send its location at the right time to pre-set coordinates.\nSelect the text file on your file system. File Format:\nLatitude Longitude Comments lighthouse.\nExample: 58.01.83N 038.51.13E Fly e135 - inet \"At home\"{jAPRS}", 1000, TextField.ANY | TextField.UNEDITABLE);//GEN-BEGIN:|35-getter|1|35-postInit
            textBox.addCommand(getExitCommand1());
            textBox.addCommand(getOkCommand());
            textBox.setCommandListener(this);//GEN-END:|35-getter|1|35-postInit
            // write post-init user code here
            //textBox.setString( log );
        }//GEN-BEGIN:|35-getter|2|
        return textBox;
    }
    //</editor-fold>//GEN-END:|35-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: Yes ">//GEN-BEGIN:|53-getter|0|53-preInit
    /**
     * Returns an initiliazed instance of Yes component.
     * @return the initialized component instance
     */
    public Command getYes() {
        if (Yes == null) {//GEN-END:|53-getter|0|53-preInit
            // write pre-init user code here
            Yes = new Command("Item", Command.ITEM, 0);//GEN-LINE:|53-getter|1|53-postInit
            // write post-init user code here
        }//GEN-BEGIN:|53-getter|2|
        return Yes;
    }
    //</editor-fold>//GEN-END:|53-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: No ">//GEN-BEGIN:|55-getter|0|55-preInit
    /**
     * Returns an initiliazed instance of No component.
     * @return the initialized component instance
     */
    public Command getNo() {
        if (No == null) {//GEN-END:|55-getter|0|55-preInit
            // write pre-init user code here
            No = new Command("Item", Command.ITEM, 0);//GEN-LINE:|55-getter|1|55-postInit
            // write post-init user code here
        }//GEN-BEGIN:|55-getter|2|
        return No;
    }
    //</editor-fold>//GEN-END:|55-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: fileBrowser ">//GEN-BEGIN:|43-getter|0|43-preInit
    /**
     * Returns an initiliazed instance of fileBrowser component.
     * @return the initialized component instance
     */
    public FileBrowser getFileBrowser() {
        if (fileBrowser == null) {//GEN-END:|43-getter|0|43-preInit
            // write pre-init user code here
            fileBrowser = new FileBrowser(getDisplay());//GEN-BEGIN:|43-getter|1|43-postInit
            fileBrowser.setTitle("fileBrowser");
            fileBrowser.setCommandListener(this);
            fileBrowser.addCommand(FileBrowser.SELECT_FILE_COMMAND);
            fileBrowser.addCommand(getExitCommand());//GEN-END:|43-getter|1|43-postInit
            // write post-init user code here
        }//GEN-BEGIN:|43-getter|2|
        return fileBrowser;
    }
    //</editor-fold>//GEN-END:|43-getter|2|





    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: waitScreen ">//GEN-BEGIN:|61-getter|0|61-preInit
    /**
     * Returns an initiliazed instance of waitScreen component.
     * @return the initialized component instance
     */
    public WaitScreen getWaitScreen() {
        if (waitScreen == null) {//GEN-END:|61-getter|0|61-preInit
            // write pre-init user code here
            waitScreen = new WaitScreen(getDisplay());//GEN-BEGIN:|61-getter|1|61-postInit
            waitScreen.setTitle("Sending position");
            waitScreen.setCommandListener(this);
            waitScreen.setText("Wait...");
            waitScreen.setTask(getTask());//GEN-END:|61-getter|1|61-postInit
            // write post-init user code here
        }//GEN-BEGIN:|61-getter|2|
        return waitScreen;
    }
    //</editor-fold>//GEN-END:|61-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: task ">//GEN-BEGIN:|66-getter|0|66-preInit
    /**
     * Returns an initiliazed instance of task component.
     * @return the initialized component instance
     */
    public SimpleCancellableTask getTask() {
        if (task == null) {//GEN-END:|66-getter|0|66-preInit
            // write pre-init user code here
            task = new SimpleCancellableTask();//GEN-BEGIN:|66-getter|1|66-execute
            task.setExecutable(new org.netbeans.microedition.util.Executable() {
                public void execute() throws Exception {//GEN-END:|66-getter|1|66-execute
                    // write task-execution user code here
                    System.out.println("Send Position");
                    //запуск таймера до следующего запуска
                    if ( timerCounter>0 ) {
                        System.out.println("Start timer");
                        timer.schedule( ttask, 1000, 1000 );
                    };

                    readFile();
                    System.out.println("Position sended");

                }//GEN-BEGIN:|66-getter|2|66-postInit
            });//GEN-END:|66-getter|2|66-postInit
            // write post-init user code here
        }//GEN-BEGIN:|66-getter|3|
        return task;
    }
    //</editor-fold>//GEN-END:|66-getter|3|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand6 ">//GEN-BEGIN:|71-getter|0|71-preInit
    /**
     * Returns an initiliazed instance of itemCommand6 component.
     * @return the initialized component instance
     */
    public Command getItemCommand6() {
        if (itemCommand6 == null) {//GEN-END:|71-getter|0|71-preInit
            // write pre-init user code here
            itemCommand6 = new Command("Send Message", Command.ITEM, 0);//GEN-LINE:|71-getter|1|71-postInit
            // write post-init user code here
        }//GEN-BEGIN:|71-getter|2|
        return itemCommand6;
    }
    //</editor-fold>//GEN-END:|71-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: list ">//GEN-BEGIN:|73-getter|0|73-preInit
    /**
     * Returns an initiliazed instance of list component.
     * @return the initialized component instance
     */
    public List getList() {
        if (list == null) {//GEN-END:|73-getter|0|73-preInit
            // write pre-init user code here
            list = new List("Send Position?", Choice.IMPLICIT);//GEN-BEGIN:|73-getter|1|73-postInit
            list.append("Yes", null);
            list.append("No", null);
            list.setCommandListener(this);
            list.setSelectedFlags(new boolean[] { false, false });//GEN-END:|73-getter|1|73-postInit
            // write post-init user code here
        }//GEN-BEGIN:|73-getter|2|
        return list;
    }
    //</editor-fold>//GEN-END:|73-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Method: listAction ">//GEN-BEGIN:|73-action|0|73-preAction
    /**
     * Performs an action assigned to the selected list element in the list component.
     */
    public void listAction() {//GEN-END:|73-action|0|73-preAction
        // enter pre-action user code here
        String __selectedString = getList().getString(getList().getSelectedIndex());//GEN-BEGIN:|73-action|1|83-preAction
        if (__selectedString != null) {
            if (__selectedString.equals("Yes")) {//GEN-END:|73-action|1|83-preAction
                // write pre-action user code here
                switchDisplayable(null, getWaitScreen());//GEN-LINE:|73-action|2|83-postAction
                // write post-action user code here
            } else if (__selectedString.equals("No")) {//GEN-LINE:|73-action|3|84-preAction
                // write pre-action user code here
                switchDisplayable(null, getFileBrowser());//GEN-LINE:|73-action|4|84-postAction
                // write post-action user code here
            }//GEN-BEGIN:|73-action|5|73-postAction
        }//GEN-END:|73-action|5|73-postAction
        // enter post-action user code here
    }//GEN-BEGIN:|73-action|6|
    //</editor-fold>//GEN-END:|73-action|6|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: stopCommand ">//GEN-BEGIN:|81-getter|0|81-preInit
    /**
     * Returns an initiliazed instance of stopCommand component.
     * @return the initialized component instance
     */
    public Command getStopCommand() {
        if (stopCommand == null) {//GEN-END:|81-getter|0|81-preInit
            // write pre-init user code here
            stopCommand = new Command("Stop", Command.STOP, 0);//GEN-LINE:|81-getter|1|81-postInit
            // write post-init user code here
        }//GEN-BEGIN:|81-getter|2|
        return stopCommand;
    }
    //</editor-fold>//GEN-END:|81-getter|2|

















    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: spacer2 ">//GEN-BEGIN:|96-getter|0|96-preInit
    /**
     * Returns an initiliazed instance of spacer2 component.
     * @return the initialized component instance
     */
    public Spacer getSpacer2() {
        if (spacer2 == null) {//GEN-END:|96-getter|0|96-preInit
            // write pre-init user code here
            spacer2 = new Spacer(16, 1);//GEN-LINE:|96-getter|1|96-postInit
            // write post-init user code here
        }//GEN-BEGIN:|96-getter|2|
        return spacer2;
    }
    //</editor-fold>//GEN-END:|96-getter|2|









    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: spacer5 ">//GEN-BEGIN:|101-getter|0|101-preInit
    /**
     * Returns an initiliazed instance of spacer5 component.
     * @return the initialized component instance
     */
    public Spacer getSpacer5() {
        if (spacer5 == null) {//GEN-END:|101-getter|0|101-preInit
            // write pre-init user code here
            spacer5 = new Spacer(16, 1);//GEN-LINE:|101-getter|1|101-postInit
            // write post-init user code here
        }//GEN-BEGIN:|101-getter|2|
        return spacer5;
    }
    //</editor-fold>//GEN-END:|101-getter|2|



    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: waitScreen1 ">//GEN-BEGIN:|103-getter|0|103-preInit
    /**
     * Returns an initiliazed instance of waitScreen1 component.
     * @return the initialized component instance
     */
    public WaitScreen getWaitScreen1() {
        if (waitScreen1 == null) {//GEN-END:|103-getter|0|103-preInit
            // write pre-init user code here
            waitScreen1 = new WaitScreen(getDisplay());//GEN-BEGIN:|103-getter|1|103-postInit
            waitScreen1.setTitle("APRS Server Connection");
            waitScreen1.setCommandListener(this);
            waitScreen1.setText("Wait...");
            waitScreen1.setTask(getTask1());//GEN-END:|103-getter|1|103-postInit
            // write post-init user code here
        }//GEN-BEGIN:|103-getter|2|
        return waitScreen1;
    }
    //</editor-fold>//GEN-END:|103-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: task1 ">//GEN-BEGIN:|106-getter|0|106-preInit
    /**
     * Returns an initiliazed instance of task1 component.
     * @return the initialized component instance
     */
    public SimpleCancellableTask getTask1() {
        if (task1 == null) {//GEN-END:|106-getter|0|106-preInit
            // write pre-init user code here
            task1 = new SimpleCancellableTask();//GEN-BEGIN:|106-getter|1|106-execute
            task1.setExecutable(new org.netbeans.microedition.util.Executable() {
                public void execute() throws Exception {//GEN-END:|106-getter|1|106-execute
                    // write task-execution user code here
                    if ( SRVConnected == false ) {


                        connectToServer();
                        SRVConnected = true;

                    } else {

                        SRVConnected = false;

                        is.close();
                        os.close();
                        sc.close();

                        is = null;
                        os = null;
                        sc = null;

                    }
                    screenUpdate();

                }//GEN-BEGIN:|106-getter|2|106-postInit
            });//GEN-END:|106-getter|2|106-postInit
            // write post-init user code here
        }//GEN-BEGIN:|106-getter|3|
        return task1;
    }
    //</editor-fold>//GEN-END:|106-getter|3|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: okCommand1 ">//GEN-BEGIN:|121-getter|0|121-preInit
    /**
     * Returns an initiliazed instance of okCommand1 component.
     * @return the initialized component instance
     */
    public Command getOkCommand1() {
        if (okCommand1 == null) {//GEN-END:|121-getter|0|121-preInit
            // write pre-init user code here
            okCommand1 = new Command("Ok", Command.OK, 0);//GEN-LINE:|121-getter|1|121-postInit
            // write post-init user code here
        }//GEN-BEGIN:|121-getter|2|
        return okCommand1;
    }
    //</editor-fold>//GEN-END:|121-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: cancelCommand ">//GEN-BEGIN:|123-getter|0|123-preInit
    /**
     * Returns an initiliazed instance of cancelCommand component.
     * @return the initialized component instance
     */
    public Command getCancelCommand() {
        if (cancelCommand == null) {//GEN-END:|123-getter|0|123-preInit
            // write pre-init user code here
            cancelCommand = new Command("Cancel", Command.CANCEL, 0);//GEN-LINE:|123-getter|1|123-postInit
            // write post-init user code here
        }//GEN-BEGIN:|123-getter|2|
        return cancelCommand;
    }
    //</editor-fold>//GEN-END:|123-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: form1 ">//GEN-BEGIN:|110-getter|0|110-preInit
    /**
     * Returns an initiliazed instance of form1 component.
     * @return the initialized component instance
     */
    public Form getForm1() {
        if (form1 == null) {//GEN-END:|110-getter|0|110-preInit
            // write pre-init user code here
            form1 = new Form("Station Pars", new Item[] { getTextField(), getChoiceGroup(), getTextField1(), getTextField15() });//GEN-BEGIN:|110-getter|1|110-postInit
            form1.addCommand(getOkCommand1());
            form1.addCommand(getCancelCommand());
            form1.setCommandListener(this);//GEN-END:|110-getter|1|110-postInit
            // write post-init user code here
        }//GEN-BEGIN:|110-getter|2|
        return form1;
    }
    //</editor-fold>//GEN-END:|110-getter|2|



    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: choiceGroup ">//GEN-BEGIN:|112-getter|0|112-preInit
    /**
     * Returns an initiliazed instance of choiceGroup component.
     * @return the initialized component instance
     */
    public ChoiceGroup getChoiceGroup() {
        if (choiceGroup == null) {//GEN-END:|112-getter|0|112-preInit
            // write pre-init user code here
            choiceGroup = new ChoiceGroup("APRS Symbol:", Choice.EXCLUSIVE);//GEN-BEGIN:|112-getter|1|112-postInit
            choiceGroup.append("(/I) Tcp/ip", null);
            choiceGroup.append("(/[) Jogger", null);
            choiceGroup.append("(/>) Car", null);
            choiceGroup.append("(/U) Bus", null);
            choiceGroup.append("(/=) Rail Eng.", null);
            choiceGroup.append("Other", null);
            choiceGroup.setSelectedFlags(new boolean[] { false, false, false, false, false, false });//GEN-END:|112-getter|1|112-postInit
            // write post-init user code here
        }//GEN-BEGIN:|112-getter|2|
        return choiceGroup;
    }
    //</editor-fold>//GEN-END:|112-getter|2|



    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField ">//GEN-BEGIN:|130-getter|0|130-preInit
    /**
     * Returns an initiliazed instance of textField component.
     * @return the initialized component instance
     */
    public TextField getTextField() {
        if (textField == null) {//GEN-END:|130-getter|0|130-preInit
            // write pre-init user code here
            textField = new TextField("Station Name:", null, 32, TextField.ANY);//GEN-LINE:|130-getter|1|130-postInit
            // write post-init user code here
        }//GEN-BEGIN:|130-getter|2|
        return textField;
    }
    //</editor-fold>//GEN-END:|130-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField1 ">//GEN-BEGIN:|131-getter|0|131-preInit
    /**
     * Returns an initiliazed instance of textField1 component.
     * @return the initialized component instance
     */
    public TextField getTextField1() {
        if (textField1 == null) {//GEN-END:|131-getter|0|131-preInit
            // write pre-init user code here
            textField1 = new TextField("Other Symbol:", null, 32, TextField.ANY);//GEN-LINE:|131-getter|1|131-postInit
            // write post-init user code here
        }//GEN-BEGIN:|131-getter|2|
        return textField1;
    }
    //</editor-fold>//GEN-END:|131-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: okCommand2 ">//GEN-BEGIN:|137-getter|0|137-preInit
    /**
     * Returns an initiliazed instance of okCommand2 component.
     * @return the initialized component instance
     */
    public Command getOkCommand2() {
        if (okCommand2 == null) {//GEN-END:|137-getter|0|137-preInit
            // write pre-init user code here
            okCommand2 = new Command("Ok", Command.OK, 0);//GEN-LINE:|137-getter|1|137-postInit
            // write post-init user code here
        }//GEN-BEGIN:|137-getter|2|
        return okCommand2;
    }
    //</editor-fold>//GEN-END:|137-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: cancelCommand1 ">//GEN-BEGIN:|139-getter|0|139-preInit
    /**
     * Returns an initiliazed instance of cancelCommand1 component.
     * @return the initialized component instance
     */
    public Command getCancelCommand1() {
        if (cancelCommand1 == null) {//GEN-END:|139-getter|0|139-preInit
            // write pre-init user code here
            cancelCommand1 = new Command("Cancel", Command.CANCEL, 0);//GEN-LINE:|139-getter|1|139-postInit
            // write post-init user code here
        }//GEN-BEGIN:|139-getter|2|
        return cancelCommand1;
    }
    //</editor-fold>//GEN-END:|139-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: form2 ">//GEN-BEGIN:|136-getter|0|136-preInit
    /**
     * Returns an initiliazed instance of form2 component.
     * @return the initialized component instance
     */
    public Form getForm2() {
        if (form2 == null) {//GEN-END:|136-getter|0|136-preInit
            // write pre-init user code here
            form2 = new Form("Connection Pars", new Item[] { getTextField2(), getTextField3(), getTextField4(), getTextField5(), getTextField6() });//GEN-BEGIN:|136-getter|1|136-postInit
            form2.addCommand(getOkCommand2());
            form2.addCommand(getCancelCommand1());
            form2.setCommandListener(this);//GEN-END:|136-getter|1|136-postInit
            // write post-init user code here
        }//GEN-BEGIN:|136-getter|2|
        return form2;
    }
    //</editor-fold>//GEN-END:|136-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField2 ">//GEN-BEGIN:|144-getter|0|144-preInit
    /**
     * Returns an initiliazed instance of textField2 component.
     * @return the initialized component instance
     */
    public TextField getTextField2() {
        if (textField2 == null) {//GEN-END:|144-getter|0|144-preInit
            // write pre-init user code here
            textField2 = new TextField("APRS Server:", null, 32, TextField.ANY);//GEN-LINE:|144-getter|1|144-postInit
            // write post-init user code here
        }//GEN-BEGIN:|144-getter|2|
        return textField2;
    }
    //</editor-fold>//GEN-END:|144-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField3 ">//GEN-BEGIN:|145-getter|0|145-preInit
    /**
     * Returns an initiliazed instance of textField3 component.
     * @return the initialized component instance
     */
    public TextField getTextField3() {
        if (textField3 == null) {//GEN-END:|145-getter|0|145-preInit
            // write pre-init user code here
            textField3 = new TextField("APRS Port:", null, 32, TextField.ANY);//GEN-LINE:|145-getter|1|145-postInit
            // write post-init user code here
        }//GEN-BEGIN:|145-getter|2|
        return textField3;
    }
    //</editor-fold>//GEN-END:|145-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField4 ">//GEN-BEGIN:|146-getter|0|146-preInit
    /**
     * Returns an initiliazed instance of textField4 component.
     * @return the initialized component instance
     */
    public TextField getTextField4() {
        if (textField4 == null) {//GEN-END:|146-getter|0|146-preInit
            // write pre-init user code here
            textField4 = new TextField("USER:", null, 32, TextField.ANY);//GEN-LINE:|146-getter|1|146-postInit
            // write post-init user code here
        }//GEN-BEGIN:|146-getter|2|
        return textField4;
    }
    //</editor-fold>//GEN-END:|146-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField5 ">//GEN-BEGIN:|147-getter|0|147-preInit
    /**
     * Returns an initiliazed instance of textField5 component.
     * @return the initialized component instance
     */
    public TextField getTextField5() {
        if (textField5 == null) {//GEN-END:|147-getter|0|147-preInit
            // write pre-init user code here
            textField5 = new TextField("PASSWORD:", null, 32, TextField.ANY);//GEN-LINE:|147-getter|1|147-postInit
            // write post-init user code here
        }//GEN-BEGIN:|147-getter|2|
        return textField5;
    }
    //</editor-fold>//GEN-END:|147-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField6 ">//GEN-BEGIN:|148-getter|0|148-preInit
    /**
     * Returns an initiliazed instance of textField6 component.
     * @return the initialized component instance
     */
    public TextField getTextField6() {
        if (textField6 == null) {//GEN-END:|148-getter|0|148-preInit
            // write pre-init user code here
            textField6 = new TextField("FILTER:", null, 32, TextField.ANY);//GEN-LINE:|148-getter|1|148-postInit
            // write post-init user code here
        }//GEN-BEGIN:|148-getter|2|
        return textField6;
    }
    //</editor-fold>//GEN-END:|148-getter|2|





    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: list1 ">//GEN-BEGIN:|151-getter|0|151-preInit
    /**
     * Returns an initiliazed instance of list1 component.
     * @return the initialized component instance
     */
    public List getList1() {
        if (list1 == null) {//GEN-END:|151-getter|0|151-preInit
            // write pre-init user code here
            list1 = new List("Exit?", Choice.IMPLICIT);//GEN-BEGIN:|151-getter|1|151-postInit
            list1.append("Yes", null);
            list1.append("No", null);
            list1.setCommandListener(this);
            list1.setSelectedFlags(new boolean[] { false, false });//GEN-END:|151-getter|1|151-postInit
            // write post-init user code here
        }//GEN-BEGIN:|151-getter|2|
        return list1;
    }
    //</editor-fold>//GEN-END:|151-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Method: list1Action ">//GEN-BEGIN:|151-action|0|151-preAction
    /**
     * Performs an action assigned to the selected list element in the list1 component.
     */
    public void list1Action() {//GEN-END:|151-action|0|151-preAction
        // enter pre-action user code here
        String __selectedString = getList1().getString(getList1().getSelectedIndex());//GEN-BEGIN:|151-action|1|154-preAction
        if (__selectedString != null) {
            if (__selectedString.equals("Yes")) {//GEN-END:|151-action|1|154-preAction
                // write pre-action user code here
                exitMIDlet();//GEN-LINE:|151-action|2|154-postAction
                // write post-action user code here
            } else if (__selectedString.equals("No")) {//GEN-LINE:|151-action|3|155-preAction
                // write pre-action user code here
                switchDisplayable(null, getForm());//GEN-LINE:|151-action|4|155-postAction
                // write post-action user code here
            }//GEN-BEGIN:|151-action|5|151-postAction
        }//GEN-END:|151-action|5|151-postAction
        // enter post-action user code here
    }//GEN-BEGIN:|151-action|6|
    //</editor-fold>//GEN-END:|151-action|6|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: cancelCommand2 ">//GEN-BEGIN:|167-getter|0|167-preInit
    /**
     * Returns an initiliazed instance of cancelCommand2 component.
     * @return the initialized component instance
     */
    public Command getCancelCommand2() {
        if (cancelCommand2 == null) {//GEN-END:|167-getter|0|167-preInit
            // write pre-init user code here
            cancelCommand2 = new Command("Cancel", Command.CANCEL, 0);//GEN-LINE:|167-getter|1|167-postInit
            // write post-init user code here
        }//GEN-BEGIN:|167-getter|2|
        return cancelCommand2;
    }
    //</editor-fold>//GEN-END:|167-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand7 ">//GEN-BEGIN:|170-getter|0|170-preInit
    /**
     * Returns an initiliazed instance of itemCommand7 component.
     * @return the initialized component instance
     */
    public Command getItemCommand7() {
        if (itemCommand7 == null) {//GEN-END:|170-getter|0|170-preInit
            // write pre-init user code here
            itemCommand7 = new Command("Send", Command.ITEM, 0);//GEN-LINE:|170-getter|1|170-postInit
            // write post-init user code here
        }//GEN-BEGIN:|170-getter|2|
        return itemCommand7;
    }
    //</editor-fold>//GEN-END:|170-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand8 ">//GEN-BEGIN:|172-getter|0|172-preInit
    /**
     * Returns an initiliazed instance of itemCommand8 component.
     * @return the initialized component instance
     */
    public Command getItemCommand8() {
        if (itemCommand8 == null) {//GEN-END:|172-getter|0|172-preInit
            // write pre-init user code here
            itemCommand8 = new Command("Cancel", Command.ITEM, 0);//GEN-LINE:|172-getter|1|172-postInit
            // write post-init user code here
        }//GEN-BEGIN:|172-getter|2|
        return itemCommand8;
    }
    //</editor-fold>//GEN-END:|172-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: form3 ">//GEN-BEGIN:|162-getter|0|162-preInit
    /**
     * Returns an initiliazed instance of form3 component.
     * @return the initialized component instance
     */
    public Form getForm3() {
        if (form3 == null) {//GEN-END:|162-getter|0|162-preInit
            // write pre-init user code here
            form3 = new Form("Message", new Item[] { getTextField7(), getTextField8(), getTextField9() });//GEN-BEGIN:|162-getter|1|162-postInit
            form3.addCommand(getCancelCommand2());
            form3.addCommand(getItemCommand7());
            form3.setCommandListener(this);//GEN-END:|162-getter|1|162-postInit
            // write post-init user code here
        }//GEN-BEGIN:|162-getter|2|
        return form3;
    }
    //</editor-fold>//GEN-END:|162-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField7 ">//GEN-BEGIN:|163-getter|0|163-preInit
    /**
     * Returns an initiliazed instance of textField7 component.
     * @return the initialized component instance
     */
    public TextField getTextField7() {
        if (textField7 == null) {//GEN-END:|163-getter|0|163-preInit
            // write pre-init user code here
            textField7 = new TextField("To:", null, 32, TextField.ANY);//GEN-LINE:|163-getter|1|163-postInit
            // write post-init user code here
        }//GEN-BEGIN:|163-getter|2|
        return textField7;
    }
    //</editor-fold>//GEN-END:|163-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField8 ">//GEN-BEGIN:|164-getter|0|164-preInit
    /**
     * Returns an initiliazed instance of textField8 component.
     * @return the initialized component instance
     */
    public TextField getTextField8() {
        if (textField8 == null) {//GEN-END:|164-getter|0|164-preInit
            // write pre-init user code here
            textField8 = new TextField("From:", null, 32, TextField.ANY);//GEN-LINE:|164-getter|1|164-postInit
            // write post-init user code here
        }//GEN-BEGIN:|164-getter|2|
        return textField8;
    }
    //</editor-fold>//GEN-END:|164-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField9 ">//GEN-BEGIN:|165-getter|0|165-preInit
    /**
     * Returns an initiliazed instance of textField9 component.
     * @return the initialized component instance
     */
    public TextField getTextField9() {
        if (textField9 == null) {//GEN-END:|165-getter|0|165-preInit
            // write pre-init user code here
            textField9 = new TextField("Text:", null, 60, TextField.ANY);//GEN-LINE:|165-getter|1|165-postInit
            // write post-init user code here
        }//GEN-BEGIN:|165-getter|2|
        return textField9;
    }
    //</editor-fold>//GEN-END:|165-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: waitScreen2 ">//GEN-BEGIN:|175-getter|0|175-preInit
    /**
     * Returns an initiliazed instance of waitScreen2 component.
     * @return the initialized component instance
     */
    public WaitScreen getWaitScreen2() {
        if (waitScreen2 == null) {//GEN-END:|175-getter|0|175-preInit
            // write pre-init user code here
            waitScreen2 = new WaitScreen(getDisplay());//GEN-BEGIN:|175-getter|1|175-postInit
            waitScreen2.setTitle("waitScreen2");
            waitScreen2.setCommandListener(this);
            waitScreen2.setTask(getTask2());//GEN-END:|175-getter|1|175-postInit
            // write post-init user code here
        }//GEN-BEGIN:|175-getter|2|
        return waitScreen2;
    }
    //</editor-fold>//GEN-END:|175-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: task2 ">//GEN-BEGIN:|178-getter|0|178-preInit
    /**
     * Returns an initiliazed instance of task2 component.
     * @return the initialized component instance
     */
    public SimpleCancellableTask getTask2() {
        if (task2 == null) {//GEN-END:|178-getter|0|178-preInit
            // write pre-init user code here
            task2 = new SimpleCancellableTask();//GEN-BEGIN:|178-getter|1|178-execute
            task2.setExecutable(new org.netbeans.microedition.util.Executable() {
                public void execute() throws Exception {//GEN-END:|178-getter|1|178-execute
                    // write task-execution user code here
                    //APRSCALL
                    //this->SysVars->getVal( "APRSCall" ) , From + ">TCPIP*", Msg
                    //From.left( From.indexOf('>') ) + ">" + SysVars->getVal( "APRSCall" ) +",TCPIP*:" + MsgText
                    //Msg = ":" + MTo.leftJustified( 9, ' ' ) + ":" + MsgText;

                    String destCall = textField7.getString();

                    while (destCall.length()!=9) {
                        destCall = destCall + " ";
                    }

                    //String msg   = new String(textField8.getString() + ">" + APRSCALL + ",TCPIP*:" + destCall +":" + textField9.getString() + "\n\r");

                    String packet = APRS_STATION_NAME+">"+APRSCALL+",TCPIP*::"+destCall.toUpperCase() +":" + textField9.getString()+'\n'+'\r';

                    byte[] conn_data = packet.getBytes(); //RV4CQ UT0UR UB3AAZ-3 UB9UPA-10

                    //System.out.println( textField8.toString()+ ">" + APRSCALL + ",TCPIP*:" + textField9.toString()  + "\n");
                    System.out.println( "msg=" + packet );

                    os.write(conn_data);


                    Messages = destCall.toUpperCase() +'<' + APRS_STATION_NAME + ": " + textField9.getString() + "\n\n" + Messages;

                    if ( Messages.length() > 1920 ) {

                        Messages = Messages.substring(0, 1920);

                    }

                    textBox1.setString( Messages );

                }//GEN-BEGIN:|178-getter|2|178-postInit
            });//GEN-END:|178-getter|2|178-postInit
            // write post-init user code here
        }//GEN-BEGIN:|178-getter|3|
        return task2;
    }
    //</editor-fold>//GEN-END:|178-getter|3|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField10 ">//GEN-BEGIN:|182-getter|0|182-preInit
    /**
     * Returns an initiliazed instance of textField10 component.
     * @return the initialized component instance
     */
    public TextField getTextField10() {
        if (textField10 == null) {//GEN-END:|182-getter|0|182-preInit
            // write pre-init user code here
            textField10 = new TextField("Server Info:", null, 128, TextField.ANY | TextField.UNEDITABLE);//GEN-LINE:|182-getter|1|182-postInit
            // write post-init user code here
        }//GEN-BEGIN:|182-getter|2|
        return textField10;
    }
    //</editor-fold>//GEN-END:|182-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField11 ">//GEN-BEGIN:|183-getter|0|183-preInit
    /**
     * Returns an initiliazed instance of textField11 component.
     * @return the initialized component instance
     */
    public TextField getTextField11() {
        if (textField11 == null) {//GEN-END:|183-getter|0|183-preInit
            // write pre-init user code here
            textField11 = new TextField("Station Info:", null, 128, TextField.ANY | TextField.UNEDITABLE);//GEN-LINE:|183-getter|1|183-postInit
            // write post-init user code here
        }//GEN-BEGIN:|183-getter|2|
        return textField11;
    }
    //</editor-fold>//GEN-END:|183-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField12 ">//GEN-BEGIN:|184-getter|0|184-preInit
    /**
     * Returns an initiliazed instance of textField12 component.
     * @return the initialized component instance
     */
    public TextField getTextField12() {
        if (textField12 == null) {//GEN-END:|184-getter|0|184-preInit
            // write pre-init user code here
            textField12 = new TextField("User/Pass:", null, 64, TextField.ANY | TextField.UNEDITABLE);//GEN-LINE:|184-getter|1|184-postInit
            // write post-init user code here
        }//GEN-BEGIN:|184-getter|2|
        return textField12;
    }
    //</editor-fold>//GEN-END:|184-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField13 ">//GEN-BEGIN:|185-getter|0|185-preInit
    /**
     * Returns an initiliazed instance of textField13 component.
     * @return the initialized component instance
     */
    public TextField getTextField13() {
        if (textField13 == null) {//GEN-END:|185-getter|0|185-preInit
            // write pre-init user code here
            textField13 = new TextField("Position Info:", null, 128, TextField.ANY | TextField.UNEDITABLE);//GEN-LINE:|185-getter|1|185-postInit
            // write post-init user code here
        }//GEN-BEGIN:|185-getter|2|
        return textField13;
    }
    //</editor-fold>//GEN-END:|185-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField14 ">//GEN-BEGIN:|186-getter|0|186-preInit
    /**
     * Returns an initiliazed instance of textField14 component.
     * @return the initialized component instance
     */
    public TextField getTextField14() {
        if (textField14 == null) {//GEN-END:|186-getter|0|186-preInit
            // write pre-init user code here
            textField14 = new TextField("Comment:", null, 128, TextField.ANY | TextField.UNEDITABLE);//GEN-LINE:|186-getter|1|186-postInit
            // write post-init user code here
        }//GEN-BEGIN:|186-getter|2|
        return textField14;
    }
    //</editor-fold>//GEN-END:|186-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand9 ">//GEN-BEGIN:|187-getter|0|187-preInit
    /**
     * Returns an initiliazed instance of itemCommand9 component.
     * @return the initialized component instance
     */
    public Command getItemCommand9() {
        if (itemCommand9 == null) {//GEN-END:|187-getter|0|187-preInit
            // write pre-init user code here
            itemCommand9 = new Command("Inbox", Command.ITEM, 0);//GEN-LINE:|187-getter|1|187-postInit
            // write post-init user code here
        }//GEN-BEGIN:|187-getter|2|
        return itemCommand9;
    }
    //</editor-fold>//GEN-END:|187-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: exitCommand2 ">//GEN-BEGIN:|192-getter|0|192-preInit
    /**
     * Returns an initiliazed instance of exitCommand2 component.
     * @return the initialized component instance
     */
    public Command getExitCommand2() {
        if (exitCommand2 == null) {//GEN-END:|192-getter|0|192-preInit
            // write pre-init user code here
            exitCommand2 = new Command("Exit", Command.EXIT, 0);//GEN-LINE:|192-getter|1|192-postInit
            // write post-init user code here
        }//GEN-BEGIN:|192-getter|2|
        return exitCommand2;
    }
    //</editor-fold>//GEN-END:|192-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: okCommand3 ">//GEN-BEGIN:|194-getter|0|194-preInit
    /**
     * Returns an initiliazed instance of okCommand3 component.
     * @return the initialized component instance
     */
    public Command getOkCommand3() {
        if (okCommand3 == null) {//GEN-END:|194-getter|0|194-preInit
            // write pre-init user code here
            okCommand3 = new Command("Ok", Command.OK, 0);//GEN-LINE:|194-getter|1|194-postInit
            // write post-init user code here
        }//GEN-BEGIN:|194-getter|2|
        return okCommand3;
    }
    //</editor-fold>//GEN-END:|194-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textBox1 ">//GEN-BEGIN:|190-getter|0|190-preInit
    /**
     * Returns an initiliazed instance of textBox1 component.
     * @return the initialized component instance
     */
    public TextBox getTextBox1() {
        if (textBox1 == null) {//GEN-END:|190-getter|0|190-preInit
            // write pre-init user code here
            textBox1 = new TextBox("Messages", "", 2000, TextField.ANY | TextField.UNEDITABLE);//GEN-BEGIN:|190-getter|1|190-postInit
            textBox1.addCommand(getExitCommand2());
            textBox1.addCommand(getItemCommand10());
            textBox1.setCommandListener(this);//GEN-END:|190-getter|1|190-postInit
            // write post-init user code here
            textBox1.setString( Messages );

        }//GEN-BEGIN:|190-getter|2|
        return textBox1;
    }
    //</editor-fold>//GEN-END:|190-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: itemCommand10 ">//GEN-BEGIN:|199-getter|0|199-preInit
    /**
     * Returns an initiliazed instance of itemCommand10 component.
     * @return the initialized component instance
     */
    public Command getItemCommand10() {
        if (itemCommand10 == null) {//GEN-END:|199-getter|0|199-preInit
            // write pre-init user code here
            itemCommand10 = new Command("Send", Command.ITEM, 0);//GEN-LINE:|199-getter|1|199-postInit
            // write post-init user code here
        }//GEN-BEGIN:|199-getter|2|
        return itemCommand10;
    }
    //</editor-fold>//GEN-END:|199-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField15 ">//GEN-BEGIN:|202-getter|0|202-preInit
    /**
     * Returns an initiliazed instance of textField15 component.
     * @return the initialized component instance
     */
    public TextField getTextField15() {
        if (textField15 == null) {//GEN-END:|202-getter|0|202-preInit
            // write pre-init user code here
            textField15 = new TextField("Beacon period(s):", null, 32, TextField.ANY);//GEN-LINE:|202-getter|1|202-postInit
            // write post-init user code here
        }//GEN-BEGIN:|202-getter|2|
        return textField15;
    }
    //</editor-fold>//GEN-END:|202-getter|2|

    //<editor-fold defaultstate="collapsed" desc=" Generated Getter: textField16 ">//GEN-BEGIN:|203-getter|0|203-preInit
    /**
     * Returns an initiliazed instance of textField16 component.
     * @return the initialized component instance
     */
    public TextField getTextField16() {
        if (textField16 == null) {//GEN-END:|203-getter|0|203-preInit
            // write pre-init user code here
            textField16 = new TextField("Beacon Timer(s):", null, 32, TextField.ANY | TextField.UNEDITABLE);//GEN-LINE:|203-getter|1|203-postInit
            // write post-init user code here
        }//GEN-BEGIN:|203-getter|2|
        return textField16;
    }
    //</editor-fold>//GEN-END:|203-getter|2|

    /**
     * Returns a display instance.
     * @return the display instance.
     */
    public Display getDisplay () {
        return Display.getDisplay(this);
    }

    /**
     * Exits MIDlet.
     */
    public void exitMIDlet() {

 try {

              //recordStore = RecordStore.openRecordStore(DBNAME, true);

              ByteArrayOutputStream baos = new ByteArrayOutputStream();

              DataOutputStream dos = new DataOutputStream(baos);

              //dos.writeUTF(stringForWrite);
              dos.writeUTF(APRS_SERVER);
              dos.writeUTF(APRS_PORT);
              dos.writeUTF(APRS_USER);
              dos.writeUTF(APRS_PASS);
              dos.writeUTF(APRS_FILTER);
              dos.writeUTF(APRS_STATION_NAME);
              dos.writeUTF(APRS_STATION_SYM);
              dos.writeUTF(APRS_BEACON_PERIOD);


              byte[] record = baos.toByteArray();

              if( recordStore.getNextRecordID() == 1 ){
                    // проверка хранилища на наличие записей. если их там нет, то и читать нечего
                  System.out.println("No Data in RMS - insert");

                  recordStore.addRecord(record, 0, record.length);

              } else {
                  System.out.println("Data in RMS - update");

                  recordStore.setRecord( 1, record, 0, record.length );
              }

              recordStore.closeRecordStore();

          } catch (Exception e) {

              System.out.println("Stop - Exception: " + e);

          }

        

        System.gc();

        switchDisplayable (null, null);
        destroyApp(true);
        notifyDestroyed();
    }

    /**
     * Called when MIDlet is started.
     * Checks whether the MIDlet have been already started and initialize/starts or resumes the MIDlet.
     */
    public void startApp() {
        if (midletPaused) {
            resumeMIDlet ();
        } else {
            initialize ();
            startMIDlet ();
        }
        midletPaused = false;
    }

    /**
     * Called when MIDlet is paused.
     */
    public void pauseApp() {
        midletPaused = true;
    }

    /**
     * Called to signal the MIDlet to terminate.
     * @param unconditional if true, then the MIDlet has to be unconditionally terminated and all resources has to be released.
     */
    public void destroyApp(boolean unconditional) {
    }

    public void screenUpdate() {

        System.out.println("screenUpdate();");

        //if (textBox != null) {
//
//            textBox.setString( log );
 //       }


        if ( SRVConnected == true ) {
            textField10.setString( APRS_SERVER + ":" + APRS_PORT + " On" );
        } else {
            textField10.setString( APRS_SERVER + ":" + APRS_PORT + " Off"  );
        }
        textField11.setString( APRS_STATION_NAME + " (" + APRS_STATION_SYM + ")"  );
        textField12.setString( APRS_USER + "/" + APRS_PASS  );

        
        textField13.setString( lastPosition );
        textField14.setString( lastStatus );
        textField16.setString( Integer.toString( timerCounter ) );

    }

    public void run() {
        System.out.println("thread run ");
        while(true) {
            //System.out.println("thread run...");
        //    screenUpdate();
            if (( sc != null )&(is != null)&(os != null)) {
                StringBuffer data = new StringBuffer();
                byte[] buf = new byte[1024];

                    try {

                        int count = is.read(buf);
                        if( count == -1 ) { break; }
                        for(int i = 0; i < count; i++) {
                                   data.append((char) buf[i]);
                        }

                        String message = new String (data.toString());

                        System.out.println("connectToServer - read: " + message);


                        if ( message.indexOf( ':' ) > 0 ) {

                            if (  message.indexOf( ':' ) == message.indexOf( "::" ) ) {

                                //типичная мессага
                                //UA5AA-14>APU25N,MB7UXN-14*,WIDE2-1,qAR,SM0RWO-14::DH8HP    :GA{08
                                //String tst= "RV3DHC>APU25N,TCPIP*,qAC,T2RUSSIA::UA3MQJ-1 :No stations have been heard except via a digipeater";
                                String tst= message;
                                String MFrom = tst.substring(0, tst.indexOf('>'));
                                String MTo = tst.substring(tst.indexOf("::")+2, tst.indexOf(":",tst.indexOf("::")+2)).trim();
                                String MMsg = tst.substring( 1 + tst.indexOf(":", tst.indexOf(":",tst.indexOf("::")+2) ) );


                                if ( (MFrom.length()!=0)&&(MTo.length()!=0)&&(MMsg.length()!=0) ) {

                                    Messages = MTo +'<' + MFrom + ": " + MMsg + '\n' + Messages;

                                    if ( Messages.length() > 1920 ) {

                                        Messages = Messages.substring(0, 1920);

                                    }

                                    textBox1.setString( Messages );
                                }

                            }

                        }
                        

                    } catch (Exception e) {

                        System.out.println("run is - Exception: " + e);

                    }
            }

        }

    }

        private void connectToServer() {

        System.out.println("connectToServer");



        try {


            if ( sc == null ) {

                System.out.println("socket://"+APRS_SERVER+":"+APRS_PORT);

                //это прямой доступ, когда на ПК разработчика прямой интернет
                //соединяемся сразу с APRS сервером
                sc = (SocketConnection)Connector.open("socket://"+APRS_SERVER+":"+APRS_PORT);

                //а это через прокси. соединяемся сначала с прокси сервером
                //sc = (SocketConnection)Connector.open("socket://10.0.0.39:3128");

                sc.setSocketOption(SocketConnection.DELAY, 1);
                sc.setSocketOption(SocketConnection.LINGER, 5);
                sc.setSocketOption(SocketConnection.RCVBUF, 8192);
                sc.setSocketOption(SocketConnection.SNDBUF, 2048);



            is  = sc.openInputStream();
            os = sc.openOutputStream();


            //INFO
            //'\n' or '0x0A' (10 in decimal) -> This character is called "Line Feed" (LF).
            //'\r' or '0x0D' (13 in decimal) -> This one is called "Carriage return" (CR).
            //в HTTP запросах нормальные люди шлют 0D0A то есть \r\n а в конце \r\n\r\n

            //выполняется только при соединении через прокси - соединение с APRS сервером
            //прокси сервер без аутентификации
            //byte[] proxy_conn_data = ("CONNECT russia.aprs2.net:14580  HTTP/1.1\n\n").getBytes();
            //прокси сервер с аутентификацией
            //byte[] proxy_conn_data = ("CONNECT "+APRS_SERVER+":"+APRS_PORT+" HTTP/1.1\r\nAuthorization: Basic Ym9sc2hha292X2F2OmZydGtrZg==\r\nProxy-Authorization: Basic Ym9sc2hha292X2F2OmZydGtrZg==\r\n\r\n").getBytes();

            //os.write(proxy_conn_data);

           // APRS_FILTER = "p/ISS/R/U/LY/YL/ES/EU/EW/ER/4X/4Z/";
            //это все тестовое будет
            //APRS_USER = "UA3MQJ";
            //APRS_PASS = "17572";
            //APRS_FILTER = "p/ISS/R/U/LY/YL/ES/EU/EW/ER/4X/4Z/";


            byte[] conn_data = ("user " + APRS_USER + " pass " + APRS_PASS + " vers QAPRS_JPos v1 filter " + APRS_FILTER + "\n").getBytes();

            os.write(conn_data);

            //th = new Thread( this );
            //th.start();

            } else {

                System.out.println("already connected");

            }


        } catch (Exception e) {

              System.out.println("connectToServer - Exception: " + e);

        }

    }

//чтение координат из текстового файла
private void readFile() {

        //switchDisplayable(null, getForm2());

        try {
            FileConnection textFile = fileBrowser.getSelectedFile();
            getTextBox().setString("");
            InputStream fis = textFile.openInputStream();
            byte[] b = new byte[1024];
            int length = fis.read(b, 0, 1024);
            fis.close();

            String position = new String(b, 0, length);



            //до первого пробела - широта
            //до второго долгота
            //а после - текст маяка
            int space1 = position.indexOf(" ");
            String Lat = position.substring(0, space1 );
            int space2 = position.indexOf(" ", space1 +1);
            String Lng = position.substring(space1+1, position.indexOf(" ", space1+1));
            String Msg = position.substring(space2+1);

            lastPosition = Lat + " " + Lng;
            lastStatus   = Msg;

            int LngR = Integer.parseInt( Lng.substring(7, 9) );
            int LatR = Integer.parseInt( Lat.substring(6, 8) );

            Random r = new Random();

            //удаление одной лишней точки
            Lat = Lat.substring(0, 2)+Lat.substring(3);
            Lng = Lng.substring(0, 3)+Lng.substring(4);

            //случайное изменение координат в небольших пределах, чтобы обойти проверку на дубли на aprsfi
            LngR = LngR +  (int)(r.nextFloat()*3);
            LatR = LatR +  (int)(r.nextFloat()*3);

            if ( LngR > 99 ) LngR = 99;
            if ( LatR > 99 ) LatR = 99;

            Lat = Lat.substring(0, 5)+numberToString(LatR)+Lat.substring(7, 8);
            Lng = Lng.substring(0, 6)+numberToString(LngR)+Lng.substring(8, 9);

            Date date = new Date();

            TimeZone defaultZone = TimeZone.getTimeZone("GMT-2");

            int offs = defaultZone.getRawOffset();

            Calendar calendar = Calendar.getInstance(defaultZone);
            calendar.setTime(date);
            StringBuffer sb = new StringBuffer();

            sb.append('/');
            int day = calendar.get(Calendar.DAY_OF_MONTH);
            sb.append(numberToString(day));
            int hour = calendar.get(Calendar.HOUR_OF_DAY) + 1;
            sb.append(numberToString(hour));
            int minute = calendar.get(Calendar.MINUTE) + 1;
            sb.append(numberToString(minute));
            sb.append('z');

            String packet = APRS_STATION_NAME+">"+APRSCALL+",TCPIP*:"+sb+Lat+APRS_STATION_SYM.charAt(0)+Lng+APRS_STATION_SYM.charAt(1)+Msg+'\n'+'\r';

            //UA3MQJ>APUN25,TCPIP*: + MsgText
            //UA1CEC>APU25N,TCPIP*,qAC,T2RUSSIA:=5931.88N/03053.60E&10147 - inet {UIV32}
            //5931.88N 03053.60E Fly e135 - inet {QAPRSj}

            if (length > 0) {




                //socket://host.com:80
                //HttpConnection MyCon = (HttpConnection) Connector.open("socket://"+APRS_SERVER+":"+, Connector.READ_WRITE, true);
                try{
                  //SocketConnection sc = (SocketConnection)
                  //  Connector.open("socket://"+APRS_SERVER+":"+APRS_PORT);

                  //OutputStream os = null;


                  byte[] packet_data = packet.getBytes();
                  os.write(packet_data);

                  //textBox1.setString( position + " \n packet:" + packet );

                } catch (IOException x){
                     x.printStackTrace();
                }

            }


        } catch (IOException ex) {
            ex.printStackTrace();
        }

        screenUpdate();

    }

     private String numberToString(int value) {
        String valStr = Integer.toString(value);
        return (value < 10) ? "0" + valStr: valStr;
     }


}


