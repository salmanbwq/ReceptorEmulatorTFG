set(component_srcs "Main.c")
idf_component_register(SRCS "${component_srcs}" "RF/mainRF.c" "RFID/rfidmanager.c" "ESPNOW/espNow.c" "IR/Receive/ReceiveIR.c" "ESPNOW/espNow.c" "RF/Receiver/receiver.c"
        "RFID/Read/readRFID.c" "IR/Receive/ReceiveIR.c" "IR/IREncoder/ir_nec_encoder.c" "IR/IRManager.c" "ESPNOW/EspNowHandler.c" "Mqtt/MqttHandler.c"
        INCLUDE_DIRS ".")
