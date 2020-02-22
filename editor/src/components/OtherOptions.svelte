
<script>
  import { logger } from "../logger.js";

  import {
    configuration,
    editConfiguration,
  } from "../stores.js";

  import CheckOption from "./CheckOption.svelte";

  function touchControl() {
    editConfiguration.update(old => $editConfiguration);
  }
</script>

<style>
  p.note {
    width: 600px;
    line-height: 1.2;
  }
</style>


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

<p class='note'><strong>Follower</strong> mode is for use with Teletype.</p>
<p class='note'><strong>Master</strong> mode is for use with Ansible, TXi, ER-301. 16n will not respond to Teletype when in master mode.</p>
<p class='note'>This will not take effect until you restart (disconnect/reconnect) your 16n.</p>

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
