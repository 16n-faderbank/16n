
<script>
	import { createEventDispatcher } from 'svelte';

  import Icon from './Icon.svelte';

  export let label;
  export let clickMessageName = null;
  export let icon = null;
  export let disabled = false;

  const dispatch = createEventDispatcher();

  function dispatchClick() {
    dispatch('message', {
      name: clickMessageName
    })
  }
</script>

<style>
  div {
    margin: 0;
    display: inline-block;
  }
  button {
    text-align: left;
    border-radius: 5px;
    display: inline-block;
  }
</style>

<div>
  <button disabled={disabled} on:click={dispatchClick}>
    {#if icon}
    <Icon i="{icon}" />
    {/if}
    { label }
  </button>
</div>