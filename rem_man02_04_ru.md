# 2.4 Настройка порта KISS #
<br />
[2 Настройка портов](http://code.google.com/p/qaprs/wiki/rem_man02_ru) | [2.3 Настройка порта AGW](http://code.google.com/p/qaprs/wiki/rem_man02_03_ru) | [Вернуться к содержанию](http://code.google.com/p/qaprs/wiki/content_ru) | [2.5 Настройка порта FLDigi](http://code.google.com/p/qaprs/wiki/rem_man02_05_ru) |
[3 Настройка маяков](http://code.google.com/p/qaprs/wiki/rem_man03_ru)

Порт поддерживает работу с KISS TNC. Контроллер может быть как аппаратным, так и эмуляцией. Часто в качестве KISS модема используется программа [MixW](http://qaprs.googlecode.com/files/MixW214Rf.rar). В ней выбирается пакетный режим, необходимая скорость. Включается поддержка эмуляции TNC по порту A. В программе, которая должна пользоваться данным модемом указывается порт B. Теперь, если два этих порта соединить нуль-модемом, то получится эмуляция работы реального KISS TNC. В качестве виртуального нуль-модема может быть использована одна из множества программ ([ComEmulDrv3-MixW](http://qaprs.googlecode.com/files/ComEmulDrv3-MixW.rar), [N8VBvCOM](http://qaprs.googlecode.com/files/N8VBvCOMSetup-226a.rar)). Рекомендую первую.

В настройках порта указывается порт для KISS модема и скорость. При работе с виртуальным TNC выбирается второй порт из пары (тот к которому НЕ подключен MixW).

http://qaprs.googlecode.com/svn/wiki/port_kiss.PNG <br />

[2 Настройка портов](http://code.google.com/p/qaprs/wiki/rem_man02_ru) | [2.3 Настройка порта AGW](http://code.google.com/p/qaprs/wiki/rem_man02_03_ru) | [Вернуться к содержанию](http://code.google.com/p/qaprs/wiki/content_ru) | [2.5 Настройка порта FLDigi](http://code.google.com/p/qaprs/wiki/rem_man02_05_ru) |
[3 Настройка маяков](http://code.google.com/p/qaprs/wiki/rem_man03_ru)