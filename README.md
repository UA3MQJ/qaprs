# Project QAPRS

[Wiki](https://github.com/UA3MQJ/qaprs/wiki)

EN|[RU](README_RU.md)|[DE](README_DE.md)

APRS - Automatic Position (or Packet) Reporting System is a special variant of a packet radio communication which report has been registered in 1992 by Bob Bruninga (WB4APR).

From usual packed radio APRS differs in two principle positions. Firsly, it is a display of maps and other data and, secondly, it uses the report which updates data in real time. Any object simply sends the information, and reception stations register it. In batch communication the great bulk of users use a point-to-point connection which usually has incidental character. It does not allow to supervise event in real time where a life term of the information is very short. Today APRS is the biggest system in the world of a real time scale by means of which it is possible to watch, both any special event, and critical situations.

Where is a moving object (airplane, boat, ballon, auto, motorcycle, runner, dog)? Where is radioshop, departure city? What's weather like in various places of the country, city? Where is a hurricane? Where is DX? To answer the questions and on many others, APRS transfers in automatic a mode data about a place of the position of an object and value of its physical parameters. The information can be transferred by several ways. It can be transferred by amateur radio, both on HF, and on VHF, CB, a sea range, a cellular telephone, the modem and allocated connection about the Internet.

The aim of this project is self-improvement in the field of programming reports of packet networks, fastening of skills of development crossplatform products on the basis of c++ and QT4. The project is developed as functional analogue of software product UI-View.

The work of APRS system can also be provided without application of the additional equipment, such as packet modems or TNC controllers. As the modem the sound card can be used. It allows any HAM working on digital mode, to test work and with network APRS without additional financial investments. It is also possible to enter network APRS without having any send-receive equipment using an Internet APRS server, but it's only a part of opportunities of the system. It is more interesting for HAMs to use a radio amateur bands as transport for APRS.

Realization of the project will allow to learn APRS protokol, to fix skills of work with network protocol practically, to master development of modules for work with hardware devices (sound cards, serial ports). Recently a set of novelties in the field of network services has appeared with which it is possible to integrate to APRS, to add functionalities of application of a network, it helps to develop opportunities and a scope of the report, to involve additional users.

Sincerely, Alex UA3MQJ. 73!

![normal_qaprs](normal_qaprs.png.jpg?raw=true)

стартует startwindow
в зависимости от подключения к отдельному ядру или запуск ядра внутри gui
1 запуск ядра внутри gui

// создаем ядро
QAPRSCore  *Core;

// устанавливаем у него порт и стартуем
Core = new QAPRSCore();
Core->setWorkPort(portBox->text().toInt());
Core->Start();

//ссылку на объект ядра кладем в MainWin
MainWin = new MainWindow();
MainWin->Core = Core;

//а MainWin становится клиентом
MainWin->atomGUI->beClient("127.0.0.1", portBox->text().toInt());

переделываем atomix на то, чем он является - QPubsub

Логические единицы системы и их обмен между собой данными

1. ядро
1.1 options
1.2 ports

2. gui
2.1 optionswindow - главное окошко системы
2.1.1 general core options
2.1.2 ports
2.1.3 beacons
2.1.4 query options
2.2 packets
2.3 messages
2.4 stations
2.5 maps
2.6 messenger

