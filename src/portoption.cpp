#include "include/portoption.h"

PortoptionDialog::PortoptionDialog()
{

	setupUi(this);

	show();

        connect(saveButton, SIGNAL(clicked()), this, SLOT(saveButtonClick()) );
        connect(closeButton, SIGNAL(clicked()), this, SLOT(closeButtonClick()) );
        connect( EasySetupComboBox, SIGNAL( currentIndexChanged( int ) ),
                 this,              SLOT( KISSEasySetup( int ) ) );

};


PortoptionDialog::~PortoptionDialog()
{

}

void PortoptionDialog::saveButtonClick() {
    QSqlQuery query;

    switch ( portTypeBox->currentIndex()-1 ) {
        case 0 : //APRS INTERNET SERVER
                        query.exec( "delete from port_param_values where port_num="+portNumEdit->text() );
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",1,'"+hostNameEdit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",2,'"+hostName2Edit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",3,'"+hostName3Edit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",4,'"+userEdit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",5,'"+passEdit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",6,'"+APRSFilter->text()+"')");
                        break;
        case 1 : //KISS
                        query.exec( "delete from port_param_values where port_num="+portNumEdit->text() );
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",1,'"+COMPortNumBox->currentText()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",2,'"+COMPortSpeedBox->currentText()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",3,'"+KISSComm1Edit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",4,'"+KISSComm2Edit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",5,'"+KISSComm3Edit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",6,'"+KISSComm4Edit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",7,'"+TXDelayEdit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",8,'"+TXTailEdit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",9,'"+SlotTimeEdit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",10,'"+PersistEdit->text()+"')");
                        if (FullDuplexCheckBox->isChecked()) {
                            query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                        "values("+portNumEdit->text()+",11,'T')");
                        } else {
                            query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                        "values("+portNumEdit->text()+",11,'F')");
                        };
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",12,'"+KISSCommEXITEdit->text()+"')");

                        break;
        case 2 : //AGW
                        query.exec( "delete from port_param_values where port_num="+portNumEdit->text() );
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",1,'"+AGWHostEdit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",2,'"+AGWPortBox->text()+"')");
                        break;
        case 5 : //AXIP
                        query.exec( "delete from port_param_values where port_num="+portNumEdit->text() );
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",1,'"+AXIPRXPortBox->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",2,'"+AXIPTXHostEdit->text()+"')");
                        query.exec( "insert into port_param_values (port_num, par_code, par_value) "
                                    "values("+portNumEdit->text()+",3,'"+AXIPTXPortBox->text()+"')");
                        break;
    };

    if (portBeaconIntervalEdit->text()=="") portBeaconIntervalEdit->setText("-1");

        QByteArray Sym;
        int symindex;
        Sym.resize( 2 );

        symindex = portSymbolEdit->currentIndex();
        Sym.data()[1] = char( ( (symindex - (symindex % 2) ) / 2) + 33 );
        if ( ( symindex % 2 ) == 1 ) Sym.data()[0] = '\\'; else Sym.data()[0] = '/';


    query.exec( "update ports set port_note = '" + portNameEdit->text() + "',"
                "port_type_id = "                + QString::number(portTypeBox->currentIndex()-1) + ", "
                "port_call = '"                  + portCallEdit->text() + "', "
                "port_beacon_text = '"           + portBeaconTextEdit->text() + "', "
                "port_beacon_interval = "        + portBeaconIntervalEdit->text() + ", "
                "port_unproto_address = '"       + portUnprotoEdit->text() + "', "
                "port_latitude = '"              + portLatEdit->text() + "', "
                "port_longitude = '"             + portLngEdit->text() + "', "
                "port_symbol = '"                + Sym + "'  "
                "where port_num = "              + portNumEdit->text() + " " );


    close();
}

void PortoptionDialog::closeButtonClick() {
    close();
}

void PortoptionDialog::KISSEasySetup(int index) {

    switch ( index ) {
        case 0 : //TNC2
                 KISSComm1Edit->setText("^M");
                 KISSComm2Edit->setText("KISS ON");
                 KISSComm3Edit->setText("RESTART");
                 KISSComm4Edit->setText("");
                 KISSCommEXITEdit->setText("192,255,192");
                 break;
        case 1 : //PK
                 KISSComm1Edit->setText("*~");
                 KISSComm2Edit->setText("EXPERT ON");
                 KISSComm3Edit->setText("KISS ON");
                 KISSComm4Edit->setText("RESTART");
                 KISSCommEXITEdit->setText("192,255,192");
                 break;
        case 2 : //D700
                 KISSComm1Edit->setText("^C^MAI 1^MTC 1^MTNC 2^MTC 0^M^M^M^M");
                 KISSComm2Edit->setText("MON OFF");
                 KISSComm3Edit->setText("KISS ON");
                 KISSComm4Edit->setText("RESTART");
                 KISSCommEXITEdit->setText("192,255,192");
                 break;
        case 3 : //Kan
                 KISSComm1Edit->setText("^M");
                 KISSComm2Edit->setText("INT KISS");
                 KISSComm3Edit->setText("RESET");
                 KISSComm4Edit->setText("");
                 KISSCommEXITEdit->setText("192,255,192");
                 break;
        case 4 : //TF
                 KISSComm1Edit->setText("^M");
                 KISSComm2Edit->setText("^[@K");
                 KISSComm3Edit->setText("");
                 KISSComm4Edit->setText("");
                 KISSCommEXITEdit->setText("192,255,192,13");
                 break;
    };
}

