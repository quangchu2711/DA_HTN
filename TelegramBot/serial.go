package main
import (
    "fmt"
    "log"
    // "bufio"
    "github.com/jacobsa/go-serial/serial"
    "github.com/ghodss/yaml"
    "io"
    "io/ioutil"
    mqtt "github.com/eclipse/paho.mqtt.golang"
)

type APP_FileConfig_t struct {
    /* MQTT */
    Broker string
    User string
    Password string
    TopicTxLed string
    TopicRxSensor string
}
var g_cfgFile APP_FileConfig_t
var g_ledClient mqtt.Client
var g_portDevice io.ReadWriteCloser
var g_err error

var APP_RecevieMQTTMessage mqtt.MessageHandler = func(client mqtt.Client, msg mqtt.Message) {
    mqttMsg := string(msg.Payload())
    fmt.Printf("Received message: %s from topic: %s\n", mqttMsg, msg.Topic())
    APP_SendToSerial(mqttMsg)
}

func APP_SendMQTTMessage(msg string) {
    g_ledClient.Publish(g_cfgFile.TopicRxSensor, 0, false, msg)
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

func APP_SerialBegin(port string, baud uint) (io.ReadWriteCloser, error){
    var options serial.OpenOptions

    // Set up options.
    options = serial.OpenOptions{
      PortName: port,
      BaudRate: baud,
      DataBits: 8,
      StopBits: 1,
      MinimumReadSize: 4,
    }

    //Open the port.
    var portDev io.ReadWriteCloser
    var err error
    portDev, err = serial.Open(options)

    return portDev, err
}

func APP_SendToSerial(msg string) {
    msg += "\n"
    g_portDevice.Write([]byte(msg))
    fmt.Println("Serial => " +  msg)
}

func APP_Init() {
    APP_ConfigFile()
    g_ledClient = APP_MqttBegin(g_cfgFile.Broker, g_cfgFile.User, g_cfgFile.Password, &APP_RecevieMQTTMessage)
    g_ledClient.Subscribe(g_cfgFile.TopicTxLed, 1, nil)
    fmt.Println("MQTT connected")
    g_portDevice, g_err = APP_SerialBegin("COM21", 9600)
    if g_err != nil {
      log.Fatalf("serial.Open: %v", g_err)
    }else {
        fmt.Println("Serial connected")
    }
}


func main() {
    APP_Init()

    for
    {

    }
    // // Set up options.
    // options := serial.OpenOptions{
    //   PortName: "COM4",
    //   BaudRate: 9600,
    //   DataBits: 8,
    //   StopBits: 1,
    //   MinimumReadSize: 4,
    // }

    // // Open the port.
    // port, err := serial.Open(options)
    // if err != nil {
    //   log.Fatalf("serial.Open: %v", err)
    // }

    // // Make sure to close it later.
    // defer port.Close()

    // scanner := bufio.NewScanner(port)
    // for scanner.Scan() {
    //     fmt.Println(scanner.Text())
    // }
    // if err := scanner.Err(); err != nil {
    //     log.Fatal(err)
    // }
}

