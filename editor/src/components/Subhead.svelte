<script>
  export let title;
</script>

<style>
  .subhead {
    display: flex;
  }

  .subhead h2 {
    flex: 3;
    margin: 0;
  }

  .subhead :global(p) {
    margin: 0;
  }

  .subhead .details {
    flex: 2;
    text-align: right;
  }
</style>

<div class="subhead">
  <h2>
    {title}
  </h2>
  <div class='details'>
    <slot></slot>
  </div>
</div>