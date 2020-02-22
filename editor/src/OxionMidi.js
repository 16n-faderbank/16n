import WebMidi from "webmidi";

export class OxionMidi {
  static isOxionSysex(data) {
    return data[1] == this.sysexMfgId[0] && data[2] == this.sysexMfgId[1] && data[3] == this.sysexMfgId[2];
  }

  static sortMidiInterfaces(a, b) {
    let aName = `${a.manufacturer} ${a.name}`;
    let bName = `${b.manufacturer} ${b.name}`;

    if (aName < bName) {
      return -1
    } else if (aName > bName) {
      return 1
    } else {
      return 0;
    }
  }

  static allInputs() {
    return WebMidi.inputs.sort(OxionMidi.sortMidiInterfaces);
  }

  static allOutputs() {
    return WebMidi.outputs.sort(OxionMidi.sortMidiInterfaces);
  }

  static sendConfig(sysexArray, output) {
    output.sendSysex(OxionMidi.sysexMfgId, [
      OxionMidi.updateConfigMsg
    ].concat(sysexArray));
  }

  static requestConfig(output) {
    output.sendSysex(OxionMidi.sysexMfgId, [
      OxionMidi.requestInfoMsg
    ]);
  }

}

OxionMidi.sysexMfgId = [0x7d, 0x00, 0x00];
OxionMidi.requestInfoMsg = 0x1F;
OxionMidi.updateConfigMsg = 0x0E;
