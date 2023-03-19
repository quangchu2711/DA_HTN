package main

import (
    "log"
    "fmt"
    "io/ioutil"
    "github.com/ghodss/yaml"
    "time"
    "strings"
    mqtt "github.com/eclipse/paho.mqtt.golang"
)

type APP_FileConfig_t struct {
    /* Telegram */
    BotName string
    BotToken string
    GroupID int64
    /* MQTT */
    Broker string
    User string
    Password string
    TopicTxLed string
    TopicRxSensor string
}

type APP_DHTSensor_t struct {
    Temperature string
    Humidity string
    TimeUpdate string
}

type APP_LightSensor_t struct {
    Light string
    TimeUpdate string
}

type APP_LivingRoomStatus_t struct {
    SensorStatus APP_DHTSensor_t
    LedStatus bool
}

type APP_KitchenStatus_t struct {
    SensorStatus APP_LightSensor_t
    LedStatus bool
}

type APP_HomeStatus_t struct {
    LivingRoom APP_LivingRoomStatus_t
    Kitchen APP_KitchenStatus_t
}

var g_cfgFile APP_FileConfig_t
var g_serialClient mqtt.Client
var g_home APP_HomeStatus_t

var APP_RecevieMQTTMessage mqtt.MessageHandler = func(client mqtt.Client, msg mqtt.Message) {
    mqttMsg := string(msg.Payload())
    fmt.Printf("Received message: %s from topic: %s\n", mqttMsg, msg.Topic())
    sensorMsg := strings.Split(mqttMsg, "/")
    g_home.LivingRoom.SensorStatus.Temperature = sensorMsg[0]
    g_home.LivingRoom.SensorStatus.Humidity = sensorMsg[1]
    g_home.Kitchen.SensorStatus.Light = sensorMsg[2]
    currentTime := time.Now()
    g_home.Kitchen.SensorStatus.TimeUpdate = currentTime.Format("01-02-2006 15:04:05")
    g_home.LivingRoom.SensorStatus.TimeUpdate = currentTime.Format("01-02-2006 15:04:05")
    // fmt.Printf("DHT", g_home.LivingRoom)
}

func APP_SendMQTTMessage(msg string) {
    g_serialClient.Publish(g_cfgFile.TopicTxLed, 0, false, msg)
    fmt.Println("Publish: " + g_cfgFile.TopicTxLed + " msg: " + msg)
}

func APP_ConfigFile() {
    yfile, errReadFile := ioutil.ReadFile("appConfig.yaml")
    if errReadFile != nil {
      log.Fatal(errReadFile)
    }
    errUnFile := yaml.Unmarshal(yfile, &g_cfgFile)
    if errUnFile != nil {
      log.Fatal(errUnFile)
    }
}

func APP_MqttBegin(broker string, user string, pw string, messagePubHandler *mqtt.MessageHandler) mqtt.Client {
    var opts *mqtt.ClientOptions = new(mqtt.ClientOptions)

    opts = mqtt.NewClientOptions()
    opts.AddBroker(broker)
    opts.SetUsername(user)
    opts.SetPassword(pw)
    opts.SetDefaultPublishHandler(*messagePubHandler)
    client := mqtt.NewClient(opts)
    if token := client.Connect(); token.Wait() && token.Error() != nil {
        panic(token.Error())
    }

    return client
}

func APP_Init() {
    /* Đoc thông tin file xong, để lấy một số thông tin:
    ví dụ: Bot token, tên bot, groupID */
    APP_ConfigFile()
    /* Hiên thị thông tin lên bot token */
    fmt.Println(g_cfgFile.BotToken)
    g_serialClient = APP_MqttBegin(g_cfgFile.Broker, g_cfgFile.User, g_cfgFile.Password, &APP_RecevieMQTTMessage)
    g_serialClient.Subscribe(g_cfgFile.TopicRxSensor, 1, nil)
    fmt.Println("MQTT Connected")
}

func APP_Handle() {
    TELEGRAM_UpdateMessage(g_cfgFile.BotToken, g_cfgFile.GroupID, &g_home)
}

func main() {
    APP_Init()
    APP_Handle()
}
