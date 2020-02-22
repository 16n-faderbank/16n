# 16n editor

The 16n editor is a javascript app based around [Svelte](svelte).

## Requirements

- A recent version of node.js. 
- As a WebMIDI app, you need a client that can support it. That basically means Chrome right now.

## Installation

    npm install

## Running the development environment

    npm run dev

This will run a development environment at localhost:5000, with live reloading enabled.

By default, the server will only respond to requests from localhost. To allow connections from other computers, edit the `sirv` commands in package.json to include the option `--host 0.0.0.0`.

## Building and running in production mode

To create an optimised version of the app:

    npm run build

You can run the newly built app with `npm run start`. This uses [sirv](https://github.com/lukeed/sirv), which is included in your package.json's `dependencies` so that the app will work when you deploy to platforms like [Heroku](https://heroku.com).

## Icons

Icons are from FontAwesome: https://fontawesome.com/icons?d=gallery&m=free .