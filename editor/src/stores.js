import { writable } from 'svelte/store';

export const configuration = writable(null);
export const editConfiguration = writable(null);
export const midiInputs = writable([]);
export const midiOutputs = writable([]);
export const selectedMidiInput = writable(null);
export const selectedMidiOutput = writable(null);
export const webMidiEnabled = writable(null);