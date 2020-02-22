<script>
  import WebMidi from "webmidi";
  import { onMount } from 'svelte'
  import { ConfigurationObject } from "../Configuration.js";
  import { OxionMidi } from "../OxionMidi.js";

  import {
    configuration,
    editConfiguration,
    midiInputs,
    midiOutputs,
    selectedMidiInput,
    selectedMidiOutput,
    webMidiEnabled
  } from "../stores.js";

  WebMidi.enable(function(err) {
    if (err) {
      console.log("WebMidi could not be enabled.", err);
    } else {
      console.log("WebMidi enabled!");
      webMidiEnabled.update(old => true);
      setupMidiHeartBeat();
    }
  }, true); // enable sysex

  function setupMidiHeartBeat() {

    console.log("Setting up hearbeat")

    doMidiHeartBeat();

    WebMidi.addListener("connected", (e) => {
      console.log('connected event', e)

      midiInputs.set(OxionMidi.allInputs(WebMidi));
      midiOutputs.set(OxionMidi.allOutputs(WebMidi));

      // ToDO TODO
      // if there's not a midi input, and there is now one with a name of 16n, set it
      // if there's not a midi output, and there is now one with a name of 16n, set it

      selectedMidiInput.update(i => $selectedMidiInput);
      selectedMidiOutput.update(i => $selectedMidiOutput);
      doMidiHeartBeat();
    });

    WebMidi.addListener("disconnected", (e) => {
      console.log('disconnected event', e)
      midiInputs.set(OxionMidi.allInputs(WebMidi));
      midiOutputs.set(OxionMidi.allOutputs(WebMidi));

      // if($midiInputs.length > 0) {
        // selectedMidiInput.update(i => $midiInputs[0]);
        // console.log($midiInputs);
      // } else {
        selectedMidiInput.update(i => null);
      // }

      // if($midiOutputs.length > 0) {
        // selectedMidiOutput.update(i => $midiOutputs[0]);
        // console.log($midiOutputs);
      // } else {
        selectedMidiOutput.update(i => null);
      // }
      
      $configuration = null;
      doMidiHeartBeat();
    });
    setInterval(() => {
      doMidiHeartBeat();
    }, 5000);
  }

  function doMidiHeartBeat() {
    if (!$selectedMidiInput && $midiInputs.length > 0) {
      // TODO TODO if 16n
      let sixteenN = $midiInputs.find(input => input.name.match(/.*16n.*/));
      if(sixteenN) {
        selectedMidiInput.update(i => sixteenN);
      }
    }
    if (!$selectedMidiOutput && $midiOutputs.length > 0) {
      // TODO TODO if 16n
      let sixteenN = $midiOutputs.find(output => output.name.match(/.*16n.*/));
      if(sixteenN) {
        selectedMidiOutput.update(i => sixteenN);
      }
    }

    if (!$configuration && $selectedMidiInput && $selectedMidiOutput) {
      listenForCC($selectedMidiInput);
      listenForSysex($selectedMidiInput);
      console.log($midiInputs[0]);
      console.log("Hearbeat requesting config.");
      requestConfig();
    }
  }

  selectedMidiInput.subscribe(newInput => {
    if (newInput) {
      $midiInputs.forEach(input => {
        console.log("Removing put from ", input);
        input.removeListener();
      });
      listenForCC(newInput);
      listenForSysex(newInput);
      configuration.update(n => null);
      requestConfig();
    }
  });

  selectedMidiOutput.subscribe(newOutput => {
    if (newOutput) {
      configuration.update(n => null);
      requestConfig();
    }
  });

  function controllerMoved(event) {
    if ($configuration) {
      $configuration.usbControls.forEach(c => {
        if (c.channel == event.channel && c.cc == event.controller.number) {
          c.val = event.value;
        }
      });
      configuration.update(c => $configuration); // trigger reactivity
    }
  }

  function listenForCC(input) {
    input.addListener("controlchange", "all", e => {
      controllerMoved(e);
    });
    console.log("Attached CC listener to ", input.name);
  }

  function listenForSysex(input) {
    input.addListener("sysex", "all", e => {
      let data = e.data;
      if (!OxionMidi.isOxionSysex(data)) {
        console.log("Sysex not for us:", data);
        return;
      }
      if (data[4] == 0x0f) {
        // it's an c0nFig message!
        let configBytes = [];
        data.slice(5, data.length - 1).forEach(d => {
          configBytes.push(d.toString(16).padStart(2, "0"));
        });
        configuration.update(n =>
          ConfigurationObject.returnConfigHashFromSysex(data)
        );
        console.log("Received config", $configuration);
        if (document.getElementById("current_config")) {
          document.getElementById("current_config").value = configBytes.join(
            " "
          );
        }
      }
    });
    console.log("Attached sysex listener to ", input.name);
  }

  function requestConfig() {
    if ($selectedMidiInput && $selectedMidiOutput) {
      console.log("Requesting config over " + $selectedMidiOutput.name);
      console.log("Hoping to receive on " + $selectedMidiInput.name);
      OxionMidi.requestConfig($selectedMidiOutput);
    }
  }

</script>

<div>
  <slot></slot>
</div>