import React from 'react';
import ReactDOM from 'react-dom';
import App from './App';
import 'whatwg-fetch'
import Promise from 'promise-polyfill';
import './index.css';

// To add to window
if (!window.Promise) {
  window.Promise = Promise;
}

ReactDOM.render(
  <App />,
  document.getElementById('root')
);
