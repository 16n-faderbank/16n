
<script>
  export let checked;
</script>

<style></style>

<p>
  <label>
    <input type=checkbox bind:checked={checked}>
    <slot></slot>
  </label>
</p>