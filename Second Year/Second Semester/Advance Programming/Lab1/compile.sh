#!bin/bash

javac -d bin -sourcepath src src/Main/Main.java
java  -Xms4G -Xmx4G -cp bin Main.Main 10 10

rm -r bin
