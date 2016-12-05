import React, { Component } from 'react';
import './App.css';
import 'semantic-ui-css/semantic.css';
import ParametersPannel from './RangeInput.js';
import CarSelector from './CarSelector.js';

const carParameters = [
  { id: "battery",
    label: "Battery capacity",
    min: 10,
    max: 100,
    step: 0.5,
    defaultValue: 20,
    unit: "kWh"
  },
  { id: "SOC_min",
    label: "Energy reserve",
    min: 5,
    max: 30,
    step: 1,
    defaultValue: 12,
    unit: "%"
  },
  { id: "eff",
    label: "Efficiency",
    min: 10.0,
    max: 30.0,
    step: 0.1,
    defaultValue: 18.0,
    unit: "Wh/km"
  },
];

class App extends Component {
  render() {
    return (
      <div className="App">
        <CarSelector />
        <ParametersPannel
          parameters={carParameters}/>
      </div>
    );
  }
}

export default App;
