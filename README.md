# ThingSpeakMqtt

platformio.ini should look something like this:

    [env:mayfly]
    monitor_speed = 115200
    board = mayfly
    platform = atmelavr
    framework = arduino
    lib_ignore = RTCZero
    build_flags =
        -D MQTT_USERNAME=YOUR_USERNAME
        -D MQTT_PASSWORD=YOUR_PASSWORD
        -D MQTT_CLIENT_ID=YOUR_CLIENT_ID
        -D THINGSPEAK_CHANNEL_ID='"1851144"'
        -DSDI12_EXTERNAL_PCINT
        ; DEBUG OPTIONS:
        ; -DNEOSWSERIAL_EXTERNAL_PCINT
        ; -D STANDARD_SERIAL_OUTPUT=Serial
        ; -D DEBUGGING_SERIAL_OUTPUT=Serial
        ; -D DEEP_DEBUGGING_SERIAL_OUTPUT=Serial    
        ; -D MS_LOGGERBASE_DEBUG
        ; https://github.com/EnviroDIY/ModularSensors/commit/cc9a66edb4d0576c6e3f0d15e908876d30224879
        ; -D MS_DEBUGGING_STD
    lib_deps =
        EnviroDIY_ModularSensors
        vshymanskyy/TinyGSM@^0.11.5
