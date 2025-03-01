set(component_srcs "Main.c")
idf_component_register(SRCS "${component_srcs}" "RF/mainRF.c" "RFID/rfidmanager.c" "ESPNOW/espNow.c" "IR/Receive/ReceiveIR.c" "ESPNOW/espNow.c" "RF/Receiver/receiver.c"
        "RFID/Read/readRFID.c" "IR/Receive/ReceiveIR.c" "IR/IREncoder/ir_nec_encoder.c" "ESPNOW/EspNowHandler.c" "Mqtt/MqttHandler.c" "DispositivesInterfaces/IRDevices/IRDevices.c"
        "DispositivesInterfaces/RFDevices/RFDevices.c" "DispositivesInterfaces/RFIDDevices/RFIDDevices.c" "DispositivesInterfaces/IRDevices/Projector/ProjectorControl.c" "DispositivesInterfaces/IRDevices/LED/LEDControl.c"
        "DispositivesInterfaces/IRDevices/TV/TVControl.c" "DispositivesInterfaces/IRDevices/AC/ACControl.c"
        INCLUDE_DIRS ".")
