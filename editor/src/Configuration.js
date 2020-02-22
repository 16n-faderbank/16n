import { logger } from "./logger.js";

export class ConfigurationObject {
  constructor({ ledOn = true,
    ledFlash = true,
    controllerFlip = false,
    deviceId = 0,
    i2cMaster = false,
    fadermin = 0,
    fadermax = 8192,
    firmwareVersion = "unknown",
    usbControls = [],
    trsControls = [] }) {

    this.ledOn = ledOn;
    this.ledFlash = ledFlash;
    this.controllerFlip = controllerFlip;
    this.usbControls = usbControls;
    this.trsControls = trsControls;
    this.deviceId = deviceId;
    this.firmwareVersion = firmwareVersion;
    this.i2cMaster = i2cMaster;
    this.fadermin = fadermin;
    this.fadermax = fadermax;
  }

  isEquivalent(otherConfig) {
    let optionEquivalents = (this.ledOn == otherConfig.ledOn &&
      this.ledFlash == otherConfig.ledFlash &&
      this.controllerFlip == otherConfig.controllerFlip);
    
    if(("i2cMaster" in this) || ("i2cMaster" in otherConfig)) {
      optionEquivalents = optionEquivalents && (this.i2cMaster == otherConfig.i2cMaster);
    }

    if(("fadermax" in this) || ("fadermax" in otherConfig)) {
      optionEquivalents = optionEquivalents && (this.fadermax == otherConfig.fadermax)
                                            && (this.fadermin == otherConfig.fadermin);
    }

    let usbEquivalent = true;
    let trsEquivalent = true;

    this.usbControls.forEach((control, i) => {
      let otherControl = otherConfig.usbControls[i];
      if (control.channel != otherControl.channel || control.cc != otherControl.cc) {
        usbEquivalent = false
      }
    });

    this.trsControls.forEach((control, i) => {
      let otherControl = otherConfig.trsControls[i];
      if (control.channel != otherControl.channel || control.cc != otherControl.cc) {
        usbEquivalent = false
      }
    });

    return optionEquivalents && usbEquivalent && trsEquivalent;
  }

  toSysexArray() {
    let array = Array.from({ length: 84 }, (v) => 0)

    array[0] = this.deviceId;
    array[1] = this.firmwareArray()[0];
    array[2] = this.firmwareArray()[1];
    array[3] = this.firmwareArray()[2];

    array[4] = this.ledOn ? 1 : 0;
    array[5] = this.ledFlash ? 1 : 0;
    array[6] = this.controllerFlip ? 1 : 0;

    // TODO: check all this i2c stuff
    array[7] = this.i2cMaster

    let faderminMSB = this.fadermin >> 7 
    let faderminLSB = this.fadermin - (faderminMSB << 7);
    array[8] = faderminLSB
    array[9] = faderminMSB
    let fadermaxMSB = this.fadermax >> 7;
    let fadermaxLSB = this.fadermax - (fadermaxMSB << 7);
    array[10] = fadermaxLSB
    array[11] = fadermaxMSB

    let usbChannelOffset = 20;
    let trsChannelOffset = 36;
    let usbControlOffset = 52;
    let trsControlOffset = 68;

    this.usbControls.forEach((control, index) => {
      array[index + usbChannelOffset] = control.channel;
      array[index + usbControlOffset] = control.cc;
    });
    this.trsControls.forEach((control, index) => {
      array[index + trsChannelOffset] = control.channel;
      array[index + trsControlOffset] = control.cc;
    });

    return array;
  }

  toJsonString() {
    let o = ConfigurationObject.clone(this);
    // truncate all controllers to length $length;
    let controllerCount = this.device().controlCount;

    o.usbControls = o.usbControls.splice(0,controllerCount);
    o.usbControls.forEach(c => delete(c.val));

    o.trsControls = o.trsControls.splice(0,controllerCount);

    return JSON.stringify(o, false, 2);
  }

  isNewConfigInvalid(json) {
    if(json.deviceId != this.deviceId) {
      return(`Cannot update - this data file is for a ${ConfigurationObject.devices[json.deviceId].name}, but you are trying to install it on a ${this.device().name} `)
    }

    // if(json.firmwareVersion != this.firmwareVersion) {
    //   return(`Cannot update - this data file is for firmware version ${json.firmwareVersion}, but you are trying to install it on a device running firmware version ${this.firmwareVersion}`);
    // }

    return false;
  }

  updateFromJson(json) {
    Object.keys(json).forEach(key => {
      this[key] = json[key];
    })

    return this;
  }

  device() {
    return ConfigurationObject.devices[this.deviceId];
  }

  firmwareArray() {
    let arr = this.firmwareVersion.trim().split(".");
    return arr.map(n => parseInt(n));
  }

  static clone(obj) {
    let newObj = new ConfigurationObject({
      ledOn: obj.ledOn,
      ledFlash: obj.ledFlash,
      controllerFlip: obj.controllerFlip,
      usbControls: [],
      trsControls: [],
      deviceId: obj.deviceId,
      firmwareVersion: obj.firmwareVersion,
      i2cMaster: obj.i2cMaster,
      fadermin: obj.fadermin,
      fadermax: obj.fadermax,
    });

    obj.usbControls.forEach((control, i) => {
      newObj.usbControls[i] = { ...control };
    })
    obj.trsControls.forEach((control, i) => {
      newObj.trsControls[i] = { ...control };
    })

    return newObj
  }

  

  static returnConfigHashFromSysex(data) {
    logger("Generating config from", data);
    let offset = 8;

    let deviceId = data[5];
    let firmwareVersion = data[6] + "." + data[7] + "." + data[8];

    let ledOn = data[1+offset];
    let ledFlash = data[2+offset];
    let controllerFlip = data[3+offset];

    let i2cMaster = data[4+offset] == 1;

    let faderminLSB = data[5+offset];
    let faderminMSB = data[6+offset]
    let fadermin = (faderminMSB << 7) + faderminLSB;

    let fadermaxLSB = data[7+offset];
    let fadermaxMSB = data[8+offset]
    let fadermax = (fadermaxMSB << 7) + fadermaxLSB;

    let usbControls = [];
    let trsControls = [];

    data.slice(17+offset, 33+offset).forEach((chan, i) => {
      if (chan != 0x7f) {
        usbControls[i] = {
          channel: chan
        }
      }
    });

    data.slice(33+offset, 49+offset).forEach((chan, i) => {
      if (chan != 0x7f) {
        trsControls[i] = {
          channel: chan
        }
      }
    });

    data.slice(49+offset, 65+offset).forEach((cc, i) => {
      if (cc != 0x7f) {
        usbControls[i].cc = cc;
      }
    });

    data.slice(65+offset, 81+offset).forEach((cc, i) => {
      if (cc != 0x7f) {
        trsControls[i].cc = cc;
      }
    });

    usbControls.forEach((c) => c.val = 0);

    return new ConfigurationObject({
      ledOn,
      ledFlash,
      controllerFlip,
      usbControls,
      trsControls,
      deviceId,
      firmwareVersion,
      i2cMaster,
      fadermin,
      fadermax
    });
  }
}

ConfigurationObject.devices = [
  {
    name: "unknown",
    controlCount: 0,
    capabilities:{}
  },
  {
    name: "Oxion development board",
    controlCount: 4,
    capabilities: {
      led: true
    }
  },
  {
    name: "16n",
    controlCount: 16,
    capabilities: {
      i2c: true,
      led: true
    }
  }
];
