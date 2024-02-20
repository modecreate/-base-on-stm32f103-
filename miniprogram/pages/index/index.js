//index.js
const app = getApp()
var isConnected = false
function sendInfo(that){
  
  wx.request({
    url:"http://api.heclouds.com/devices/793987873",

    header:{
      'content-type':'application/x-www-form-urlencoded',
      "api-key":"Wl4saKHmTZDr=tau2trIJsbLoaY=",
    },
    success (res){
      if(res.data.data.online){
        console.log("已连接")
        getInfo(that)
        isConnected = true
        console.log(isConnected)
      }else{
        console.log("连接丢失")
        getInfo(that)
        isConnected = false
      }
      
    },
    fail(res) {
      console.log("请求失败")
      isConnected = false
    },
    complete() {
      if (isConnected) {
        that.setData({ isConnected: true })
      } else {
        that.setData({ isConnected: false })
      }
    }
  })
}

function getInfo(that){
  wx.request({
    url:"http://api.heclouds.com/devices/793987873/datastreams",
    method :"GET",
    //设备ID
    //api-key
    header:{
      'content-type':'application/x-www-form-urlencoded',
      "api-key":"Wl4saKHmTZDr=tau2trIJsbLoaY=",
    },
    data:{
    },
    success(res) {
     that.setData({
       light1:res.data.data[0].current_value,
       ps1:res.data.data[1].current_value,
       BEEP1:res.data.data[2].current_value,
       light:res.data.data[0].id,
       ps:res.data.data[1].id,
       BEEP:res.data.data[2].id,
     })

    console.log(res.data.data[0].id,":",res.data.data[0].current_value)
    console.log(res.data.data[1].id,":",res.data.data[1].current_value)
    console.log(res.data.data[2].id,":",res.data.data[2].current_value)    
    }
  })
}

function control(hardware_id,switch_value){
  wx.request({
    url:"http://api.heclouds.com/cmds?device_id=793987873",
    method :'POST',
    //设备ID
    //api-key
    header:{
      'content-type':'application/x-www-form-urlencoded',
      "api-key":"Wl4saKHmTZDr=tau2trIJsbLoaY=",
    },
    data:{switch_value},
    success(res) {
      console.log(res)
      if(res.data.data){
        console.log("成功")
      }else {
        console.log("失败")
      }
    }
  })
}

Page({

  data: {
    light1:0,
    ps1:0,
    BEEP1:0,
  },



  change:function(event) {
    var cmdData = event.detail.value == true ? 1:0
    switch (event.currentTarget.id) {
      case 'system':
        control('system',cmdData)
        if(cmdData == 1){
          console.log('系统开启')
        }else{
          console.log('系统关闭')
        }
        break;
      default:
        break;
    }
    
  },

  switchchange:function(hardware_id){
    this.controlsystem(hardware_id,switch_value);
  },


  onShow: function(){
    var that = this
    var timer = setInterval(function(){
      sendInfo(that)
    },5000)
  },
 
  onLoad: function() {
    var that = this
    sendInfo(that)
  },

})
