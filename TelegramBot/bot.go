package main

import (
    "time"
    "strconv"
    "fmt"
)

func BOT_CreateTextAboutTimeDifference(updateTime string) string {
    var text string

    previousTime, _ := time.Parse("01-02-2006 15:04:05", updateTime)
    timeNow := time.Now()
    timeNowFormat := timeNow.Format("01-02-2006 15:04:05")
    currentTime, _ := time.Parse("01-02-2006 15:04:05", timeNowFormat)
    diffTime := currentTime.Sub(previousTime)
    hours := int(diffTime.Hours())
    minutes := int(diffTime.Minutes())
    seconds := int(diffTime.Seconds())

    if hours > 0 {
        if hours > 24 {
            text = ""
        }else {
            text = "<i>" + "Cập nhật " + strconv.Itoa(hours) + " giờ trước" + "</i>"
        }

    }else if minutes > 0 {
        text =  "<i>" + "Cập nhật " + strconv.Itoa(minutes) + " phút trước" + "</i>"

    }else if seconds > 0 {
        text =  "<i>" + "Cập nhật " + strconv.Itoa(seconds) + " giây trước" + "</i>"
    }
    if seconds < 3 {
        text = ""
    }

    return text

}

func BOT_Progress (msg string, homeStatus *APP_HomeStatus_t) (bool, string) {
    var inlineType bool
    fmt.Println("Bot: " + msg)

    switch (msg) {
    case "Help", "help":
        msg = "[Bảng điều khiển/Điều khiển thiết bị/Xem tình trạng căn phòng]"
        inlineType = true;
    case "Điều khiển thiết bị", "Contorl", "control":
        msg = "[Chọn câu lệnh điều khiển/Bật đèn phòng khách/Tắt đèn phòng khách/Bật đèn phòng bếp/Tắt đèn phòng bếp]"
        inlineType = true;
    case "Xem tình trạng căn phòng", "Status", "status":
        msg = "[Tình trạng căn phòng/Phòng bếp/Phòng khách]"
        inlineType = true;
    case "Phòng bếp":
        var timeDifferent string
        var lrledSta string
        lrSta := homeStatus.LivingRoom
        tem := "Nhiệt độ phòng bếp: " + "<b>" + lrSta.SensorStatus.Temperature + "°C" + "</b>"
        hum := "Độ ẩm phòng bếp: " + "<b>" + lrSta.SensorStatus.Humidity + "%" + "</b>"
        sensorResponse := tem + "\n" + hum
        fmt.Println(sensorResponse)
        timeDifferent = BOT_CreateTextAboutTimeDifference(lrSta.SensorStatus.TimeUpdate)
        if lrSta.LedStatus == true{
            lrledSta = "Đèn phòng bếp" + "<b>" + " đang bật" + "</b>"
        }else {
            lrledSta = "Đèn phòng bếp" + "<b>" + " đang tắt" + "</b>"
        }
        msg = sensorResponse + "\n" + timeDifferent + "\n" + lrledSta
        inlineType = false
    case "Bật đèn phòng bếp":
        APP_SendMQTTMessage("ON1")
        msg = "Đèn phòng bếp đã bật"
        inlineType = false
    case "Tắt đèn phòng bếp":
        APP_SendMQTTMessage("OFF1")
        msg = "Đèn phòng bếp đã tắt"
        inlineType = false
    case "Bật đèn phòng khách":
        APP_SendMQTTMessage("ON2")
        msg = "Đèn phòng khách đã bật"
        inlineType = false
    case "Tắt đèn phòng khách":
        APP_SendMQTTMessage("OFF2")
        msg = "Đèn phòng khách đã tắt"
        inlineType = false
    default: /* Khác tất cả tin nhắn */
        msg = "Câu lệnh không được hỗ trợ, vui lòng thử lại một số lệnh sau:\n[1] help\n[2] control\n[3] status"
        inlineType = false
    }

    return inlineType, msg
}