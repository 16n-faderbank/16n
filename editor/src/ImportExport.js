import { logger } from "./logger.js";

export class ImportExport {
  static import(currentEditConfig, currentConfig, editConfigStore) {
    let fileInputNode = document.createElement('input');
    fileInputNode.setAttribute("type", "file");
    fileInputNode.setAttribute("id", "uploadedConfig");
    fileInputNode.setAttribute("onchange", "window.handleFiles(this.files)");

    window.handleFiles = (files) => {
      if (files.length > 0) {
        let newConfig = files[0];
        const reader = new FileReader();

        reader.addEventListener("load", e => {
          const newConfigData = JSON.parse(reader.result);
          const invalidConfig = currentEditConfig.isNewConfigInvalid(newConfigData);
          if (invalidConfig) {
            alert(invalidConfig);
            return;
          } else {
            editConfigStore.update(old => currentEditConfig.updateFromJson(newConfigData));
            if (currentEditConfig.isEquivalent(currentConfig)) {
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

  }

  static export(configObject) {
    let dataStr = "data:text/json;charset=utf-8," + encodeURIComponent(configObject.toJsonString());

    let downloadAnchorNode = document.createElement('a');
    downloadAnchorNode.setAttribute("href", dataStr);
    downloadAnchorNode.setAttribute("download", `${configObject.device().name}_controller_config.json`);
    document.body.appendChild(downloadAnchorNode); // required for firefox
    downloadAnchorNode.click();
    downloadAnchorNode.remove();
  }
}