<script>
  import {
    midiInputs,
    midiOutputs,
    selectedMidiInput,
    selectedMidiOutput,
  } from "../stores.js";

  import { onMount, onDestroy } from 'svelte';

  import { OxionMidi } from "../OxionMidi.js";
  import Icon from "./Icon.svelte";

  let container;
  let changed=false;
  let firstRunIn = true;
  let firstRunOut = true;

  onMount(() => {
    const unsubscribeInputs = midiInputs.subscribe(value => {
      if(firstRunIn) {
        firstRunIn = false;
        return;
      }
      changed = true;
      setTimeout(() => {
        changed = false
      },750);
    });

    const unsubscribeOutputs = midiOutputs.subscribe(value => {
      if(firstRunOut) {
        firstRunOut = false;
        return;
      }
      changed = true;
      setTimeout(() => {
        changed = false
      },750);
    });
    
    onDestroy(() => {
      unsubscribeInputs(); unsubscribeOutputs();
    });
  });

</script>

<style>
  .container {
    text-align: right;
  }

  .wrap {
    display: inline-block;
  }

  label {
    display: inline-block;
    font-weight: bold;
  }
  select {
    display: inline-block;
    width: 240px;
    transition: 0.75s ease;
  }
  
  .changed select {
    transition: 0.75s ease;
    box-shadow: 0 0 10px #800;
  }

  p {
    margin: 0;
  }
</style>

<div class="container {changed ? 'changed':''}">
  <div class='wrap'>
    <p>
      <!-- <Icon classList="mid" i="sliders-h" /> -->
      <label>Input:</label>
      {#if $midiInputs.length > 0}
        <select bind:value={$selectedMidiInput} on:change='{() => selectedMidiInput.update(old => $selectedMidiInput) }'>
          {#each $midiInputs as input}
            <!-- <option value={input}>{input.manufacturer} {input.name}</option> -->
            <option value={input}>{input.name}</option>
          {/each}
        </select>
      {:else}No MIDI inputs detected.{/if}
    </p>

    <p>
      <label>Output:</label>
      {#if $midiOutputs.length > 0}
        <select bind:value={$selectedMidiOutput} on:change='{() => selectedMidiOutput.update(old => $selectedMidiOutput) }'>
          {#each $midiOutputs as output}
            <!-- <option value={output}>{output.manufacturer} {output.name}</option> -->
            <option value={output}>{output.name}</option>
            i
        {/each}
      </select>
      {:else}No MIDI outputs detected.{/if}
    </p>
  </div>
</div>
