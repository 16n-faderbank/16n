<script>
  import { setContext } from 'svelte'
  import { ConfigurationObject } from "./Configuration.js";
  import { ImportExport } from "./ImportExport.js";
  import { logger } from "./logger.js";
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
  import OtherOptions from "./components/OtherOptions.svelte";
  import Subhead from "./components/Subhead.svelte";
  import { Tabs, TabList, TabPanel, Tab } from './components/tabs';

  // window.debugMode = true;

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
        ImportExport.import($editConfiguration, $configuration, editConfiguration);
        break;
      case "exportConfig":
        ImportExport.export($configuration);
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
    }
  }

  function transmitConfig() {
    let sysexArray = $editConfiguration.toSysexArray();
    logger("Sending sysex:", sysexArray);

    OxionMidi.sendConfig(sysexArray, $selectedMidiOutput);

    $configuration = $editConfiguration;

    editMode = !editMode;
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
              <OtherOptions />
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
