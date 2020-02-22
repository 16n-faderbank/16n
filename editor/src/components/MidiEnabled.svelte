<script>
  import {
    webMidiEnabled
  } from "../stores.js";

  export let fallback = null;

</script>

{#if $webMidiEnabled}
  <slot></slot>
{:else if fallback}
  {fallback}
{/if}