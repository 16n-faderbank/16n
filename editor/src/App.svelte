<script>
  import { setContext } from 'svelte'
  import { ConfigurationObject } from "./Configuration.js";
  import { OxionMidi } from "./OxionMidi.js";

  import {
    configuration,
    editConfiguration,
    selectedMidiInput,
    selectedMidiOutput,
    webMidiEnabled
  } from "./stores.js";

  import Button from "./components/Button.svelte";
  import CheckOption from "./components/CheckOption.svelte";
  import Control from "./components/Control.svelte";
  import DebugConsole from "./components/DebugConsole.svelte";
  import EditControl from "./components/EditControl.svelte";
  import Icon from "./components/Icon.svelte";
  import MidiContext from "./components/MidiContext.svelte";
  import MidiEnabled from "./components/MidiEnabled.svelte";
  import MidiSelector from "./components/MidiSelector.svelte";
  import Subhead from "./components/Subhead.svelte";
  import { Tabs, TabList, TabPanel, Tab } from './components/tabs';

  let editMode = false;
  let configDirty = false;

  editConfiguration.subscribe(c => {
    if (c && $configuration) {
      configDirty = !c.isEquivalent($configuration);
    }
  });

  function handleMessage(message) {
    switch (message.detail.name) {
      case "toggleEditMode":
        toggleEditMode();
        break;
      case "transmitConfig":
        transmitConfig();
        break;
      case "requestConfig":
        OxionMidi.requestConfig($selectedMidiOutput);
        break;
      case "importConfig":
        let fileInputNode = document.createElement('input');
        fileInputNode.setAttribute("type", "file");
        fileInputNode.setAttribute("id", "uploadedConfig");
        fileInputNode.setAttribute("onchange", "window.handleFiles(this.files)");

        window.handleFiles = (files) => {
          if(files.length > 0) {
            let newConfig = files[0];
            console.log(newConfig);
            const reader = new FileReader();

            reader.addEventListener("load", e => {
              const newConfigData = JSON.parse(reader.result);
              const invalidConfig = $editConfiguration.isNewConfigInvalid(newConfigData);
              if(invalidConfig) {
                alert(invalidConfig);
                return;
              } else {
                editConfiguration.update(old => $editConfiguration.updateFromJson(newConfigData));
                if($editConfiguration.isEquivalent($configuration)) {
                  alert("Imported configuration is identical to currently loaded configuration; no changes to upload.");
                } else {
                  alert("New configuration imported. Choose 'update controller' to import, or 'Cancel' to abort");
                }
              }
            });

            reader.readAsText(newConfig);

            window.handleFiles = null;
          }
        }

        fileInputNode.click();

        fileInputNode.remove();
        break;
      case "exportConfig":
        let dataStr = "data:text/json;charset=utf-8," + encodeURIComponent($configuration.toJsonString());

        let downloadAnchorNode = document.createElement('a');
        downloadAnchorNode.setAttribute("href",     dataStr);
        downloadAnchorNode.setAttribute("download", `${$configuration.device().name}_controller_config.json`);
        document.body.appendChild(downloadAnchorNode); // required for firefox
        downloadAnchorNode.click();
        downloadAnchorNode.remove();
        break;
      default:
        break;
    }
  }

  function toggleEditMode() {
    editMode = !editMode;
    if (editMode) {
      let oldConfig = ConfigurationObject.clone($configuration);
      editConfiguration.update(c => oldConfig);
    } else {
      console.log($editConfiguration);
      console.log($configuration);
      console.log($configuration == $editConfiguration);
      // this will have been changed by bindings.
    }
  }

  function transmitConfig() {
    let sysexArray = $editConfiguration.toSysexArray();
    console.log(sysexArray);

    OxionMidi.sendConfig(sysexArray, $selectedMidiOutput);

    $configuration = $editConfiguration;

    editMode = !editMode;
  }

  function touchControl() {
    editConfiguration.update(old => $editConfiguration);
    console.log("touch", $editConfiguration);
  }
</script>

<style>

  #controls {
    display: flex;
    min-width: calc(16 * 60px);
  }

  #head {
    margin-bottom: 1rem;
    border-bottom: 1px solid #ccc;
    display: flex;
  }

  #head h1 {
    flex: 1 0;
    margin: 0 0 2rem;
  }

  #head .details {
    flex: 1 0;
  }

  p.device {
    text-align: right;
  }

  p.note {
    width: 400px;
    line-height: 1.2;
  }

  main {
    width: 75%;
    margin: 0 auto;
    min-width: calc(16*75px);
    /* background: #fff; */
  }

</style>

<MidiContext>
  <main>
    <div id="head">
      <h1>16n configuration tool</h1>
      <MidiEnabled>
        <div class="details">
          <!-- <MidiSelector /> -->
          {#if $configuration}
            <p class='device'>Connected: <strong>{$configuration.device().name}</strong> running firmware <strong>{$configuration.firmwareVersion}</strong></p>
          {:else}
            <p class='device'>No device conncected.</p>
          {/if}
        </div>
      </MidiEnabled>
    </div>
    <MidiEnabled fallback="WebMIDI could not be enabled. Please use a web browser that supports WebMIDI, such as Google Chrome.">
      {#if $configuration}
        {#if editMode}
          <Subhead title="Edit Configuration">
            <Button label="Cancel" icon="times" clickMessageName="toggleEditMode" on:message={handleMessage} />
            <Button label="Import config" icon="file-import" clickMessageName="importConfig" on:message={handleMessage}/>
            <Button label="Update controller" icon="download" clickMessageName="transmitConfig" disabled={!configDirty} on:message={handleMessage}  />
          </Subhead>
          <Tabs>
            <TabList>
              <Tab>USB</Tab> 
              <Tab>TRS Jack</Tab> 
              <Tab>Other Options</Tab> 
            </TabList>

            <TabPanel>
              <div id="controls">
                {#each $editConfiguration.usbControls as editControl, index}
                  {#if index < $configuration.device().controlCount}
                    <EditControl {editControl} {index} />
                  {/if}
                {/each}
              </div>
            </TabPanel>
          
            <TabPanel>
              <div id="controls">
                {#each $editConfiguration.trsControls as editControl, index}
                  {#if index < $configuration.device().controlCount}
                    <EditControl {editControl} {index} />
                  {/if}
                {/each}
              </div>
            </TabPanel>

            <TabPanel>
              {#if $configuration.device().capabilities.led}
              <CheckOption bind:checked={$editConfiguration.ledOn}>
                LED permanently on when powered
              </CheckOption>

              <CheckOption bind:checked={$editConfiguration.ledFlash}>
                LED flash on MIDI activity
              </CheckOption>
              {/if}

              <CheckOption bind:checked={$editConfiguration.controllerFlip}>
                Rotate controller 180º
              </CheckOption>

              {#if $configuration.device().capabilities.i2c}
              <h3>I2C Master/Follower</h3>
              <select bind:value={$editConfiguration.i2cMaster} on:change={touchControl}>
                <option value={false}>Follower</option>
                <option value={true}>Master</option>
              </select>

              <p>This will not take effect until you restart (disconnect/reconnect) your 16n.</p>

              <h3>Fader Minimum/Maximum scalars</h3>
              <div>
                <label>Fader Minimum read value</label>
                <input
                  type="number"
                  bind:value={$editConfiguration.fadermin}
                  on:change={touchControl}
                  min="0"
                  max="{(1 << 13) - 1}" />
              </div>
              <div>
                <label>Fader Maximum read value</label>
                <input
                  type="number"
                  bind:value={$editConfiguration.fadermax}
                  on:change={touchControl}
                  min="0"
                  max="{(1 << 13) - 1}" />
              </div>
              <p class='note'>You probably shouldn't touch this, unless you're having issues with your fader scaling. "Raw" analog read values are from 0 to 8192. The defaults are based on experience.</p>
              {/if}
            </TabPanel>
          </Tabs>

        {:else}
          <Subhead title="Current Configuration">
            <Button label="Export current config" icon="file-export" clickMessageName="exportConfig" on:message={handleMessage}/>
            <Button label="Edit Config" icon="pencil-alt" clickMessageName="toggleEditMode" on:message={handleMessage} />
            <!-- <Button label="Reload config from controller" icon="sync" clickMessageName="requestConfig" on:message={handleMessage}/> -->
          </Subhead>
          <Tabs>
            <TabList>
              <Tab>USB</Tab> 
              <Tab>TRS Jack</Tab> 
            </TabList>
            <TabPanel>
              <div id="controls">
                {#each $configuration.usbControls as control, index}
                  {#if index < $configuration.device().controlCount}
                    <Control {control} {index} />
                  {/if}
                {/each}
              </div>
            </TabPanel>

            <TabPanel>
              <div id="controls">
                {#each $configuration.trsControls as control, index}
                  {#if index < $configuration.device().controlCount}
                    <Control {control} {index} disableValue={true} />
                  {/if}
                {/each}
              </div>
              <p>There is no realtime preview of the TRS outputs.</p>
            </TabPanel>
          </Tabs>
        {/if}
        <p />
      {:else}
        <p>Connect a controller via USB.</p>
      {/if}
    </MidiEnabled>
  </main>

</MidiContext>
