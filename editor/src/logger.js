
function logger(...obj) {
  if(window.debugMode) {
    console.log(...obj);
  }
}

export {logger};